#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QWidget>
#include <QSystemTrayIcon>
#include "FtpManager.h"
#include "HandleModel/HandleCheckVersion.h"
#include "HandleModel/HandleDownloadFile.h"
#include <private/qzipreader_p.h>
#include <QLabel>

namespace Ui {
class SystemTray;
}

class SystemTray : public QWidget
{
    Q_OBJECT

public:
    explicit SystemTray(QWidget *parent = 0);
    ~SystemTray();
    void initIcon();
    void initConfig();
    //
    QString updateAppName() const;
    void setUpdateAppName(const QString &updateAppName);

    QString updateVersion() const;
    void setUpdateVersion(const QString &updateVersion);

    QString updateAppPid() const;
    void setUpdateAppPid(const QString &updateAppPid);
    //
    void startCheckedUpdate();
    void handleUpdateVersion(const QString &updateAppExe);

private slots:
    void slotActiveTray(QSystemTrayIcon::ActivationReason reason);
    void slotStartUpdate();
    void slotUpdateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void slotUpdateDownloadFinished();

private:
    void showSystemMessage();
    //
    static void handleZip(const QString &zipFile, const QString &unZipDir);
    static QString getFileVersion(const QString &fileName);

private:
    Ui::SystemTray *ui;
    QSystemTrayIcon *m_pSystemTrayIcon;
    QLabel *m_pLbGifBg;
    //
    QString m_updateAppName;
    QString m_updateVersion;
    QString m_updateAppPid;
    //
    float m_downloadProgress=0;
    HandleCheckVersion m_HandleCheckVersion;
    HandleDownloadFile m_HandleDownloadFile;
};

#endif // SYSTEMTRAY_H
