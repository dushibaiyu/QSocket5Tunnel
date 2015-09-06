#include "clientsocket.h"
#include "remotesocket.h"

ClientSocket::ClientSocket(QAsioTcpsocket * soc,QObject * parent) :
    QObject(parent),lastSize(0),socket_(soc),isKey(false)
{
    aes = new QAesWrap(QByteArray("dushibaiyu"),QByteArray("dushibaiyu.com"),QAesWrap::AES_256);
    connect(socket_,&QAsioTcpsocket::sentReadData,this,&ClientSocket::readData,Qt::DirectConnection);
    connect(socket_,&QAsioTcpsocket::disConnected,[&](){emit socketDis(this);clear();});
    socket_->do_start();
}

ClientSocket::~ClientSocket()
{
    clear();
    socket_->disconnectFromHost();
    delete aes;
    delete socket_;
}

void ClientSocket::readData(const QByteArray & data)
{
    if (buffer.isOpen() && buffer.atEnd()) // Everything has been read; the buffer is safe to reset
        buffer.close();
    if (!buffer.isOpen())
        buffer.open(QBuffer::ReadWrite|QBuffer::Truncate);
    qint64 readPos = buffer.pos();
    buffer.seek(buffer.size());
    buffer.write(data);
    buffer.seek(readPos);
    if (lastSize == 0) {
        if (buffer.bytesAvailable() < 4) return;
        char str[4] = {0};
        buffer.read(str,4);
        lastSize = qFromBigEndian<uint>((uchar*)str);
    }
    while (buffer.bytesAvailable() >= static_cast<qint64>(lastSize)) {
        OperaterType type;
        int id;
        QByteArray revdata = deserializeData(getAes(),type,id,buffer.read(lastSize));
        qDebug() << "ClientSocket::readData : type = " << type << "  id = " << id;
        switch (type) {
        case NeedKey:
            isKey = false;
            qDebug() << " get Key :" << revdata;
            emit getKey(this,revdata);
            break;
        case NewLink:
        {
            qDebug() << "link new :" << revdata;
            QList<QByteArray> list = revdata.split('@');
            if (list.size() != 2) return;
            auto sock = new RemoteSocket(this,id);
            sock->moveToThread(this->thread());
            lock.lockForWrite();
            socketList.insert(id,sock);
            lock.unlock();
            sock->connectTo(list.at(0),list.at(1).toShort());//新的连接
        }
            break;
        case DisLink:
        {
            auto tp = getRemote(id);
            if (tp != nullptr) {
                removeConnet(id);
                delete tp;
            }
        }
            break;
        case SwapData:
        {
            qDebug() << "SwapData id = " << id;
            auto tp = getRemote(id);
            if (tp != nullptr) {
                tp->write(revdata);
            } else {
                write(serializeData(getAes(),DisLink,id,QByteArray()));
            }
        }
            break;
        default:
            write(serializeData(getAes(),DisLink,id,QByteArray()));
            break;
        }
        if (!buffer.atEnd() && buffer.bytesAvailable() > 4) {
            char str[4] = {0};
            buffer.read(str,4);
            lastSize = qFromBigEndian<uint>((uchar*)str);
        } else {
            lastSize = 0 ;
            return ;
        }
    }

}

void ClientSocket::haveKey(const QByteArray & key)
{
    if (isKey) return;
    qDebug () << "ClientSocket::haveKey = " << key;
    if (key.isEmpty()) socket_->disconnectFromHost();
    isKey = true;
    write(serializeData(getAes(),NeedKey,0,key));
    delete aes;
    aes = new QAesWrap(key,key,QAesWrap::AES_256);
}

void ClientSocket::clear()
{
    isKey = false;
    lock.lockForWrite();
    for (auto it = socketList.begin(); it != socketList.end(); ++it) {
        it.value()->close();
        it.value()->deleteLater();
    }
    socketList.clear();
    lock.unlock();
}
