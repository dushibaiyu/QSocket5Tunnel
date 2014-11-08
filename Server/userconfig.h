#ifndef USERCONFIG_H
#define USERCONFIG_H

//#include <QMutex>
#include <QHash>
#include <QPair>
#include <QString>

class UserConfig
{
public:
    static UserConfig & getClass();
    int getUserId(const QString & user, const QString & pass, QString &token);
    QString getToken(int userid) const;
private:
    UserConfig();
//    QMutex mut;
    QHash<QString,QPair<QString,int> > userHash;
    QHash<int,QString> tokenHash;
};

class SysConfig
{
public :
    SysConfig();
    int getBindPort() const
    {return port;}
    bool getThreadType() const
    {return type;}
    int getMaxThread() const
    {return size;}
private:
    int port;
    int size;
    bool type;
};
#endif // USERCONFIG_H
