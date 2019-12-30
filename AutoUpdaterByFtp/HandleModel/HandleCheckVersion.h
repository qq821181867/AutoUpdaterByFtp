#ifndef HANDLECHECKVERSION_H
#define HANDLECHECKVERSION_H

#include <QObject>
#include "FtpManager.h"
#include <QTimer>

class HandleCheckVersion : public QObject
{
    Q_OBJECT
public:
    explicit HandleCheckVersion(QObject *parent = nullptr);
    void setFtpInfo(const QString &host,int port,
                    const QString &user, const QString &password);
    void setDownloadInfo(const QString &downloadDir, const QString &workDir);
    QString getDownloadFileName() const;
    //
    QString updateVersion() const;
    void setUpdateVersion(const QString &updateVersion);
    QString getUpdateAppPid() const;
    void setUpdateAppPid(const QString &updateAppPid);
    //
    void startCheckedUpdate(int sec);
    //解压后版本判断及更新配置信息
    void updateToVersionSetting(const QString &version);

signals:
    void sigUpdateFile(const QString &updateByDownloadFileName);

private slots:
    void slotFtpDownloadFinished();
    void slotTimerChecked();

private:
    bool checkUpdateAppIsQuit();
    QString checkIsUpdate();

private:
    QString m_updateAppPid;
    QString m_updateVersion;
    QString m_downloadDir;
    QString m_workDir;
    //
    FtpManager m_FtpManager;
    QTimer m_timerChecked;
};

#endif // HANDLECHECKVERSION_H
