#include "FtpManager.h"
#include <QFileInfo>

FtpManager::FtpManager(QObject *parent) : QObject(parent)
{
    // 设置协议
    m_pUrl.setScheme("ftp");
}

void FtpManager::setHostPort(const QString &host, int port)
{
    m_pUrl.setHost(host);
    m_pUrl.setPort(port);
}

void FtpManager::setUserInfo(const QString &userName, const QString &password)
{
    m_pUrl.setUserName(userName);
    m_pUrl.setPassword(password);
}

void FtpManager::put(const QString &fileName, const QString &path)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();

    m_pUrl.setPath(path);
    QNetworkReply *pReply = m_manager.put(QNetworkRequest(m_pUrl), data);

    connect(pReply, SIGNAL(uploadProgress(qint64, qint64)), this, SIGNAL(uploadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
}

void FtpManager::get(const QString &path, const QString &fileName)
{
    qDebug()<<__FUNCTION__<<path<<fileName;
    //
    QFileInfo info;
    info.setFile(fileName);

    m_file.setFileName(fileName);
    m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    m_pUrl.setPath(path);

    QNetworkReply *pReply = m_manager.get(QNetworkRequest(m_pUrl));

    connect(pReply, SIGNAL(finished()), this, SLOT(slotDownloadFinished()));
    connect(pReply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
}

void FtpManager::slotDownloadFinished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    switch (pReply->error()) {
    case QNetworkReply::NoError : {
        m_file.write(pReply->readAll());
        m_file.flush();
    }
        break;
    default:
        break;
    }

    m_file.close();
    pReply->deleteLater();
    emit downloadFinished();
}
