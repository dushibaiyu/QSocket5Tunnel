#ifndef CONFIGCLASS_H
#define CONFIGCLASS_H
#include <QString>
#include "../common/datastruct.h"

class ConfigClass
{
public:
    ~ConfigClass();

    static ConfigClass & getClass();
    void saveSetting();

    QString key;
    int port;
    int MaxThread;
    int MaxCache;

private:
    void loadSetting();

    ConfigClass();
};

#endif // CONFIGCLASS_H
