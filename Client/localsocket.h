#ifndef LocalSocket_H
#define LocalSocket_H

#include <qasiotcpsocket.h>

class LocalSocket: public QObject
{
    Q_OBJECT
public:
    explicit LocalSocket(QAsioTcpsocket * loc,QObject *parent = 0):
        QObject(parent), local(loc)
    {
        id = local->socketDescriptor();
        connect(local,&QAsioTcpsocket::disConnected,this,&LocalSocket::disConnected,Qt::DirectConnection);
        connect(local,&QAsioTcpsocket::sentReadData,this,&LocalSocket::readData,Qt::DirectConnection);
        local->do_start();
    }

    inline void write(const QByteArray &  data) {local->write(data);}
    inline void write(const char * data, int size = -1) {local->write(data,size);}

    inline void close() {local->disconnectFromHost();}

    inline int getID() {return id;}

signals:
    void disConnect(int id);
protected slots:
    void readData(const QByteArray & data);

    void disConnected();
private:
    QAsioTcpsocket * local;
    int id;

    QAsioTcpsocket * server;
};

#endif // LocalSocket_H

void LocalSocket::readData(const QByteArray & data)
{}
