#-------------------------------------------------
#
# Project created by QtCreator 2019-08-21T10:03:20
#
#-------------------------------------------------

QT       += core gui network gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoUpdaterByFtp
CONFIG(debug,debug|release){
    DESTDIR = $$OUT_PWD/../bin/Debug
}else{
    DESTDIR = $$OUT_PWD/../bin/Release
}
TEMPLATE = app

CONFIG+=precompile_header
PRECOMPILED_HEADER=Common/stable.h


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        SystemTray.cpp \
    FtpManager.cpp \
    Common/Common.cpp \
    Database/Config/ConfigFile.cpp \
    HandleModel/HandleCheckVersion.cpp \
    HandleModel/HandleDownloadFile.cpp

HEADERS += \
        SystemTray.h \
    FtpManager.h \
    Common/Common.h \
    Database/Config/ConfigFile.h \
    HandleModel/HandleCheckVersion.h \
    HandleModel/HandleDownloadFile.h

FORMS += \
        SystemTray.ui

RESOURCES += \
    Resources/res.qrc
