#ifndef LocalSocket_H
#define LocalSocket_H

#include <QTcpSocket>

class LocalSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit LocalSocket(int id,QObject *parent = 0):
        QTcpSocket(parent),socketId(id)
    {
        this->setSocketDescriptor(id);
    }
    int getSocketID() const
    {
        return this->socketId;
    }

private:
    int socketId;
};

#endif // LocalSocket_H
