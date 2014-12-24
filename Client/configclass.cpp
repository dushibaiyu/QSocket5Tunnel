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
    key = sett.value("config/Key","dushibaiyu.com").toString();
    localPort = sett.value("config/localPort",6666).toInt();
    MaxThread = sett.value("config/MaxThread",5).toInt();
    MaxCache = sett.value("config/MaxCache",20).toInt();
}

void ConfigClass::saveSetting()
{
    QSettings sett("QS5ClientTunnel.conf",QSettings::IniFormat);
    sett.setValue("config/server",serverUrl);
    sett.setValue("config/serverPort",serverPort);
    sett.setValue("config/Key",key);
    sett.setValue("config/localPort",localPort);
    sett.setValue("config/MaxThread",MaxThread);
    sett.setValue("config/MaxCache",MaxCache);
}
