#include "HandleAutoUpdater.h"
#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <Windows.h>
#include <QTimer>
#include <QFileInfo>
#include <QDir>

#define HANDLEAUTOUPDATER_EXENAME "AutoUpdaterByFtp.exe"
#define HANDLEAUTOUPDATER_UPDATEWORKDIR "/updateDir"
#define HANDLEAUTOUPDATER_UPDATEINFOFILE "/updateInfo.ini"
#define HANDLEAUTOUPDATER_KEY_VERSION "updateInfo/Version"
#define HANDLEAUTOUPDATER_KEY_UPDATEFILE "updateInfo/updateFile"

HandleAutoUpdater::HandleAutoUpdater(QObject *parent) : QObject(parent)
{

}

void HandleAutoUpdater::startHandle()
{
    if(!checkHaveUpdateModle())
    {
        return;
    }
    //
    QString updatefile = checkIsUpdateVersion();
    if(updatefile.isEmpty())
    {
        //版本未更新开始检测
        startCheckUpdateVersion();
    }
    else
    {
        //版本更新,启动解压更新
        startZipUpdateVersion();
    }
}

QString HandleAutoUpdater::checkIsUpdateVersion()
{
    QString updateDir = QApplication::applicationDirPath()+HANDLEAUTOUPDATER_UPDATEWORKDIR;
    QDir dir(updateDir);
    if(!dir.exists())
    {
        dir.mkdir(updateDir);
    }
    QString updateInfofile = updateDir+HANDLEAUTOUPDATER_UPDATEINFOFILE;
    QSettings settings(updateInfofile, QSettings::IniFormat);
    QString downVersion = settings.value(HANDLEAUTOUPDATER_KEY_VERSION,QApplication::applicationVersion()).toString();
    if(QApplication::applicationVersion()==downVersion)
    {
        //版本未更新
        return QString();
    }
    return settings.value(HANDLEAUTOUPDATER_KEY_UPDATEFILE,"").toString();
}

void HandleAutoUpdater::startCheckUpdateVersion()
{
    m_pProcess = new QProcess(this);
    QString checkUpdateExe = QApplication::applicationDirPath()+"/"+HANDLEAUTOUPDATER_EXENAME;
    QString appName = QApplication::applicationName();
    QString appVersion = QApplication::applicationVersion();
    QString appPid = QString::number(GetCurrentProcessId());
    m_pProcess->start(checkUpdateExe,
                      QStringList()<<appName<<appVersion<<appPid);
    qDebug()<<__FUNCTION__<<checkUpdateExe<<appName<<appVersion<<appPid;
}

void HandleAutoUpdater::startZipUpdateVersion()
{
    QFileInfo fileinfo(QApplication::applicationFilePath());
    QString qstrFileName = QApplication::applicationDirPath()+"/"+HANDLEAUTOUPDATER_EXENAME;
    qstrFileName=qstrFileName.replace('/',"\\");
    qDebug()<<__FUNCTION__<<qstrFileName<<fileinfo.fileName()<<ShellExecuteA(NULL, "runas", qstrFileName.toStdString().c_str() , fileinfo.fileName().toStdString().c_str(), NULL, SW_SHOWNORMAL);
    QTimer::singleShot(0,[]{
        QApplication::exit();
    });
}

bool HandleAutoUpdater::checkHaveUpdateModle()
{
    return QFile::exists(QApplication::applicationDirPath()+"/"+HANDLEAUTOUPDATER_EXENAME);
}
