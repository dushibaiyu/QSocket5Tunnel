#include "socket5server.h"
#include "localsocket.h"

Socket5Server::Socket5Server(QObject *parent) : QObject(parent),
    server_(new QAsioTcpServer(4096,this)),socket_(new QAsioTcpsocket(4096,this))
{
    connect(server_,&QAsioTcpServer::newConnection,this,&Socket5Server::newSocket,Qt::DirectConnection);
//    connect(socket_,&QAsioTcpsocket::)
}

Socket5Server::~Socket5Server()
{
    socket_->disconnectFromHost();
    delete server_;
    delete socket_;
}

void Socket5Server::newSocket(QAsioTcpsocket * socket)
{
    new LocalSocket(socket,this);
}
