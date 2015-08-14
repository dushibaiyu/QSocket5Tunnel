#include "server.h"

Server::Server(QObject *parent) : QObject(parent),server_(new QAsioTcpServer(4096,this))
{
    connect(server_,&QAsioTcpServer::newConnection,[&](QAsioTcpsocket * socket){
        auto nsc = new ClientSocket(socket);
        nsc->moveToThread(this->thread());
        connect(nsc,&ClientSocket::socketDis,this,&Server::removeClientd);
        emit this->newClient(nsc);
    });
    connect(this,&Server::newClient,this,&Server::newClientd);
}

Server::~Server()
{
    server_->close();
    qDeleteAll(clients.begin(),clients.end());
}

void Server::newClient(ClientSocket * cl)
{
    clients.append(cl);
}

void Server::removeClientd(ClientSocket *cl)
{
    clients.removeAll(cl);
    cl->deleteLater();
}
