#ifndef REMOTESOCKET_H
#define REMOTESOCKET_H

#include <QTcpSocket>

class RemoteSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit RemoteSocket(int id,QObject *parent = 0):
        QTcpSocket(parent),socketId(id)
    {}
    int getSocketID() const
    {
        return this->socketId;
    }

private:
    int socketId;
};

#endif // REMOTESOCKET_H
