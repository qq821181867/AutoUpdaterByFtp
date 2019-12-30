#include "ConfigFile.h"
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDebug>

#define CONFIGFILE_FILE "/configUpdater.ini"
#define CONFIGFILE_CODEC "UTF-8"

ConfigFile::ConfigFile()
{
    QString strDir = QApplication::applicationDirPath()+"/config";
    m_qstrFileConfig = strDir+CONFIGFILE_FILE;

    if(!QFile::exists(m_qstrFileConfig))
    {
        QDir dir;
        dir.mkdir(strDir);
        //
        setConfig();
    }
}

 QMap<QString*, ConfigFile::VariableInfo> ConfigFile::variableMap()
{
    QMap<QString*, VariableInfo> map; // <变量的指针，变量的init信息>
    // variableMap.insert(&全局变量, {键名, 默认值});

    map.insert(&g_Host, {"Service/Host",""});
    map.insert(&g_Port, {"Service/Port","21"});
    map.insert(&g_User, {"Service/User",""});
    map.insert(&g_Password, {"Service/Password",""});
    map.insert(&g_UpdateWorkDir, {"Service/UpdateWorkDir",""});

    map.insert(&g_UpdateTime, {"UpdateConfig/UpdateTime","60"});
    //
    return map;
}

void ConfigFile::readConfig()
{
    auto map = variableMap();
    QSettings settings(m_qstrFileConfig, QSettings::IniFormat);
    settings.setIniCodec(CONFIGFILE_CODEC);
    for (auto itr = map.begin(); itr != map.end(); ++itr) {
        VariableInfo info = itr.value();
        *itr.key() =   settings.value(info.key, info.defaultValue).toString();
        if(*itr.key()==info.defaultValue && !settings.contains(info.key))
        {
            settings.setValue(info.key, info.defaultValue);
        }
        qDebug() << info.key << " = " << *itr.key();
    }
}

ConfigFile &ConfigFile::getInstance()
{
    static ConfigFile INSTANCE;
    return INSTANCE;
}

void ConfigFile::setConfig()
{
    auto map = variableMap();
   QSettings settings(m_qstrFileConfig, QSettings::IniFormat);
   settings.setIniCodec(CONFIGFILE_CODEC);
    for (auto itr = map.begin(); itr != map.end(); ++itr) {
        VariableInfo info = itr.value();
        settings.setValue(info.key, info.defaultValue);
    }
}


