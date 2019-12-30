#include "SystemTray.h"
#include "ui_SystemTray.h"
#include "Database/Config/ConfigFile.h"
#include <Windows.h>
#include <QDir>
#include <QSettings>
#include <QMovie>

#pragma comment(lib, "version.lib")

#define SYSTEMTRAY_TITLETIME 2000
#define SYSTEMTRAY_TITLE "%1更新"
#define SYSTEMTRAY_UPDATEDIR "/updateDir"

SystemTray::SystemTray(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemTray)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //
    ui->setupUi(this);
    m_pSystemTrayIcon = new QSystemTrayIcon(this);
    initIcon();


    m_pLbGifBg = new QLabel(this);
    QMovie *pMovie = new QMovie(QApplication::applicationDirPath()+"/AutoUpdaterByFtp.gif",QByteArray(),m_pLbGifBg);
    m_pLbGifBg->setMovie(pMovie);
    m_pLbGifBg->setAlignment(Qt::AlignCenter);
    pMovie->start();
    m_pLbGifBg->adjustSize();
    this->resize(m_pLbGifBg->size());


    connect(m_pSystemTrayIcon,&QSystemTrayIcon::activated,this,&SystemTray::slotActiveTray);

    connect(&m_HandleCheckVersion, &HandleCheckVersion::sigUpdateFile, this, &SystemTray::slotStartUpdate);
    connect(&m_HandleDownloadFile, &HandleDownloadFile::downloadProgress, this, &SystemTray::slotUpdateDownloadProgress);
    connect(&m_HandleDownloadFile, &HandleDownloadFile::downloadFinished, this, &SystemTray::slotUpdateDownloadFinished);
    connect(&m_HandleCheckVersion, &HandleCheckVersion::sigUpdateFile, &m_HandleDownloadFile, &HandleDownloadFile::slotDownloadFileName);
}

SystemTray::~SystemTray()
{
    delete ui;
}

void SystemTray::initIcon()
{
    QString iconFile = QApplication::applicationDirPath()+"/AutoUpdaterByFtp.ico";
    if(QFile::exists(iconFile))
    {
        QApplication::setWindowIcon(QIcon(iconFile));
    }
    else
    {
        QApplication::setWindowIcon(QIcon("://updateIcon.png"));
    }
    m_pSystemTrayIcon->setIcon(QApplication::windowIcon());
}

void SystemTray::initConfig()
{
    ConfigFile::getInstance().readConfig();

    m_HandleCheckVersion.setFtpInfo(configFile().g_Host,configFile().g_Port.toInt(),
                                    configFile().g_User, configFile().g_Password);
    m_HandleDownloadFile.setFtpInfo(configFile().g_Host,configFile().g_Port.toInt(),
                                    configFile().g_User, configFile().g_Password);

    m_HandleCheckVersion.setDownloadInfo(configFile().g_UpdateWorkDir,QApplication::applicationDirPath()+SYSTEMTRAY_UPDATEDIR);
    m_HandleDownloadFile.setDownloadInfo(configFile().g_UpdateWorkDir,QApplication::applicationDirPath()+SYSTEMTRAY_UPDATEDIR);

    m_HandleCheckVersion.setUpdateVersion(updateVersion());
    m_HandleCheckVersion.setUpdateAppPid(updateAppPid());
}

void SystemTray::slotActiveTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Context:
//        showMenu();
        break;
    case QSystemTrayIcon::DoubleClick:
//        showWindow();
        break;
    case QSystemTrayIcon::Trigger:
        showSystemMessage();
        break;
    }
}

void SystemTray::slotStartUpdate()
{
    m_pSystemTrayIcon->show();
    showSystemMessage();
}

void SystemTray::slotUpdateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_downloadProgress = (float)bytesReceived/bytesTotal;
}

void SystemTray::slotUpdateDownloadFinished()
{
    m_downloadProgress = 0;
    m_pSystemTrayIcon->showMessage("系统提示",this->windowTitle()+"更新完成,下次启动生效",QSystemTrayIcon::MessageIcon::Information,SYSTEMTRAY_TITLETIME);
    //更新完成退出程序
    QTimer::singleShot(SYSTEMTRAY_TITLETIME+2000,[]{
        QApplication::exit();
    });
}

