#ifndef SOCKET5SERVER_H
#define SOCKET5SERVER_H

#include <QObject>
#include <qasiotcpserver.h>

class LocalSocket;

class Socket5Server : public QObject
{
    Q_OBJECT
public:
    explicit Socket5Server(QObject *parent = 0);
    ~Socket5Server();

    void addNewClient(int id,LocalSocket * sock,const QString & host){}

signals:

public slots:
    bool Listen(const QString & ip,qint16 port = 6666){
        if (ip.isEmpty())
            return server_->listen(port);
        else
            return server_->listen(ip,port);
    }

protected:
    void newSocket(QAsioTcpsocket * socket);
//    void readData(const QByteArray & data);
private:
    QAsioTcpServer * server_;
    QAsioTcpsocket * socket_;
};

#endif // SOCKET5SERVER_H
