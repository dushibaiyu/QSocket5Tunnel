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
    QSettings sett("QS5ServerTunnel.conf",QSettings::IniFormat);;
    key = sett.value("config/Key","dushibaiyu.com").toString();
    port = sett.value("config/Port",8888).toInt();
    MaxThread = sett.value("config/MaxThread",20).toInt();
    MaxCache = sett.value("config/MaxCache",30).toInt();
}

void ConfigClass::saveSetting()
{
    QSettings sett("QS5ServerTunnel.conf",QSettings::IniFormat);
    sett.setValue("config/Key",key);
    sett.setValue("config/Port",port);
    sett.setValue("config/MaxThread",MaxThread);
    sett.setValue("config/MaxCache",MaxCache);
}
