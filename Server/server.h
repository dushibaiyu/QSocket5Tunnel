#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "qasiotcpserver.h"
#include "clientsocket.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

    bool Listen(const QString & ip,qint16 port = 8888){
        if (ip.isEmpty())
            return server_->listen(port);
        else
            return server_->listen(ip,port);
    }

signals:
    void newClient(ClientSocket * cl);
public slots:

protected slots:

    void newClientd(ClientSocket * cl);
    void removeClientd(ClientSocket * cl);
private:
    QAsioTcpServer * server_;
    QList<ClientSocket*> clients;
};

#endif // SERVER_H
