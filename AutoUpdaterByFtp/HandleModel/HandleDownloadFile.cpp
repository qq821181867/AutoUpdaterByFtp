#include "HandleDownloadFile.h"

HandleDownloadFile::HandleDownloadFile(QObject *parent) : QObject(parent)
{
    connect(&m_FtpManager,&FtpManager::downloadProgress, this, &HandleDownloadFile::downloadProgress);
    connect(&m_FtpManager,&FtpManager::downloadFinished, this, &HandleDownloadFile::downloadFinished);
}

void HandleDownloadFile::setFtpInfo(const QString &host, int port, const QString &user, const QString &password)
{
    m_FtpManager.setHostPort(host,port);
    m_FtpManager.setUserInfo(user, password);
}

void HandleDownloadFile::setDownloadInfo(const QString &downloadDir, const QString &workDir)
{
    m_downloadDir = downloadDir;
    m_workDir = workDir;
}

void HandleDownloadFile::slotDownloadFileName(const QString &downloadFileName)
{
    QString downloadUrl = m_downloadDir+"/"+downloadFileName;
    QString saveFile = m_workDir+"/"+downloadFileName;
    m_FtpManager.get(downloadUrl,saveFile);
}
