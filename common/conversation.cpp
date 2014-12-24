#include "conversation.h"
#include "opensslaes.h"
#include "../common/datastruct.h"
#include <QTcpSocket>

Conversation::Conversation(OpensslAES *aes, QObject *parent) :
    QObject(parent),socket1(nullptr),socket2(nullptr),aes(aes)
{}

bool Conversation::setDescriptor(qint16 socket)
{
    if (socket2 == nullptr) {
        socket2 = new QTcpSocket(this);
    } else {
        if (socket2->state() == QTcpSocket::ConnectedState)
            socket2->disconnectFromHost();
        disconnect(socket2, &QTcpSocket::readyRead, this, &Conversation::socket2Read);
        disconnect(socket2, &QTcpSocket::disconnected, this, &Conversation::socket2Closed);
    }

    if (socket1 == nullptr) {
        socket1 = new QTcpSocket(this);
    } else {
        if (socket1->state() == QTcpSocket::ConnectedState)
            socket1->disconnectFromHost();
        disconnect(socket1, &QTcpSocket::readyRead, this, &Conversation::socket1Read);
        disconnect(socket1, &QTcpSocket::disconnected, this, &Conversation::socket1Closed);
    }

    if (initConSocket(socket)) {
        connectSockets();
        return true;
    }
    return false;
}

void Conversation::connectSockets()
{
    connect(socket1, &QTcpSocket::readyRead, this, &Conversation::socket1Read);
    connect(socket1, &QTcpSocket::disconnected, this, &Conversation::socket1Closed);
    connect(socket2, &QTcpSocket::readyRead, this, &Conversation::socket2Read);
    connect(socket2, &QTcpSocket::disconnected, this, &Conversation::socket2Closed);
    socket2Read();
    socket1Read();
}

void Conversation::socket1Read()
{
    if (socket1->bytesAvailable() > 0) {
        //加密后传给socket2
        QByteArray data = socket1->readAll();
        socket2->write(encryptData(aes,data));
    }
}

void Conversation::socket2Read()
{
    if (socket2->bytesAvailable() > 0) {
        //解密后传给socket1
        QByteArray data = socket2->readAll();
        if (decryptData(aes,data))
            socket1->write(data);
    }
}

void Conversation::socket2Closed()
{
    if(socket1->state() != QTcpSocket::ConnectedState)
        emit connectionClosed(this);
    else
        socket1->disconnectFromHost();

}

void Conversation::socket1Closed() {
    if(socket2->state() != QTcpSocket::ConnectedState)
        emit connectionClosed(this);
    else
        socket2->disconnectFromHost();
}

