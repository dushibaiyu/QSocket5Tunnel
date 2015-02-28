#ifndef THREADSERVER_H
#define THREADSERVER_H

#include "qasiotcpserver.h"
#include "threadhandle.h"
#include <QMap>
#include <QMutex>

class ClientSocket;

class ThreadServer : public QAsioTcpServer
{
    Q_OBJECT
public:
    explicit ThreadServer(int threadsize = 2,int asioThread = 1 ,QObject *parent = 0);
    ~ThreadServer();

public slots:
    void removeThread(QThread * thread,int id);

protected:
    void incomingConnection();

private:
    ThreadHandle handler;
    QMap<int,ClientSocket *> clients;
    QMutex mutex;
};

#endif // TESTSERVER_H
