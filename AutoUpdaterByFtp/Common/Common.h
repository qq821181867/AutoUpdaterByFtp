#ifndef COMMON_H
#define COMMON_H
/*!
*@file Common.h
*@brief
*
* 系统使用到公共函数
*
*@author shengjian.chen
*@version s/n:20150807001
*@date 2015.08.07
*/
#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <QTranslator>
#include <Windows.h>
class UseApplication;
class KeyPressEater;

class Common
{
public:
    Common();
    /**
     * @brief openSystemLogWriteFiqle 开启输出日志到文件中
     */
    static void openSystemLogWriteFile();
    /**
     * @brief setSystemDefaultCode 设置系统默认编码
     */
    static void setSystemDefaultCode();
    /**
     * @brief getSystemDirPath 获取系统程序所在目录
     * @return 返回系统程序所在目录
     */
    static QString getSystemDirPath();
    /**
     * @brief ApplicationRunOne 限制应用程序只能运行一次
     * @param customApp 自定义应用对象
     * @return 是否已经运行
     */
    static bool ApplicationRunOne(QApplication &customApp);
    /**
     * @brief loadSystemIcon 加载应用程序图标
     * @param customApp 自定义应用对象
     */
    static void loadSystemIcon(QApplication &customApp);
    /**
     * @brief loadQSSToSystem 加载Qss样式到系统程序中
     */
    static void loadQSSToSystem();
    /**
     * @brief messageBoxCustomWarning 弹出提示框
     * @param parent 父窗口指针
     * @param title  标题名称
     * @param msg      消息内容
     * @param buttons  按钮类型
     * @param defaultButton 默认选中按钮
     * @return 消息选中按钮值
     */
    static int messageBoxCustomWarning(QWidget * parent, QString title, QString msg, QMessageBox::StandardButtons buttons=QMessageBox::Ok, QMessageBox::StandardButton defaultButton=QMessageBox::Yes);
    /**
     * @brief getLocalIPToString 获取本机ip地址
     * @return ip地址
     */
    static QString getLocalIPToString();
    /**
     * @brief getLocalMACToString 获取本机Mac地址
     * @return  mac地址
     */
    static QString getLocalMACToString();
    /**
     * @brief logDebug 打印日志输出
     * @param fileName 文件名
     * @param functionName 函数名称
     * @param strLogText 日志信息内容
     */
    static void logDebug(const QString& strLogText, const QString & fileName ,const int & line, const QString & functionName);
    /**
     * @brief imageToBase64 图片转base64
     * @param image         转换图片
     * @param qstrFormat    图片格式
     * @return              图片base64数据流
     */
	static QByteArray imageToBase64(const QImage &image, const int &nQuality = 100 , const QString &qstrFormat = "jpg");
    /**
     * @brief base64Toimage base64转图片
     * @param byteArrayBase64   转换base64
     * @return              图片
     */
    static QImage base64Toimage(const QByteArray &byteArrayBase64);

    //开启系统错误监控
    enum emOpenErrorHandleType{
        Default=0,
        Normal,
        RestartApp
    };
    static void openErrorReturn(emOpenErrorHandleType type);
    static LONG WINAPI handleDefaultExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs);
    static LONG WINAPI handleNormalExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs);
    static LONG WINAPI handleRestartAppExceptionInfo(PEXCEPTION_POINTERS pExceptPtrs);

private:

};

#endif // COMMON_H
