#ifndef REMOTESOCKET_H
#define REMOTESOCKET_H

#include "qasiotcpsocket.h"

class RemoteSocket : public QAsioTcpSocket
{
    Q_OBJECT
public:
    explicit RemoteSocket(int id,QObject *parent = 0):
        QAsioTcpSocket(4096,parent),socketId(id)
    {
        connect(this,&RemoteSocket::connected,[&](){
            if (!lacalData.isEmpty()) {
                this->write(lacalData);
                lacalData.clear();
            }
        });
    }
    ~RemoteSocket(){}
    int getSocketID() const
    {
        return this->socketId;
    }

    void Write(const QByteArray & data) {
        if (state() == ConnectingState) {
            lacalData.append(data);
        } else {
            write(data);
        }
    }

private:
    QByteArray lacalData;
    int socketId;
};

#endif // REMOTESOCKET_H
