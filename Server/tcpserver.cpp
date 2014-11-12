#include "tcpserver.h"
#include "threadhandle.h"
#include "userconfig.h"

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
     tcpClient = new  QMap<int,ClientSocket *>;
}

TcpServer::~TcpServer()
{
    emit this->sentDisConnect(-1);

    delete tcpClient;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    auto th = ThreadHandle::getClass().getThread();
    ClientSocket * tcpTemp = new ClientSocket(socketDescriptor);

    connect(tcpTemp,&ClientSocket::sockDisConnect,this,&TcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(this,&TcpServer::sentDisConnect,tcpTemp,&ClientSocket::disConTcp);//断开信号
    connect(th,&QThread::finished,tcpTemp,&ClientSocket::deleteThis);
    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中

    emit connectClient(tcpTemp->socketDescriptor(),tcpTemp->peerAddress().toString(),tcpTemp->peerPort());
}

void TcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
    emit sockDisConnect(handle,ip,prot);
}


void TcpServer::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandle::getClass().clear();
    tcpClient->clear();
}
