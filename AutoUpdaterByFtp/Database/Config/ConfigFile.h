#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QObject>

/**
 * @brief The ConfigFile class
 *  读取配置文件只需在 void ConfigFile::variableMap() 中注册全局变量
 */
class ConfigFile
{
public:
    static ConfigFile& getInstance();

public:
    ConfigFile();
    void readConfig();
    void setConfig();

private:
    struct VariableInfo {
        QString key;
        QString defaultValue;
    };

    QMap<QString*, VariableInfo> variableMap();

private:
    QString m_qstrFileConfig;

public:
    //配置
    QString g_Host;
    QString g_Port;
    QString g_User;
    QString g_Password;
    QString g_UpdateWorkDir;
    //
    QString g_UpdateTime;
};

inline ConfigFile &configFile() {
    return ConfigFile::getInstance();
}

#endif // CONFIGFILE_H
