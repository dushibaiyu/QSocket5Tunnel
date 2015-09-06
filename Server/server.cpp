#include "server.h"

Server::Server(QObject *parent) : QObject(parent),server_(new QAsioTcpServer(4096,this))
{
    connect(server_,&QAsioTcpServer::newConnection,[&](QAsioTcpsocket * socket){
        auto nsc = new ClientSocket(socket);
        nsc->moveToThread(this->thread());
        connect(nsc,&ClientSocket::socketDis,this,&Server::removeClientd);
        connect(nsc,&ClientSocket::getKey,this,&Server::getKey);
        emit this->newClient(nsc);
        qDebug() << "new Connection : " << socket->socketDescriptor();
    });
    connect(this,&Server::newClient,this,&Server::newClientd);
}

Server::~Server()
{
    server_->close();
    qDeleteAll(clients.begin(),clients.end());
}

void Server::newClientd(ClientSocket * cl)
{
    clients.append(cl);
}

void Server::removeClientd(ClientSocket *cl)
{
    clients.removeAll(cl);
    cl->deleteLater();
}

void Server::getKey(ClientSocket * cl,const QByteArray & /*userData*/)
{
    cl->haveKey(QByteArray("dushibaiyu_KEY"));
    qDebug () << "Server::getKey : rev Kry = dushibaiyu_KEY";
}
