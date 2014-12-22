#include "userconfig.h"
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QStringList>
#include <QUuid>

UserConfig::UserConfig()
{
    if (QFile::exists("UserConfig")) {
        QFile file("UserConfig");
        file.open(QFile::ReadOnly);
        QTextStream stream(&file);
        QStringList list;
        while(!stream.atEnd()) {
            list = stream.readLine().split(';');
            if (list.size() != 4) continue;
            int id = list.at(2).toInt();
            if (id <= 0) continue;
            userHash.insert(list.at(0),qMakePair(list.at(1),id));
        }
    }
    if (userHash.isEmpty()) {
        userHash.insert(QString("dushibaiyu"),qMakePair(QString("www.dushibaiyu.com"),1000));
    }
}

int UserConfig::getUserId(const QString &user, const QString &pass, QString &token)
{
    QHash<QString,QPair<QString,int> >::iterator it = userHash.find(user);
    if (it == userHash.end()) {
        return -1;
    } else if (it.value().first != pass) {
        return -1;
    } else {
        token = QUuid::createUuid().toString();
        tokenHash[it.value().second] = token ;
        return it.value().second;
    }
}

QString UserConfig::getToken(int userid) const
{
    return tokenHash.value(userid,QString());
}

UserConfig & UserConfig::getClass()
{
    static UserConfig user;
    return user;
}


SysConfig::SysConfig()
{
    QSettings seting("SysConfig",QSettings::IniFormat);
    port = seting.value("bindPort",8888).toInt();
    type = seting.value("ThreadType",false).toBool();
    size = seting.value("ThreadMax",100).toInt();
}

