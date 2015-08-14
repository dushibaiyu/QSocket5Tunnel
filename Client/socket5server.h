#ifndef SOCKET5SERVER_H
#define SOCKET5SERVER_H

#include <QObject>
#include <qasiotcpserver.h>
#include <QMap>
#include <QReadWriteLock>
#include <qaeswrap.h>
#include <QBuffer>
#include "../common/datastruct.h"

class LocalSocket;

class Socket5Server : public QObject
{
    Q_OBJECT
public:
    explicit Socket5Server(QObject *parent = 0);
    ~Socket5Server();

signals:
    void socketDisconnet();
public slots:
    bool Listen(const QString & ip,qint16 port = 6666){
        if (ip.isEmpty())
            return server_->listen(port);
        else
            return server_->listen(ip,port);
    }

    inline void newConnet(const QString & host,int id,LocalSocket * socket) {
        lock.lockForWrite();
        clients.insert(id,socket);
        lock.unlock();
        socket_->write(serializeData(getAes(),NewLink,id,host.toUtf8()));
    }

    inline LocalSocket * removeConnet(int id) {
            lock.lockForRead();
            auto tp = clients.value(id,nullptr);
            lock.unlock();
            if (tp != nullptr) {
                lock.lockForWrite();
                clients.remove(id);
                lock.unlock();
            }
            return tp;
    }

    inline void write(const QByteArray & data) {
        socket_->write(data);
    }


    inline const QAesWrap & getAes() const {return *aes;}
protected slots:
    void newSocket(QAsioTcpsocket * socket);
    void readData(const QByteArray & data);
    void socketDis();
private:
    QAsioTcpServer * server_;
    QAsioTcpsocket * socket_;
    QMap<int,LocalSocket *> clients;
    QReadWriteLock lock;
    QAesWrap * aes;
    bool isHaveKey;
    QBuffer buffer;
    uint lastSize;
};

#endif // SOCKET5SERVER_H
