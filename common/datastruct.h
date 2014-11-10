#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QDataStream>
#include <QByteArray>
#include <QDebug>


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
