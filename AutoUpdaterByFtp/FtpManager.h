#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include <QUrl>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class FtpManager : public QObject
{
    Q_OBJECT
public:
    explicit FtpManager(QObject *parent = nullptr);
    // 设置地址和端口
    void setHostPort(const QString &host, int port = 21);
    // 设置登录 FTP 服务器的用户名和密码
    void setUserInfo(const QString &userName, const QString &password);
    // 上传文件
    void put(const QString &fileName, const QString &path);
    // 下载文件
    void get(const QString &path, const QString &fileName);

signals:
    void error(QNetworkReply::NetworkError);
    // 上传进度
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    // 下载进度
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();

private slots:
    // 下载过程中写文件
    void slotDownloadFinished();

private:
    QUrl m_pUrl;
    QFile m_file;
    QNetworkAccessManager m_manager;
};

#endif // FTPMANAGER_H
