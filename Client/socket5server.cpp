#include "socket5server.h"
#include "localsocket.h"

Socket5Server::Socket5Server(QObject *parent) : QObject(parent),
    server_(new QAsioTcpServer(4096,this)),socket_(new QAsioTcpsocket(4096,this)),
    isHaveKey(false),lastSize(0)
{
    aes = nullptr;
    connect(server_,&QAsioTcpServer::newConnection,this,&Socket5Server::newSocket,Qt::DirectConnection);
    connect(socket_,&QAsioTcpsocket::sentReadData,this,&Socket5Server::readData,Qt::DirectConnection);
    connect(socket_,&QAsioTcpsocket::disConnected,this,&Socket5Server::socketDis);
    connect(socket_,&QAsioTcpsocket::connected,[&](){/*TODO:发送Key请求*/});
}

Socket5Server::~Socket5Server()
{
    socket_->disconnectFromHost();
    delete server_;
    delete socket_;
}

void Socket5Server::newSocket(QAsioTcpsocket * socket)
{
    auto st = new LocalSocket(socket);
    st->moveToThread(this->thread());
}

void Socket5Server::readData(const QByteArray & data)
{
    if (buffer.atEnd()) // Everything has been read; the buffer is safe to reset
        buffer.close();
    if (!buffer.isOpen())
        buffer.open(QBuffer::ReadWrite|QBuffer::Truncate);
    buffer.write(data);
    if (lastSize == 0) {
        if (buffer.bytesAvailable() < 4) return;
        char str[4] = {0};
        buffer.read(str,4);
        lastSize = qFromBigEndian<uint>((uchar*)str);
    }
    while (buffer->bytesAvailable() >= static_cast<qint64>(lastSize)) {
//        auto bytearry = buffer->read(lastSize);
        //数据处理
        if (!buffer->atEnd() && buffer->bytesAvailable() > 4) {
            char str[4] = {0};
            buffer.read(str,4);
            lastSize = qFromBigEndian<uint>((uchar*)str);
        } else {
            lastSize = 0 ;
            return ;
        }
    }
}

void Socket5Server::socketDis()
{
    isHaveKey = false;
    server_->close();
    lock.lockForWrite();
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        it.value()->close();
        it.value()->deleteLater();
    }
    clients.clear();
    lock.unlock();
    emit socketDisconnet();
}
