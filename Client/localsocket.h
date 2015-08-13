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
        connect(local,&QAsioTcpsocket::disConnected,[&](){emit disConnect(id);socket.disconnectFromHost();deleteLater();});
        connect(local,&QAsioTcpsocket::sentReadData,this,&LocalSocket::readData,Qt::DirectConnection);
        connect(&socket,&QAsioTcpsocket::connected,[&](){this->linkSuess();qDebug() << " &socket,&QAsioTcpsocket::connected" ;});
        connect(&socket,&QAsioTcpsocket::disConnected,[&](){ close();
            this->deleteLater();
        });
        connect(&socket,&QAsioTcpsocket::sentReadData,[&](const QByteArray & data){
            qDebug() << " &socket,&QAsioTcpsocket::sendData" ;
            local->write(data);
        });
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

    inline void linkSuess() {
        local->write(repData);
    }

signals:
    void disConnect(int id);
protected slots:
    void readData(const QByteArray & data);
private:
    QAsioTcpsocket * local;
    QAsioTcpsocket socket;
    int id;
    State state_;
    Socket5Server * server;
    QByteArray repData;
};

#endif // LocalSocket_H
