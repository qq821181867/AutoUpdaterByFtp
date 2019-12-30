#include "HandleCheckVersion.h"
#include <QSettings>
#include <QProcess>
#include <QApplication>
#include <QDebug>

#define HANDLECHECKVERSION_UPDATEINFO "updateInfo.ini"
//ini
#define HANDLECHECKVERSION_KEY_VERSION "updateInfo/Version"
#define HANDLECHECKVERSION_KEY_UPDATEFILE "updateInfo/updateFile"

HandleCheckVersion::HandleCheckVersion(QObject *parent) : QObject(parent)
{
    connect(&m_timerChecked,&QTimer::timeout, this, &HandleCheckVersion::slotTimerChecked);
    connect(&m_FtpManager,&FtpManager::downloadFinished, this, &HandleCheckVersion::slotFtpDownloadFinished);
}

void HandleCheckVersion::setFtpInfo(const QString &host, int port,
                                    const QString &user, const QString &password)
{
    m_FtpManager.setHostPort(host,port);
    m_FtpManager.setUserInfo(user, password);
}

void HandleCheckVersion::setDownloadInfo(const QString &downloadDir, const QString &workDir)
{
    m_downloadDir = downloadDir;
    m_workDir = workDir;
}

QString HandleCheckVersion::getDownloadFileName() const
{
    QString file = m_workDir+"/"+HANDLECHECKVERSION_UPDATEINFO;
    QSettings settings(file, QSettings::IniFormat);
    return settings.value(HANDLECHECKVERSION_KEY_UPDATEFILE,"").toString();
}

QString HandleCheckVersion::updateVersion() const
{
    return m_updateVersion;
}

void HandleCheckVersion::setUpdateVersion(const QString &updateVersion)
{
    m_updateVersion = updateVersion;
}

void HandleCheckVersion::startCheckedUpdate(int sec)
{
    m_timerChecked.setSingleShot(true);
    m_timerChecked.setInterval(sec*1000);
    m_timerChecked.start();
}

void HandleCheckVersion::updateToVersionSetting(const QString &version)
{
    QString file = m_workDir+"/"+HANDLECHECKVERSION_UPDATEINFO;
    QSettings settings(file, QSettings::IniFormat);
    settings.setValue(HANDLECHECKVERSION_KEY_VERSION,version);
}

void HandleCheckVersion::slotFtpDownloadFinished()
{
    if(checkUpdateAppIsQuit())
    {
        qDebug()<<__FUNCTION__<<"checkUpdateAppIsQuit";
        //监控主程序退出，发出退出信号,退出更新进程
        QTimer::singleShot(0,[]{
            QApplication::exit();
        });
    }
    //
    QString updateFile = checkIsUpdate();
    if(updateFile.isEmpty())
    {
        m_timerChecked.start();
        return;
    }
    //版本更新
    emit sigUpdateFile(updateFile);
}

void HandleCheckVersion::slotTimerChecked()
{
    m_FtpManager.get(m_downloadDir+"/"+HANDLECHECKVERSION_UPDATEINFO,m_workDir+"/"+HANDLECHECKVERSION_UPDATEINFO);
}

bool HandleCheckVersion::checkUpdateAppIsQuit()
{
    QProcess process;
    process.start("tasklist" ,QStringList()<<"/FI"<<"PID eq " +getUpdateAppPid());
    process.waitForFinished();
    QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput());
    if(outputStr.contains(getUpdateAppPid()))
    {
        return false;
    }
    return true;
}

QString HandleCheckVersion::checkIsUpdate()
{
    QString file = m_workDir+"/"+HANDLECHECKVERSION_UPDATEINFO;
    QSettings settings(file, QSettings::IniFormat);
    QString downVersion = settings.value(HANDLECHECKVERSION_KEY_VERSION,"").toString();
    if(updateVersion()==downVersion)
    {
        //版本未更新
        return QString();
    }
    return settings.value(HANDLECHECKVERSION_KEY_UPDATEFILE,"").toString();
}

QString HandleCheckVersion::getUpdateAppPid() const
{
    return m_updateAppPid;
}

void HandleCheckVersion::setUpdateAppPid(const QString &updateAppPid)
{
    m_updateAppPid = updateAppPid;
}
