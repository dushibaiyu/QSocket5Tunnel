#ifndef REMOTESOCKET_H
#define REMOTESOCKET_H

#include "clientsocket.h"

class RemoteSocket : public QObject
{
    Q_OBJECT
public:
    explicit RemoteSocket(ClientSocket * soc,int id,QObject *parent = 0): QObject(parent),
        socket_(new QAsioTcpsocket(4096)),remote(soc),socketId(id),isLinked(false)
    {
        connect(socket_,&QAsioTcpsocket::sentReadData,[&](const QByteArray & data){
            remote->write(serializeData(remote->getAes(),SwapData,socketId,data));
        });
        connect(socket_,&QAsioTcpsocket::connected,[&](){
            isLinked = true;
            remote->newLink(true,id);});
        connect(socket_,&QAsioTcpsocket::disConnected,[&](){
            if (!isLinked) remote->newLink(false,id);
            if (remote->removeConnet(id) != nullptr )
                remote->write(serializeData(remote->getAes(),DisLink,id,QString()));
            deleteLater();
        });
    }
    ~RemoteSocket(){
        delete socket_;
    }
    int getSocketID() const
    {
        return this->socketId;
    }

    inline void write(const QByteArray & data) {
        socket_->write(data);
    }

    inline void connectTo(const QString & host, qint16 port) {
        socket_->connectToHost(host,port);
    }
    inline void disCon() {
        socket_->disconnectFromHost();
    }
private:
    QAsioTcpsocket * socket_;
    ClientSocket * remote;
    int socketId;
    bool isLinked;
};

#endif // REMOTESOCKET_H
