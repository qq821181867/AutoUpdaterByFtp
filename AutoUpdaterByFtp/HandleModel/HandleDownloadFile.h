#ifndef HANDLEDOWNLOADFILE_H
#define HANDLEDOWNLOADFILE_H

#include <QObject>
#include "FtpManager.h"

class HandleDownloadFile : public QObject
{
    Q_OBJECT
public:
    explicit HandleDownloadFile(QObject *parent = nullptr);
    void setFtpInfo(const QString &host,int port,
                    const QString &user, const QString &password);
    void setDownloadInfo(const QString &downloadDir, const QString &workDir);

signals:
    // 下载进度
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();

public slots:
    void slotDownloadFileName(const QString &downloadFileName);

private:
    QString m_downloadDir;
    QString m_workDir;
    //
    FtpManager m_FtpManager;
};

#endif // HANDLEDOWNLOADFILE_H
