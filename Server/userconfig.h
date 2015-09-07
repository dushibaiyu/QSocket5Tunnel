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
    SysConfig & getConfig() {
        static SysConfig conf;
        return conf;
    }


    int getBindPort() const
    {return port;}
    int getServerThreads() const
    {return serverSize;}
    int getSocketThreads() const
    {return socketSize;}
    QString getInitPassword() const
    { return password;}
    QString getInitSalt() const
    { return salt; }

private:
    SysConfig();
    int port;
    int serverSize;
    int socketSize;
    QString password;
    QString salt;
};
#endif // USERCONFIG_H
