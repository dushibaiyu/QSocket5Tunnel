#ifndef LocalSocket_H
#define LocalSocket_H

#include <qasiotcpsocket.h>
#include "socket5server.h"
#include <private/qipaddress_p.h>
#include <QBuffer>

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
        connect(&socket,&QAsioTcpsocket::connected,[&](){this->linkSuess();});
        connect(&socket,&QAsioTcpsocket::disConnected,[&](){ close();
            this->deleteLater();});
        local->do_start();
    }

    ~LocalSocket() {
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

void LocalSocket::readData(const QByteArray & data)
{

    switch (state_) {
    case NeedCheck:
    {
        if(data.at(0)!= 5) {//只允许Socket5
            close();
            deleteLater();
            //            return;
        } else {
            char rep[2] = {0X05,0X00};
            local->write(rep,2);
            state_ = GetLink;
        }
    }
        break;
    case GetLink:
    {
        QByteArray * bufdata = const_cast<QByteArray *>(&data);
        QBuffer buf(bufdata);
        buf.open(QBuffer::ReadOnly);
        if (buf.bytesAvailable() < 6) {
            close();
            deleteLater();
            return;
        }
        repData.append((char)0x05); // SOCKS version
        repData.append((char)0x00);
        QByteArray ba = buf.read(4);
        if(ba.at(0)!= 5 || ba.at(1)!=1 || ba.at(2)!=0) {// only handle "open stream" requests
            close();
            deleteLater();
            return;
        }
        repData.append((char)0x00);
        repData.append(ba.at(3));
        QString host;
        switch(ba.at(3)) {
        // IPv4
        case 1: {
            if (buf.bytesAvailable() < 4) {
                close();
                deleteLater();
                return;
            }
            ba = buf.read(4);
            QIPAddressUtils::toString(host, uqFromBigEndian<quint32>((uchar*)ba.constData()));
            repData.append(ba);
        }
            break;
            // domain name
        case 3: {
            ba = buf.read(1);
            int len = ba.at(0);
            if (buf.bytesAvailable() < len) {
                close();
                deleteLater();
                return;
            }
            repData.append(ba);
            ba = buf.read(len);
            host = ba;
            repData.append(ba);
        }
            break;
        case 4:{
            if (buf.bytesAvailable() < 16) {
                close();
                deleteLater();
                return;
            }
            ba = buf.read(16);
            QIPAddressUtils::toString(host,ba.constData());
            repData.append(ba);
        }
            break;
        default: {
            close();
            deleteLater();
            return;
        }
        }

        if (buf.bytesAvailable() < 2) {
            close();
            deleteLater();
            return;
        }
        host.append("::");

        ba = buf.read(2);
        qint16 port = qFromBigEndian<qint16>((uchar*)ba.constData());

        socket.connectToHost(host,port);

        host.append(host.number(port));
        repData.append(ba);
//        server->addNewClient(id,this,host);

        state_ = Done;
    }
        break;
    default:
    {
        socket.write(data);
    }
        break;
    }
}




#endif // LocalSocket_H
