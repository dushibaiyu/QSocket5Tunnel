#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QDataStream>
#include <QByteArray>

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


#endif // DATASTRUCT_H
