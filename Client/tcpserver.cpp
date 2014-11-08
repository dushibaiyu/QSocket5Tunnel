#include "tcpserver.h"
#include <QPair>
#include <QtEndian>
#include <QHostAddress>

TcpServer::TcpServer(QString serHost,qint16 serPort,qint16 localBind,QObject *parent) :
    QTcpServer(parent),serHost(serHost),serPort(serPort),localBind(localBind),lastsize(0)
{
     tcpClient = new  QHash<int,LocalSocket *>;
     serverSocket = new QTcpSocket(this);
     isSerCon = false;
     connect(serverSocket,&QTcpSocket::readyRead,this,&TcpServer::serSocketRead);
     connect(serverSocket,&QTcpSocket::disconnected,this,&TcpServer::serSocketDisCon);
}

TcpServer::~TcpServer()
{
    delete tcpClient;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    //qDebug() << socketDescriptor << "   socketDescriptor";
    LocalSocket * tcpTemp = new LocalSocket(socketDescriptor,this);
    QString thisHost;
    qint16 thisPort;
    if (isSerCon)
    {
        initLocalProxy(thisHost,thisPort,tcpTemp);
        connect(tcpTemp,&LocalSocket::readyRead,this,&TcpServer::LocalSocketRead);
        connect(tcpTemp,&LocalSocket::disconnected,this,&TcpServer::localSockedDisCon);
        swapData data;
        data.operater = 1;
        data.socketID = socketDescriptor;
        data.userID = this->userID;
        QByteArray buf;
        {
            QPair<QString,qint16> host(thisHost,thisPort);
            QDataStream stream(&buf,QIODevice::ReadWrite);
            stream << host;
            //qDebug() << thisHost << thisPort;
        }
        data.data = buf;
        buf.clear();
        {
            QDataStream stream(&buf,QIODevice::ReadWrite);
            stream << data;
        }
        qlonglong ize = buf.size();
        QByteArray ba = QByteArray::number(ize,16);
        while(ba.size() < 6)
            ba.insert(0,'0');
        ba += buf;
        serverSocket->write(ba);
        tcpClient->insert(socketDescriptor,tcpTemp);
    }
    else
    {
        tcpTemp->close();
        tcpTemp->deleteLater();
        serverSocket->connectToHost(serHost,serPort);
        if (serverSocket->waitForConnected())
        {
            isSerCon = true;
        }
        return;
    }
}

void TcpServer::initLocalProxy(QString & thisHost,qint16 & thisPort,LocalSocket * sock)
{
    QByteArray generalError;
    generalError.append((char)0x05); // SOCKS5
    generalError.append((char)0x01); // general error

    sock->waitForReadyRead();
    QByteArray ba = sock->read(2);
    // 1st byte - SOCKS version
    // 2nd byte - Auth method count

    int authCnt = (int)ba.at(1);

    // we only serve SOCKS5
    if(ba.at(0)!=5) {
        sock->close();
        sock->deleteLater();
        return;
    }
    while(sock->bytesAvailable() < authCnt)
        sock->waitForReadyRead();
    // ignore auth
    sock->read(authCnt);
    QByteArray response;
    response.append((char)0x05); // SOCKS version
    response.append((char)0x00); // requested auth method (0x00 means no auth)
    sock->write(response);
    sock->waitForBytesWritten();
    while(sock->bytesAvailable() < 4)
        sock->waitForReadyRead();
    ba = sock->read(4);
    // 1st byte - SOCKS version
    // 2nd byte - command (1 = open TCP stream)
    // 3rd byte - 0x00
    // 4th byte - address type (1 = IPv4, 3 = QDN, 4 = IPv6)
    if(ba.at(0)!=5) {
        sock->write(generalError);
        sock->waitForBytesWritten();
        sock->close();
        sock->deleteLater();
        return;
    } else if(ba.at(1)!=1) {
        // only handle "open stream" requests
        sock->write(generalError);
        sock->waitForBytesWritten();
        sock->close();
        sock->deleteLater();
        return;
    } else if(ba.at(2)!=0) {
        sock->write(generalError);
        sock->waitForBytesWritten();
        sock->close();
        sock->deleteLater();
        return;
    }
    // reuse old response object containing 0x05, 0x00
    // 1st byte = SOCKS version
    // 2nd byte = status (0x00 = success)
    // 3rd byte = 0x00
    // 4th and following bytes - repeat what the client sent, not sure that's correct,
    //                                                        it should contain the outgoing host:port pair
    response.append((char)0x00);
    response.append(ba.at(3));
    switch(ba.at(3)) {
    // IPv4
    case 1: {
        while(sock->bytesAvailable() < 4)
            sock->waitForReadyRead();
        ba = sock->read(4);
        QHostAddress addr(qFromBigEndian<quint32>((uchar*)ba.constData()));
        thisHost = addr.toString();
        response.append(ba);
    };
        break;
        // domain name
    case 3: {
        while(sock->bytesAvailable() < 1)
            sock->waitForReadyRead();
        ba = sock->read(1);
        int len = ba.at(0);
        response.append(ba);
        while(sock->bytesAvailable() < len)
            sock->waitForReadyRead();
        ba = sock->read(len);
        thisHost = ba;
        response.append(ba);
    };
        break;
    case 4:{
        while(sock->bytesAvailable() < 16)
            sock->waitForReadyRead();
        ba = sock->read(16);
        QHostAddress addr(ba.constData());
        thisHost = addr.toString();
        response.append(ba);
    };
        break;
    default: {
        // deny IPv6 (yes, I'm lazy, it's just a few lines of code) and incorrect values
        sock->write(generalError);
        sock->waitForBytesWritten();
        sock->close();
        sock->deleteLater();
        return;
    }
    }
    while(sock->bytesAvailable() < 2)
        sock->waitForReadyRead();
    ba = sock->read(2);
    // port in network order
    thisPort = qFromBigEndian<qint16>((uchar*)ba.constData());
    response.append(ba);
    sock->write(response); // notify success and proceed with remote connection
}

