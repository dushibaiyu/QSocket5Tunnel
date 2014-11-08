#include "clientsocket.h"
#include <QHostAddress>
#include <QThread>
#include "userconfig.h"

ClientSocket::ClientSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor),userID(-1),lastsize(0),aes(nullptr)
{
    this->setSocketDescriptor(socketDescriptor);

    connect(this,&ClientSocket::readyRead,this,&ClientSocket::clientData);
    connect(this,&ClientSocket::disconnected,
            [&](){
                emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            });

}

ClientSocket::~ClientSocket()
{
    if (aes != nullptr)
        delete aes;
}

void ClientSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
}

void ClientSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        this->disconnectFromHost();
    }
}

void ClientSocket::clientData()
{
    QByteArray ba;
    if (lastsize == 0)
    {
        ba = this->read(6);
        lastsize = ba.toLongLong(0,16);
    }
    while (this->bytesAvailable() >= static_cast<qint64>(lastsize))
    {
        ba = this->read(this->lastsize);
        swapData data;
        {
            QDataStream stream(&ba,QIODevice::ReadWrite);
            stream >> data;
        }
        switch (data.operater)
        {
        case 0:
            handleSwapData(data);
            break;
        case 1:
            handleNewCon(data);
            break;
        case 2:
            handleDisCon(data);
            break;
        case 3:
            handleUserLog(data);
            break;
        default:
            break;
        }
        if (!this->atEnd() || this->bytesAvailable() > 6)
        {
            ba = this->read(6);
            lastsize = ba.toLongLong(0,16);
        }
        else
        {
            lastsize = 0 ;
            return ;
        }
    }
}

void ClientSocket::remoteData()
{
    RemoteSocket * sock = qobject_cast<RemoteSocket *>(QObject::sender());
    Q_ASSERT(sock);
    swapData data;
    //qDebug() << "remoteData" << sock->getSocketID();
    data.operater = 0;
    data.socketID = sock->getSocketID();
    data.userID = this->userID;
    data.data = encryptData(sock->readAll());
    QByteArray buf;
    {
        QDataStream stream(&buf,QIODevice::ReadWrite);
        stream << data;
    }
    qulonglong size = buf.size();
    QByteArray ba = QByteArray::number(size,16);
    while(ba.size() < 6)
        ba.insert(0,'0');
    ba += buf;
    this->write(ba);
}

void ClientSocket::remoteDisCon()
{
    RemoteSocket * sock = qobject_cast<RemoteSocket *>(QObject::sender());
    Q_ASSERT(sock);
    if (socketList.contains(sock->getSocketID()))
    {
        swapData data;
        data.operater = 2;
        data.socketID = sock->getSocketID();
        data.userID = this->userID;
        QByteArray buf;
        {
            QDataStream stream(&buf,QIODevice::ReadWrite);
            stream << data;
        }
        qulonglong size = buf.size();
        QByteArray ba = QByteArray::number(size,16);
        while(ba.size() < 6)
            ba.insert(0,'0');
        ba += buf;
        this->write(ba);
        socketList.remove(sock->getSocketID());
    }
    delete sock;
}

void ClientSocket::handleNewCon(swapData &data)
{
    if (!decryptData(data)) return;
    QPair<QString,qint16> host;
    {
        QDataStream stream(&data.data,QIODevice::ReadWrite);
        stream >> host;
    }
    //qDebug() << host.first << host.second;
    if (host.first.isEmpty()) return;
    RemoteSocket * sock = new RemoteSocket(data.socketID,this);
    connect(sock,&RemoteSocket::readyRead,this,&ClientSocket::remoteData);
    connect(sock,&RemoteSocket::disconnected,this,&ClientSocket::remoteDisCon);
    socketList.insert(data.socketID,sock);
    sock->connectToHost(host.first,host.second);
}

void ClientSocket::handleDisCon(swapData &data)
{
    RemoteSocket * sock = socketList.value(data.socketID,nullptr);
    if (sock != nullptr)
    {
        socketList.remove(data.socketID);
        sock->disconnectFromHost();
    }
}

void ClientSocket::handleSwapData(swapData &data)
{
    RemoteSocket * sock = socketList.value(data.socketID,nullptr);
    if (sock != nullptr && decryptData(data))
    {
        sock->write(data.data);
    }
    else
    {
        data.operater = static_cast<char>(0X02);
        data.data.clear();
        QByteArray buf;
        {
            QDataStream stream(&buf,QIODevice::ReadWrite);
            stream << data;
        }
        qulonglong size = buf.size();
        QByteArray ba = QByteArray::number(size,16);
        while(ba.size() < 6)
            ba.insert(0,'0');
        ba += buf;
        this->write(ba);
    }
}

void ClientSocket::handleUserLog(swapData &data)
{
    QPair<QString,QString> user;
    {
        QDataStream stream(&data.data,QIODevice::ReadWrite);
        stream >> user;
    }
    if (user.first.isEmpty() || user.second.isEmpty())
    {
        data.userID = -1;
        data.data.clear();
    }
    else
    {
        data.userID = UserConfig::getClass().getUserId(user.first,user.second,token);
        if (data.userID > 0 && (!token.isEmpty()))
        {
            this->userID = data.userID;
            data.data = token.toUtf8();
            if (aes != nullptr)
                delete aes;
            aes = new BotanAES256(token);
        }
        else
        {
            data.data.clear();
        }
    }
    QByteArray buf;
    {
        QDataStream stream(&buf,QIODevice::ReadWrite);
        stream << data;
    }
    qulonglong size = buf.size();
    QByteArray ba = QByteArray::number(size,16);
    while(ba.size() < 6)
        ba.insert(0,'0');
    ba += buf;
    this->write(ba);
}

bool ClientSocket::decryptData(swapData &data)
{
    if (data.data.isEmpty()) return false;
    return true;//TODO:加密优化，现在没有加密传输
    if (userID ==- 1)
    {
        if (data.userID <= 0)
        {
            return false;
        }
        token =UserConfig::getClass().getToken(data.userID);
        if (token.isEmpty())
        {
            userID = -1;
            return false;
        }
        if (aes != nullptr)
            delete aes;
        aes = new BotanAES256(token);
    }
    data.data = aes->Decrypt(data.data);
    if (data.data.isEmpty()) return false;
    return true;
}

QByteArray ClientSocket::encryptData(const QByteArray &data)
{
    return data;//TODO:加密优化，现在没有加密传输
    return aes->Encrypt(data);
}
