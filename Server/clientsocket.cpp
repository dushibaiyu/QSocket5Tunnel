#include "clientsocket.h"
//#include <QHostAddress>
#include <QThread>
#include "userconfig.h"

ClientSocket::ClientSocket(asio::ip::tcp::socket * socket, QObject *parent) :
    QAsioTcpSocket(socket,4096,parent),userID(-1),lastsize(0),aes(nullptr)
{
    socketID = socketDescriptor();
    connect(this,&ClientSocket::readReadly,this,&ClientSocket::clientData);
    connect(this,&ClientSocket::disconnected, [&](){
        emit sentDiscon(QThread::currentThread(),socketID);//发送断开连接的用户信息
    });
}

ClientSocket::~ClientSocket()
{
    if (aes != nullptr)
        delete aes;
    if (!socketList.isEmpty()) {
        qDeleteAll(socketList.begin(),socketList.end());
    }
    socketList.clear();
}

void ClientSocket::clientData()
{
    if (lastsize == 0) {
        if (this->bytesAvailable() < 6) return;
        basize = this->read(6);
        lastsize = basize.toLongLong(0,16);
    }
    while (this->bytesAvailable() >= static_cast<qint64>(lastsize)) {
        bytearry = this->read(lastsize);
        if(deSerializeData(bytearry,data)) {
            switch (data.operater) {
            case 0:
                handleSwapData();
                break;
            case 1:
                handleNewCon();
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
        if (!this->atEnd() && this->bytesAvailable() > 6) {
            basize = this->read(6);
            lastsize = basize.toLongLong(0,16);
        } else {
            lastsize = 0 ;
            return ;
        }
    }
}

void ClientSocket::remoteData()
{
    RemoteSocket * sock = qobject_cast<RemoteSocket *>(QObject::sender());
    Q_ASSERT(sock);
    data.operater = 0;
    data.socketID = sock->getSocketID();
    data.userID = this->userID;
    data.data = encryptData(aes,sock->readAll());
    sentClientData();
}

void ClientSocket::sentRemoteDisCon(int socketId)
{
    data.operater = 2;
    data.socketID = socketId;
    data.userID = this->userID;
    data.data.clear();
    sentClientData();
}

void ClientSocket::remoteDisCon()
{
    RemoteSocket * sock = qobject_cast<RemoteSocket *>(QObject::sender());
    Q_ASSERT(sock);
    if (socketList.contains(sock->getSocketID())) {
        socketList.remove(sock->getSocketID());
        sentRemoteDisCon(sock->getSocketID());
    }
    sock->deleteLater();
}

void ClientSocket::handleNewCon()
{
    if (!decryptClientData(data)) return;
    if (!deSerializeData(data.data,newHost)) return;
    if (newHost.first.isEmpty()) return;
    RemoteSocket * sock = new RemoteSocket(data.socketID,this);
    sock->connectToHost(newHost.first,newHost.second);
    connect(sock,&RemoteSocket::readReadly,this,&ClientSocket::remoteData);
    connect(sock,&RemoteSocket::disconnected,this,&ClientSocket::remoteDisCon);
    socketList.insert(data.socketID,sock);
}

void ClientSocket::handleDisCon()
{
    RemoteSocket * sock = socketList.value(data.socketID,nullptr);
    if (sock != nullptr) {
        socketList.remove(data.socketID);
        if(sock->state() == QAsioTcpSocket::ConnectedState) {
            sock->disconnectFromHost();
        } else {
            sock->deleteLater();
        }
    }
}

void ClientSocket::handleSwapData()
{
    RemoteSocket * sock = socketList.value(data.socketID,nullptr);
    if (sock != nullptr && decryptClientData(data)) {
        sock->Write(data.data);
    } else {
        data.operater = 2;
        data.data.clear();
        sentClientData();
    }
}

inline  bool ClientSocket::sentClientData()
{
    if (!serializeData(bytearry,data)) return false;
    qulonglong size = bytearry.size();
    basize = QByteArray::number(size,16);
    while(basize.size() < 6)
        basize.insert(0,'0');
    this->write(basize + bytearry);
    return true;
}

void ClientSocket::handleUserLog()
{
    QPair<QString,QString> user;
    if (!deSerializeData(data.data,user) || user.first.isEmpty() ||
            user.second.isEmpty()) {
        data.userID = -1;
        data.data.clear();
    } else {
        data.userID = UserConfig::getClass().getUserId(user.first,user.second,token);
        if (data.userID > 0 && (!token.isEmpty())) {
            this->userID = data.userID;
            data.data = token.toUtf8();
            if (aes != nullptr)
                delete aes;
            aes = new OpensslAES(data.data);
        } else {
            data.data.clear();
        }
    }
    sentClientData();
}

bool ClientSocket::decryptClientData(swapData &data)
{
    if (userID ==- 1) {
        if (data.userID <= 0) {
            return false;
        }
        token =UserConfig::getClass().getToken(data.userID);
        if (token.isEmpty()) {
            userID = -1;
            return false;
        }
        if (aes != nullptr)
            delete aes;
        aes = new OpensslAES(token.toUtf8());
    }
    return decryptData(aes,data.data);
}

