#ifndef USERCONFIG_H
#define USERCONFIG_H

//#include <QMutex>
#include <QHash>
#include <QPair>
#include <QString>

/*
 * UserConfig格式：
 * 一行一个用户，
 * 每行的格式：
 * username;password;id(int);
 * 用户名和密码中不能出现分号(;)
*/

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
    void saveConfig();
    int getBindPort() const
    {return port;}
//    bool getThreadType() const
//    {return type;}
    int getMaxThread() const
    {return size;}
private:
    int port;
    int size;
//    bool type;//true:固定每个链接处理几个，false：固定线程数
};
#endif // USERCONFIG_H
