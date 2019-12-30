# AutoUpdaterByFtp
使用方法：
方法一：编译对应版本程序，将程序放置所需要更新的程序目录一起打包
方法二：使用项目内部iss文件打包整个完整程序，进行可控制化模块安装

#AutoUpdaterByFtpControl
启动AutoUpdaterByFtp.exe使用方法
在程序启动，界面初始化前：
m_HandleAutoUpdater.startHandle();
注释：该操作内部会自动检测版本是否已下载新版本，如下载新版本则退出主程序，使用AutoUpdaterByFtp.exe解压更新文件，并启动；
否则启动AutoUpdaterByFtp.exe进行定时的版本更新。
