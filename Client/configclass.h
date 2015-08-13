#ifndef CONFIGCLASS_H
#define CONFIGCLASS_H
#include <QString>
//#include "../common/datastruct.h"

class ConfigClass
{
public:
    ~ConfigClass();

    static ConfigClass & getClass();
    void saveSetting();

    QString serverUrl;
    int serverPort;
    int localPort;
    QString user;
    QString password;

private:
    void loadSetting();

    ConfigClass();
};

#endif // CONFIGCLASS_H