void TcpServer::socketConnect(const QString &user, const QString &pass)
{
    serverSocket->connectToHost(serHost,serPort);
    if (serverSocket->waitForConnected())
    {
        isSerCon = true;
        swapData data;
        data.operater = 3;
        QByteArray buf;
        {
            QPair<QString,QString> host(user,pass);
            QDataStream stream(&buf,QIODevice::ReadWrite);
            stream << host;
        }
        data.data = buf;
        buf.clear();
        {
            QDataStream stream(&buf,QIODevice::ReadWrite);
            stream << data;
        }
        qlonglong ize = buf.size();
        QByteArray ba = QByteArray::number(ize,16);
        while(ba.size() < 6)
            ba.insert(0,'0');
        ba += buf;
        serverSocket->write(ba);
    }
    else
    {
        isSerCon = false;
        emit notToServer();
    }
}

void TcpServer::serSocketRead()
{
    QByteArray ba;
    if (lastsize == 0)
    {
        ba = serverSocket->read(6);
        lastsize = ba.toLongLong(0,16);
    }
    while (serverSocket->bytesAvailable() >= static_cast<qint64>(lastsize))
    {
        swapData data;
        ba = serverSocket->read(this->lastsize);
        {
            QDataStream stream(&ba,QIODevice::ReadWrite);
            stream >> data;
        }
        switch (data.operater)
        {
        case 0:
            handleSwapData(data);
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
        if (!serverSocket->atEnd() || serverSocket->bytesAvailable() > 6)
        {
            ba = serverSocket->read(6);
            lastsize = ba.toLongLong(0,16);
        }
        else
        {
            lastsize = 0 ;
            return ;
        }
    }
}

void TcpServer::serSocketDisCon()
{
    isSerCon = false;
    lastsize = 0;
    for (auto it = tcpClient->begin(); it != tcpClient->end(); ++it)
    {
        it.value()->disconnectFromHost();
        delete it.value();
    }
    tcpClient->clear();
}

void TcpServer::LocalSocketRead()
{
    LocalSocket * sock = qobject_cast<LocalSocket *>(QObject::sender());
    Q_ASSERT(sock);
    swapData data;
    data.operater = 0;
    data.socketID = sock->getSocketID();
    data.userID = this->userID;
    data.data = sock->readAll();
    QByteArray  buf;
    {
        QDataStream stream(&buf,QIODevice::ReadWrite);
        stream << data;
    }
    qulonglong size = buf.size();
    QByteArray ba = QByteArray::number(size,16);
    while(ba.size() < 6)
        ba.insert(0,'0');
    ba += buf;
    serverSocket->write(ba);
}

void TcpServer::localSockedDisCon()
{
    LocalSocket * sock = qobject_cast<LocalSocket *>(QObject::sender());
    Q_ASSERT(sock);
    if (tcpClient->contains(sock->getSocketID()))
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
        serverSocket->write(ba);
        tcpClient->remove(sock->getSocketID());
    }
    delete sock;
}

void TcpServer::handleDisCon(swapData &data)
{
    LocalSocket * sock = tcpClient->value(data.socketID,nullptr);
    if (sock != nullptr)
    {
        tcpClient->remove(data.socketID);
        sock->disconnectFromHost();
    }
}

void TcpServer::handleSwapData(swapData &data)
{
    LocalSocket * sock = tcpClient->value(data.socketID,nullptr);
    //qDebug() << "//qDebug() << data.socketID;" << data.socketID << sock;
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
        serverSocket->write(ba);
    }
}

void TcpServer::handleUserLog(swapData &data)
{
    this->userID = data.userID;
    this->tocken = data.data;
    if(!this->listen(QHostAddress::Any,localBind))
    {
        emit listenState(false);
    }
    else
    {
        emit listenState(true);
    }
}
