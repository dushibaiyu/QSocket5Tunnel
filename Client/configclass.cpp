#include "configclass.h"
#include <QSettings>

const QByteArray pass = "http://www.islove.me";

ConfigClass::ConfigClass()
{
    loadSetting();
}

ConfigClass::~ConfigClass()
{
    saveSetting();
}

ConfigClass & ConfigClass::getClass()
{
    static ConfigClass config;
    return config;
}

void ConfigClass::loadSetting()
{
    QSettings sett("QS5ClientTunnel.conf",QSettings::IniFormat);
    serverUrl = sett.value("config/server","127.0.0.1").toString();
    serverPort = sett.value("config/serverPort",8888).toInt();
    localPort = sett.value("config/localPort",6666).toInt();
    user = sett.value("config/user").toString();
    QByteArray data = sett.value("config/password").toByteArray();
    if (!data.isEmpty()) {
        OpensslAES aes(pass);
        data = aes.decrypt(data);
        password = QString::fromUtf8(data);
    }
}

void ConfigClass::saveSetting()
{
    QSettings sett("QS5ClientTunnel.conf",QSettings::IniFormat);
    sett.setValue("config/server",serverUrl);
    sett.setValue("config/serverPort",serverPort);
    sett.setValue("config/localPort",localPort);
    sett.setValue("config/user",user);
    OpensslAES aes(pass);
    QByteArray data = password.toUtf8();
    data = aes.encrypt(data);
    sett.setValue("config/password",data);
}
