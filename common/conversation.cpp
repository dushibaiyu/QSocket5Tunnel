#include "conversation.h"
#include <QTcpSocket>

Conversation::Conversation(OpensslAES *aes, QObject *parent) :
    QObject(parent),remote(nullptr),local(nullptr)
{}

bool Conversation::setDescriptor(qint16 socket)
{
    if (local == nullptr) {
        local = new QTcpSocket(this);
    } else {
        if (local->state() == QTcpSocket::connected())
            local->disconnectFromHost();
        disconnect(local, &QTcpSocket::readyRead, this, &Conversation::localRead);
        disconnect(local, &QTcpSocket::disconnected, this, &Conversation::localClosed);
    }

    if (remote == nullptr) {
        remote = new QTcpSocket(this);
    } else {
        if (remote->state() == QTcpSocket::connected())
            remote->disconnectFromHost();
        disconnect(remote, &QTcpSocket::readyRead, this, &Conversation::remoteRead);
        disconnect(remote, &QTcpSocket::disconnected, this, &Conversation::remoteClosed);
    }

    if (initLocalSocket(socket)) {
        connectSockets();
        return true;
    }
    return false;
}

void Conversation::connectSockets()
{
    connect(remote, &QTcpSocket::readyRead, this, &Conversation::remoteRead);
    connect(remote, &QTcpSocket::disconnected, this, &Conversation::remoteClosed);
    connect(local, &QTcpSocket::readyRead, this, &Conversation::localRead);
    connect(local, &QTcpSocket::disconnected, this, &Conversation::localClosed);
    localRead();
    remoteRead();
}

void Conversation::remoteRead()
{
    if (remote->bytesAvailable() > 0)
        local->write(remote->readAll());
}

void Conversation::localRead()
{
    if (local->bytesAvailable() > 0)
        remote->write(local->readAll());
}

void Conversation::remoteClosed()
{
    if(local->state() != QTcpSocket::connected())
        emit connectionClosed(this);
    else
        local->disconnectFromHost();

}

void Conversation::localClosed() {
    if(remote->state() != QTcpSocket::connected())
        emit connectionClosed(this);
    else
        remote->disconnectFromHost();
}

