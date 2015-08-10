#include "socket5server.h"
#include "localsocket.h"

Socket5Server::Socket5Server(QObject *parent) : QObject(parent),
    server_(new QAsioTcpServer(4096,this)),socket_(new QAsioTcpsocket(4096,this))
{

}