void SystemTray::showSystemMessage()
{
    QString text = this->windowTitle();
    if(m_downloadProgress>0)
    {
        text+=QString("%1%").arg(m_downloadProgress,0,'f',2);
    }
    m_pSystemTrayIcon->showMessage("系统提示",this->windowTitle(),QSystemTrayIcon::MessageIcon::Information,SYSTEMTRAY_TITLETIME);
}

void SystemTray::handleZip(const QString &zipFile, const QString &unZipDir)
{
     qDebug()<<__FUNCTION__<<zipFile<<unZipDir;
     if(zipFile.isEmpty() || !QFile::exists(zipFile))
     {
         //解压包为空或者文件不存在
         return;
     }
     QZipReader cZip(zipFile);
     if(!cZip.extractAll(unZipDir))
     {
         qDebug()<<__FUNCTION__<<"QZipReader.status()"<<cZip.status()<<"errorString()"<<cZip.device()->errorString();
     }
     cZip.close();
}

QString SystemTray::getFileVersion(const QString &fileName)
{
    QString vRetVersion;
    char vVersion[256] = {0};
    DWORD vHandle = 0;
    DWORD vInfoSize = 0;
    vInfoSize = ::GetFileVersionInfoSizeA(fileName.toStdString().c_str(), &vHandle);
    if(vInfoSize <= 0)
    {
        return false;
    }
    char *pData = new char[vInfoSize + 1];
    memset(pData, 0, vInfoSize + 1);
    if(!::GetFileVersionInfoA(fileName.toStdString().c_str(), NULL, vInfoSize, pData))
    {
        if(pData)
        {
            delete pData;
        }
        return   FALSE;
    }
    QString vTempStr = "\\";
    VS_FIXEDFILEINFO *pFileInfo;
    unsigned int vUInfoSize = 0;
    if(!::VerQueryValueA(pData, vTempStr.toStdString().c_str(), (void**)&pFileInfo, &vUInfoSize))
    {
        if(pData)
        {
            delete pData;
        }
        return false;
    }
    WORD vVer[4];
    vVer[0] = HIWORD(pFileInfo->dwProductVersionMS);
    vVer[1] = LOWORD(pFileInfo->dwProductVersionMS);
    vVer[2] = HIWORD(pFileInfo->dwProductVersionLS);
    vVer[3] = LOWORD(pFileInfo->dwProductVersionLS);
    sprintf(vVersion, "%d.%d.%d.%d", vVer[0], vVer[1], vVer[2], vVer[3]);
    if(pData)
    {
        delete pData;
    }
    vRetVersion = vVersion;
    return vRetVersion;
}

QString SystemTray::updateAppPid() const
{
    return m_updateAppPid;
}

void SystemTray::setUpdateAppPid(const QString &updateAppPid)
{
    m_updateAppPid = updateAppPid;
}

QString SystemTray::updateVersion() const
{
    return m_updateVersion;
}

void SystemTray::setUpdateVersion(const QString &updateVersion)
{
    m_updateVersion = updateVersion;
}

QString SystemTray::updateAppName() const
{
    return m_updateAppName;
}

void SystemTray::setUpdateAppName(const QString &updateAppName)
{
    m_updateAppName = updateAppName;
    setWindowTitle(QString(SYSTEMTRAY_TITLE).arg(updateAppName));
}

void SystemTray::startCheckedUpdate()
{
    initConfig();
    m_HandleCheckVersion.startCheckedUpdate(configFile().g_UpdateTime.toInt());
}

void SystemTray::handleUpdateVersion(const QString &updateAppExe)
{
    initConfig();
    //更新动画
    this->show();
    //解压更新文件包
    QTimer::singleShot(2000,[=]{
        //解压
        QString zipFile = QApplication::applicationDirPath()+SYSTEMTRAY_UPDATEDIR+"/"+this->m_HandleCheckVersion.getDownloadFileName();
        handleZip(zipFile,QApplication::applicationDirPath());
        //对比版本
        QString qstrFileName = QApplication::applicationDirPath()+"/"+updateAppExe;
        m_HandleCheckVersion.updateToVersionSetting(getFileVersion(qstrFileName));
        //启动程序
        qstrFileName=qstrFileName.replace('/',"\\");
        qDebug()<<__FUNCTION__<<qstrFileName<<ShellExecuteA(NULL, "runas", qstrFileName.toStdString().c_str() , NULL, NULL, SW_SHOWNORMAL);
        //
        QApplication::exit();
    });
}
