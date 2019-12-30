#include "Common.h"
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QIcon>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QBuffer>
#include <QAbstractButton>
#include <QDateTime>
#include <QFile>
#include <QProcess>

//日志消息输出函数设置 4.x
void customMessageHandler(QtMsgType type, const char *msg);
void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type)
    {
        //调试信息提示
        case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
            break;
            //一般的warning提示
        case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
            break;
            //严重错误提示
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
            break;
            //致命错误提示
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            abort();
    }
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2").arg(txt).arg(current_date);

    #ifdef _DEBUG
        QString filename= QCoreApplication::applicationDirPath()+QString("/log/debug_log%1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    #else
        QString filename= QCoreApplication::applicationDirPath()+QString("/log/release_log%1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    #endif

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
}
//日志消息输出函数设置 5.x
void customMessageHandler5(QtMsgType type, const QMessageLogContext &context , const QString &msg);
void customMessageHandler5(QtMsgType type, const QMessageLogContext &context , const QString &msg)
{
    QString txt;
    switch (type)
    {
        //调试信息提示
        case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
            break;
            //一般的warning提示
        case QtWarningMsg:
            return;
//            txt = QString("Warning: %1").arg(msg);
            break;
            //严重错误提示
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
            break;
            //致命错误提示
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            abort();
    }
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2").arg(txt).arg(current_date);
    message = QString("File:%1,%2 Fun:%3").arg(context.file).arg(context.line).arg(context.function)+",Msg:"+message;

    #ifdef _DEBUG
        QString filename= QCoreApplication::applicationDirPath()+"/"+QApplication::applicationName()+QString("log/debug_log%1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    #else
        QString filename= QCoreApplication::applicationDirPath()+"/"+QApplication::applicationName()+QString("log/release_log%1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    #endif

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
}
//
Common::Common()
{

}

void Common::openSystemLogWriteFile()
{
#ifndef _DEBUG
   qInstallMessageHandler(QtMessageHandler(customMessageHandler5));
#endif
}

void Common::setSystemDefaultCode()
{
    //设置程序编码
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
}

QString Common::getSystemDirPath()
{
   QString qstrResult =  QString("%1/").arg(QCoreApplication::applicationDirPath());
   return qstrResult;
}

//限制应用程序只运行一次
bool Common::ApplicationRunOne(QApplication &customApp){
    #ifdef APP_INSTANCE_MUTEX
    //判断系统是否已经运行
    if (!customApp.DoCheckAppInstance())
    {
        messageBoxCustomWarning(NULL,QObject::tr(SYSTEM_TIP_TITLE),QObject::tr(SYSTEM_TIP_NONEAPP));
        customApp.exit(1);
        return true;
    }
    customApp.CreateAppMutex();
    #endif//APP_INSTANCE_MUTEX
    return false;
}

void Common::loadSystemIcon(QApplication &customApp)
{
    customApp.setWindowIcon(QIcon(getSystemDirPath()+"app.ico"));
}

void Common::loadQSSToSystem(){
    QString qss;
    QFile qssFile(getSystemDirPath()+"app.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QString::fromUtf8(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }
}

//消息显示
int Common::messageBoxCustomWarning(QWidget * parent, QString title, QString msg,
                                    QMessageBox::StandardButtons buttons,
                                    QMessageBox::StandardButton defaultButton)
{
    QMessageBox msgBox(QMessageBox::Warning, title, msg, 0, parent);
    msgBox.setStandardButtons(buttons);
    msgBox.setDefaultButton(defaultButton);
    if(NULL!=msgBox.button(QMessageBox::Ok))
    {
        msgBox.button(QMessageBox::Ok)->setText("确 定");
    }

    if(NULL!=msgBox.button(QMessageBox::Yes))
    {
        msgBox.button(QMessageBox::Yes)->setText("确 定");
    }

    if(NULL!=msgBox.button(QMessageBox::No))
    {
        msgBox.button(QMessageBox::No)->setText("关 闭");
    }
    return msgBox.exec();
}

//获取本机ip地址
QString Common::getLocalIPToString(){
    QString ip= "";
    QString localHOSTName =QHostInfo::localHostName();
    QHostInfo hostinfo = QHostInfo::fromName(localHOSTName);
    QList<QHostAddress> list = hostinfo.addresses();
    foreach(QHostAddress qHostAddress,list)
    {
        if(!qHostAddress.isNull() &&  qHostAddress!=QHostAddress::LocalHost && qHostAddress.protocol() == QAbstractSocket::IPv4Protocol)	//只选择IPv4地址
        {//
            //修改代码 2016/03/11 lys
            QString tmp = qHostAddress.toString();
            if(tmp!=""){//
                ip = tmp;
                break;
            }
        }
    }
    //加入代码 20160314 lys
    list = hostinfo.addresses();
    foreach(QHostAddress qHostAddress,list)
    {
        if(!qHostAddress.isNull() &&  qHostAddress!=QHostAddress::LocalHost && qHostAddress.protocol() == QAbstractSocket::IPv6Protocol)	//只选择IPv4地址
        {//
            //修改代码 2016/03/11 lys
            QString tmp = qHostAddress.toString();
            if(tmp!=""){//
                ip = ip + "|" + tmp;
                break;
            }
        }
    }
    return ip;
}
//获取本机MAC地址
QString Common::getLocalMACToString(){
    //
    QString mac;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface qNetworkInterface, list){
        if( qNetworkInterface.isValid()
            &&qNetworkInterface.flags().testFlag(QNetworkInterface::IsUp)
            &&qNetworkInterface.flags().testFlag(QNetworkInterface::IsRunning)
            &&qNetworkInterface.flags().testFlag(QNetworkInterface::CanBroadcast)
            &&qNetworkInterface.flags().testFlag(QNetworkInterface::CanMulticast)
            &&!qNetworkInterface.flags().testFlag(QNetworkInterface::IsLoopBack)){
                mac = qNetworkInterface.hardwareAddress();
                if(!mac.isEmpty() &&  qNetworkInterface.humanReadableName() =="本地连接"){
                    break;
                }
        }
    }
    return mac;
}

void Common::logDebug(const QString &strLogText, const QString &fileName, const int &line, const QString &functionName)
{
    qDebug()<<QString(fileName).split("\\").last()+"("+QString::number(line)+")"<<"->"<<functionName<<"->"<<strLogText;
}

QByteArray Common::imageToBase64(const QImage &image,const int &nQuality,const QString &qstrFormat)
{
    if(image.isNull()){
      return QByteArray();
    }
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, qstrFormat.toStdString().c_str(), nQuality);
    buffer.close();
    return imageData.toBase64();
}



QImage Common::base64Toimage(const QByteArray &byteArrayBase64)
{
    return QImage::fromData(QByteArray::fromBase64(byteArrayBase64));
}

void Common::openErrorReturn(emOpenErrorHandleType type)
{
#ifndef _DEBUG
    switch (type)
    {
        case emOpenErrorHandleType::Normal:
        {
            SetUnhandledExceptionFilter(Common::handleNormalExceptionInfo);
        }
        break;
        case emOpenErrorHandleType::RestartApp:
        {
            SetUnhandledExceptionFilter(Common::handleRestartAppExceptionInfo);
        }
        break;
        default:
            SetUnhandledExceptionFilter(Common::handleDefaultExceptionInfo);
        break;
    }

	SetErrorMode(SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
#endif
}

LONG WINAPI Common::handleDefaultExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs)
{
    return EXCEPTION_EXECUTE_HANDLER;
}

LONG Common::handleNormalExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs)
{
    qDebug()<<__FUNCTION__<<pExceptPtrs->ExceptionRecord->ExceptionCode;
    //
    QString qstrText = "系统发生未知异常,确认后重启";
    QMessageBox::warning(NULL,"系统错误",qstrText);
    return EXCEPTION_EXECUTE_HANDLER;
}

LONG Common::handleRestartAppExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs)
{
    long result = handleNormalExceptionInfo(pExceptPtrs);
    QString qstrFileName = QApplication::applicationFilePath();
    qstrFileName=qstrFileName.replace('/',"\\");
    qDebug()<<__FUNCTION__<<qstrFileName<<"restart"<<ShellExecuteA(NULL, "open", qstrFileName.toStdString().c_str() , NULL, NULL, SW_SHOWNORMAL);
    return result;
}
