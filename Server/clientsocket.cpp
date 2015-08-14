#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "clientsocket.h"
#include "userconfig.h"

ClientSocket::ClientSocket(QAsioTcpsocket * soc,QObject * parent) : QObject(parent),
   socket_(soc),lastsize(0),aes(nullptr),isKey(false),getSize(0),revSize(0)
{
    connect(socket_,&QAsioTcpsocket::sentReadData,this,&ClientSocket::readData,Qt::DirectConnection);
    connect(socket_,&QAsioTcpsocket::disConnected,this,&ClientSocket::socketDis);
    socket_->do_start();
}

ClientSocket::~ClientSocket()
{
    if (aes != nullptr)
        delete aes;
    if (!socketList.isEmpty()) {
        qDeleteAll(socketList.begin(),socketList.end());
    }
    socketList.clear();
}

void ClientSocket::readData(const QByteArray & data)
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
        getSize += lastSize;
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

