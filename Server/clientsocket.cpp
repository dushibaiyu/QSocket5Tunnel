#include "clientsocket.h"
#include <QHostAddress>
#include <QThread>

ClientSocket::ClientSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor),userID(-1),lastsize(0)
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
    data.operater = 0;
    data.socketID = sock->getSocketID();
    data.userID = this->userID;
    data.data = sock->readAll();
    QByteArray buf;
    {
        QDataStream stream(&buf,QIODevice::ReadWrite);
        stream << data;
    }
    qulonglong size = buf.size();
    QByteArray ba = QByteArray::number(size,16);
    while(ba.size() < 6)
        ba.insert(0,static_cast<char>(0X00));
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
    QPair<QString,qint16> host;
    {
        QDataStream stream(&data.data,QIODevice::ReadWrite);
        stream >> host;
    }
    qDebug() << host.first << host.second;
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
    if (sock != nullptr)
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
    QByteArray buf;
    QPair<QString,QString> host;
    {
        QDataStream stream(&buf,QIODevice::ReadWrite);
        stream >> host;
    }
    data.userID = 1000;
    data.data = "123456";
    buf.clear();
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
