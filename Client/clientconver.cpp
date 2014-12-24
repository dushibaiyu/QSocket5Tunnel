#include "clientconver.h"
#include <QTcpSocket>
#include <QtEndian>
#include <QHostAddress>
#include "../common/datastruct.h"
#include "configclass.h"

ClientConver::ClientConver(OpensslAES *aes, QObject *parent)
    : Conversation(aes,parent)
{

}

ClientConver::~ClientConver()
{

}

bool ClientConver::initConSocket(qint16 socket)
{
    QString thisHost;
    qint16 thisPort = 0;
    socket1->setSocketDescriptor(socket);
    if (initLocalProxy(thisHost,thisPort,socket1)) {
        socket2->connectToHost(ConfigClass::getClass().serverUrl,ConfigClass::getClass().serverPort);
        if (socket2->waitForConnected(5000)) {
            QByteArray data(thisHost.toUtf8());
            data += "\n";
            data += QByteArray::number(thisPort);
            data = encryptData(aes,data);
            int size = data.size();
            QByteArray basize = QByteArray::number(size,16);
            while(basize.size() < 4)
                basize.insert(0,'0');
            socket2->write(basize + data);
            return true;
        }
    }
    return false;
}

bool ClientConver::initLocalProxy(QString &thisHost, qint16 &thisPort, QTcpSocket *sock)
{
    QByteArray generalError;
    generalError.append((char)0x05); // SOCKS5
    generalError.append((char)0x01); // general error
    if (sock->bytesAvailable() < 2)
        sock->waitForReadyRead();
    QByteArray ba = sock->read(2);
    // 1st byte - SOCKS version
    // 2nd byte - Auth method count

    int authCnt = (int)ba.at(1);

    // we only serve SOCKS5
    if(ba.at(0)!=5) {
        sock->close();
        sock->deleteLater();
        return false;
    }
    while(sock->bytesAvailable() < authCnt)
        sock->waitForReadyRead();
    // ignore auth
    sock->read(authCnt);
    QByteArray response;
    response.append((char)0x05); // SOCKS version
    response.append((char)0x00); // requested auth method (0x00 means no auth)
    sock->write(response);
    sock->waitForBytesWritten();
    while(sock->bytesAvailable() < 4)
        sock->waitForReadyRead();
    ba = sock->read(4);
    // 1st byte - SOCKS version
    // 2nd byte - command (1 = open TCP stream)
    // 3rd byte - 0x00
    // 4th byte - address type (1 = IPv4, 3 = QDN, 4 = IPv6)
    if(ba.at(0)!=5) {
        sock->write(generalError);
        sock->waitForBytesWritten();
        sock->close();
//        sock->deleteLater();
        return false;
    } else if(ba.at(1)!=1) {
        // only handle "open stream" requests
        sock->write(generalError);
        sock->waitForBytesWritten();
        sock->close();
//        sock->deleteLater();
        return false;
    } else if(ba.at(2)!=0) {
        sock->write(generalError);
        sock->waitForBytesWritten();
        sock->close();
//        sock->deleteLater();
        return false;
    }
    // reuse old response object containing 0x05, 0x00
    // 1st byte = SOCKS version
    // 2nd byte = status (0x00 = success)
    // 3rd byte = 0x00
    // 4th and following bytes - repeat what the client sent, not sure that's correct,
    //                                                        it should contain the outgoing host:port pair
    response.append((char)0x00);
    response.append(ba.at(3));
    switch(ba.at(3)) {
    // IPv4
    case 1: {
        while(sock->bytesAvailable() < 4)
            sock->waitForReadyRead();
        ba = sock->read(4);
        QHostAddress addr(qFromBigEndian<quint32>((uchar*)ba.constData()));
        thisHost = addr.toString();
        response.append(ba);
    };
        break;
        // domain name
    case 3: {
        while(sock->bytesAvailable() < 1)
            sock->waitForReadyRead();
        ba = sock->read(1);
        int len = ba.at(0);
        response.append(ba);
        while(sock->bytesAvailable() < len)
            sock->waitForReadyRead();
        ba = sock->read(len);
        thisHost = ba;
        response.append(ba);
    };
        break;
    case 4:{
        while(sock->bytesAvailable() < 16)
            sock->waitForReadyRead();
        ba = sock->read(16);
        QHostAddress addr(ba.constData());
        thisHost = addr.toString();
        response.append(ba);
    };
        break;
    default: {
        // deny IPv6 (yes, I'm lazy, it's just a few lines of code) and incorrect values
        sock->write(generalError);
        sock->waitForBytesWritten();
        sock->close();
//        sock->deleteLater();
        return false;
    }
    }
    while(sock->bytesAvailable() < 2)
        sock->waitForReadyRead();
    ba = sock->read(2);
    // port in network order
    thisPort = qFromBigEndian<qint16>((uchar*)ba.constData());
    response.append(ba);
    sock->write(response); // notify success and proceed with remote connection
    return true;
}
