#include "SystemTray.h"
#include <QApplication>
#include "Common/Common.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Common::openSystemLogWriteFile();
    //
    SystemTray systemTray;

    QString updateAppExe;
    QString updateAppName;
    QString updateVersion;
    QString updateAppId;
    if(argc==4)
    {
        updateAppName+=argv[1];
        updateVersion+=argv[2];
        updateAppId+=argv[3];
        //
        systemTray.setUpdateAppName(updateAppName);
        systemTray.setUpdateVersion(updateVersion);
        systemTray.setUpdateAppPid(updateAppId);
        //
        systemTray.startCheckedUpdate();
    }
    else if(argc==2)
    {
        updateAppExe+=argv[1];
        systemTray.handleUpdateVersion(updateAppExe);
    }
    else
    {
        QTimer::singleShot(0,[]{
            QApplication::exit();
        });
    }
    //
    return a.exec();
}
