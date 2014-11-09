#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
/***
 * TODO: next:
 *  添加加密，安全和快速的加密算法
 *  优化下服务端的进程架构，尽量直接线程建立socket，取消用moveToThread
 *  服务端替换底层事件循环系统，用libev的事件替换，这样在linux，bsd，mac下就能有更好的性能。
 *  测试压缩，看下性能，考虑是否压缩传输数据。
 *
 * TODO: Future
 *  登录认证模块独立出来，动态返回服务器地址，多服务器的话，可以达到负载均衡
 *  服务端加上数据库，管理用户和token，也可以登录用http，和http交互。
 *  如果用户系统完善，添加流量统计。
 *  客户端界面设计优化
 *  考虑加入其他代理协议。
 *  如果有效率更高的，更换数据序列化，和传输的数据结构。
 * */

#define DATASTREAM_VISION QDataStream::Qt_5_2
/****
 * operater : 值说明
 *          0 ：数据交换，就是socket5的数据转发
 *          1 ：新建链接，新建需要的远程链接，发想服务器由此状态，服务器返回无此状态
 *          2 ：断开链接。
 *          3 ：用户登录。
 * ****/

struct swapData
{
    qint8 operater = 8;//操作
    qint32 userID = -1;//用户ID
    qint32 socketID = -1;//socket链接的ID
    QByteArray data;//交换的数据
};

QDataStream & operator<< (QDataStream & out, swapData & data);

QDataStream & operator >> (QDataStream & in, swapData & data);

template <typename T>
inline bool serializeData(QByteArray & data, T & structed)
{
    QDataStream stream(&data,QIODevice::WriteOnly);
    stream.setVersion(DATASTREAM_VISION);
    stream << structed;
    if (stream.status() == QDataStream::Ok)
        return true;
    else
    {
        qDebug() << "数据序列化错误。";
        return false;
    }
}

template <typename T>
inline bool deSerializeData(QByteArray & data, T & structed)
{
    if (data.isEmpty()) return false;
    QDataStream stream(&data,QIODevice::ReadOnly);
    stream.setVersion(DATASTREAM_VISION);
    stream >> structed;
    if (stream.status() == QDataStream::Ok)
        return true;
    else
    {
        qDebug() << "数据序列化错误。";
        return false;
    }
}

//inline bool decryptData(BotanAES256 * aes,swapData & data)
//{
//#ifndef USE_ENCRYPT
//    return true;
//#endif
//    if (data.data.isEmpty()) return false;
//    data.data = aes->Decrypt(data.data);
//    if (data.data.isEmpty()) return false;
//    return true;
//}

//inline QByteArray encryptData(BotanAES256 * aes,const QByteArray &data)
//{
//#ifndef USE_ENCRYPT
//    return data;
//#endif
//    return aes->Encrypt(data);
//}

#endif // DATASTRUCT_H
