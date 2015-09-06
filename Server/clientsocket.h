#ifndef ClientSocket_H
#define ClientSocket_H

#include "qasiotcpsocket.h"
#include "../common/datastruct.h"
#include <QMap>
#include <QBuffer>
#include <QReadWriteLock>

class RemoteSocket;

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QAsioTcpsocket * soc,QObject * parent = 0);
    ~ClientSocket();

    const QAesWrap & getAes() const {return *aes;}

    inline void write(const QByteArray & data){
        revSize += (data.size() - 9);
        socket_->write(data);
    }


    inline void removeConnet(int id) {
        lock.lockForWrite();
        socketList.remove(id);
        lock.unlock();
    }

    inline RemoteSocket * getRemote(int id){
        lock.lockForRead();
        auto tp = socketList.value(id,nullptr);
        lock.unlock();
        return tp;
    }

    inline void newLink(bool islink,int id) {
        qDebug() << "newLink  islink = " << islink;
        QByteArray data;
        if (islink) {
            data.setNum(1);
        } else {
            data.setNum(0);
            removeConnet(id);
        }
        socket_->write(serializeData(getAes(),NewLink,id,data));
    }

    quint64 GetSize() const {return getSize;}
    quint64 RevSize() const {return revSize;}
signals:
    void socketDis(ClientSocket * client);
    void getKey(ClientSocket * client,const QByteArray & revData);

public slots:
    void haveKey(const QByteArray & key);
protected slots:
    void readData(const QByteArray & data);

    void clear();
private:
    QReadWriteLock lock;
    QMap<int,RemoteSocket *> socketList;
    QString UserName;
    uint lastSize;
    QAesWrap * aes;
    QBuffer buffer;

    bool isKey;
    QAsioTcpsocket * socket_;
    quint64 getSize,revSize;

    Q_DISABLE_COPY(ClientSocket)
};

#endif // ClientSocket_H
