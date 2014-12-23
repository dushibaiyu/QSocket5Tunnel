#ifndef TcpServer_H
#define TcpServer_H

#include <QTcpServer>
#include <QList>
#include "configclass.h"
#include "clientconmanger.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();
     void clear();
protected:
    void incomingConnection(qintptr socketDescriptor);
    void goFawrd() {
        cutsize ++;
        if (cutsize >= manger.size())
            cutsize = 0;
    }
private:
    QList<ClientConManger *> manger;
    int cutsize;
};

#endif // TcpServer_H
