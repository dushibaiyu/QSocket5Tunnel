#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "threadserver.h"
#include "qasiotcpsocket.h"
#include "clientsocket.h"
#include <QThread>
#include <QDebug>


ThreadServer::ThreadServer(int threadsize,int asioThread ,QObject *parent)
    : QAsioTcpServer(threadsize,asioThread,parent)
{
    qDebug() << "Main Thread Id :" << QThread::currentThreadId();
    handler.initThreadType(ThreadHandle::THREADSIZE,threadsize);
}

ThreadServer::~ThreadServer()
{
    this->close();
    if (clients.isEmpty()) {
        qDeleteAll(clients.begin(),clients.end());
    }
}


void ThreadServer::incomingConnection(asio::ip::tcp::socket *socket)
{
    ClientSocket * soc = new ClientSocket(socket);
    soc->moveToThread(handler.getThread());
    int id  = soc->socketDescriptor();
    if (id != -1) {
        QObject::connect(soc,&ClientSocket::sentDiscon,this,&ThreadServer::removeThread);
        mutex.lock();
        clients.insert(id,soc);
        mutex.unlock();
    }
}

void ThreadServer::removeThread(QThread * thread,int id)
{
    handler.removeThread(thread);
    mutex.lock();
    ClientSocket * sock = clients.value(id,nullptr);
    if (sock != nullptr) {
        sock->deleteLater();
        clients.remove(id);
    }
    mutex.unlock();
}
