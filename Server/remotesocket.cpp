#include "remotesocket.h"

RemoteSocket::RemoteSocket(int id,QObject *parent) :
    QTcpSocket(parent),socketId(id)
{
    connect(this,&RemoteSocket::disconnected,[&](){emit disconnectedID(this->socketId);});
}
