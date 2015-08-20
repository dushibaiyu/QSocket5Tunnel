#ifndef LocalSocket_H
#define LocalSocket_H

#include <qasiotcpsocket.h>
#include "socket5server.h"
#include <private/qipaddress_p.h>
#include <QBuffer>
#include <QtEndian>
#include <QDebug>

class LocalSocket: public QObject
{
    Q_OBJECT
public:
    enum State {
        NeedCheck,
        GetLink,
        Done
    };

    explicit LocalSocket(QAsioTcpsocket * loc,Socket5Server * server_,QObject *parent = 0):
        QObject(parent), local(loc),state_(NeedCheck),server(server_)
    {
        id = local->socketDescriptor();
        connect(local,&QAsioTcpsocket::disConnected,[&](){
            if (server->getLocal(id) != nullptr) {
                server->write(serializeData(server->getAes(),DisLink,id,QByteArray()));
                server->removeConnet(id);
            }
            deleteLater();
        });
        connect(local,&QAsioTcpsocket::sentReadData,this,&LocalSocket::readData,Qt::DirectConnection);
        local->do_start();
    }

    ~LocalSocket()
    {
        delete local;
    }

    inline void write(const QByteArray &  data) {local->write(data);}
    inline void write(const char * data, int size = -1) {local->write(data,size);}

    inline void close() {local->disconnectFromHost();}

    inline int getID() {return id;}

    inline void linkSuess(bool islink) {
        if (!islink) repData[2] = 0X04;
        local->write(repData);
    }

signals:
    void disConnect(int id);
protected slots:
    void readData(const QByteArray & data);
private:
    QAsioTcpsocket * local;
    int id;
    State state_;
    Socket5Server * server;
    QByteArray repData;
};

#endif // LocalSocket_H
