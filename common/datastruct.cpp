#include "datastruct.h"

QDataStream & operator<< (QDataStream & out, swapData & data)
{
    out << data.operater << data.userID << data.socketID << data.data;
    return out;
}

QDataStream & operator >> (QDataStream & in, swapData & data)
{
    in >> data.operater >> data.userID >> data.socketID >> data.data;
    return in;
}
