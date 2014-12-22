#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "tcpserver.h"
#include <QPair>
#include <QtEndian>
#include <QHostAddress>
#include <QTime>

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent),userID(-1),lastsize(0),aes(nullptr)
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
    if (aes != nullptr)
        delete aes;

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    LocalSocket * tcpTemp = new LocalSocket(socketDescriptor,this);
    QString thisHost;
    qint16 thisPort;
    if (isSerCon)
    {
        initLocalProxy(thisHost,thisPort,tcpTemp);
        connect(tcpTemp,&LocalSocket::readyRead,this,&TcpServer::LocalSocketRead);
        connect(tcpTemp,&LocalSocket::disconnected,this,&TcpServer::localSockedDisCon);
        data.operater = 1;
        data.socketID = socketDescriptor;
        data.userID = this->userID;
        newHost = qMakePair(thisHost,thisPort);
        if (!serializeData(bytearry,newHost)) return ;
        data.data = encryptData(aes,bytearry);
        if (sentServerData())
            tcpClient->insert(socketDescriptor,tcpTemp);
    }
    else
    {
        tcpTemp->close();
        tcpTemp->deleteLater();
        serverSocket->connectToHost(ConfigClass::getClass().serverUrl,ConfigClass::getClass().serverPort);
        if (serverSocket->waitForConnected())
        {
            isSerCon = true;
        }
        return;
    }
}

void TcpServer::serSocketRead()
{
//    QTime tm;
//    tm.start();
    if (lastsize == 0)
    {
        if (serverSocket->bytesAvailable() < 6) return;
        basize = serverSocket->read(6);
        lastsize = basize.toLongLong(0,16);
    }
    while (serverSocket->bytesAvailable() >= static_cast<qint64>(lastsize))
    {
        bytearry = serverSocket->read(lastsize);
        if(deSerializeData(bytearry,data))
        {
            switch (data.operater)
            {
            case 0:
                handleSwapData();
                break;
            case 2:
                handleDisCon();
                break;
            case 3:
                handleUserLog();
                break;
            default:
                break;
            }
        }
        if (!serverSocket->atEnd() && serverSocket->bytesAvailable() > 6)
        {
            basize = serverSocket->read(6);
            lastsize = basize.toLongLong(0,16);
        }
        else
        {
            lastsize = 0 ;
//            qDebug() << "lastsize = 0 " << tm.elapsed();
            return ;
        }
    }
//    qDebug() << "serSocketRead " << tm.elapsed() << " " << lastsize;
}




inline bool TcpServer::sentServerData()
{
    if (!serializeData(bytearry,data)) return false;
    qulonglong size = bytearry.size();
    basize = QByteArray::number(size,16);
    while(basize.size() < 6)
        basize.insert(0,'0');
    serverSocket->write(basize + bytearry);
    return true;
}

void TcpServer::handleUserLog()
{
    if (data.userID == -1 || data.data.isEmpty())
    {
        emit userErro();
        return;
    }
    this->userID = data.userID;
    this->tocken = QString::fromUtf8(data.data);
    if (aes != nullptr)
        delete aes;
    aes = new OpensslAES(data.data);
    if(!this->listen(QHostAddress::Any,ConfigClass::getClass().localPort))
    {
        emit listenState(false);
    }
    else
    {
        emit listenState(true);
    }
}

