// https://blog.rekols.com/2018/09/29/dbus-basis/
// http://www.cnblogs.com/wwang/archive/2010/10/27/1862552.html
#include "stdio.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qSetMessagePattern("[ %{file}: %{line} ] %{message}");

    // 日志
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString log = "[" + currentDateTime.toString("yyyy/MM/dd HH:mm:ss") + "] ";
//    for(int i=0; i<argc; i++){
//        log += " " + QString::fromLocal8Bit(argv[i]);
//    }
    QStringList args = QApplication::arguments();
    for(int i=0; i<args.length(); i++){
        log += " " + args.at(i);
    }
    log += "\n";
    QFile file(QApplication::applicationDirPath() + "/log.txt");
    if (file.open(QFile::WriteOnly|QIODevice::Append)) {
        file.write(log.toUtf8());
        file.close();
    }

    // 连接到 DBus 总线
    QDBusConnection dbus = QDBusConnection::sessionBus();
    // 如果注册成功则创建窗口(必须以 xxx.xxx 来指定 service name)
    if (dbus.registerService("com.hty.edit")) {
        MainWindow w;
        w.move(QApplication::desktop()->width()/2-w.width()/2, QApplication::desktop()->height()/2-w.height()/2);
        w.show();
        // 所有Slot都导出为这个Object的method
        dbus.registerObject("/hty/edit", &w, QDBusConnection::ExportAllSlots);
        return a.exec();
    }else{
        qDebug() << "QDBusInterface";
        QDBusInterface interface( "com.hty.edit", "/hty/edit", "com.hty.edit.registry", QDBusConnection::sessionBus());
        if (!interface.isValid()) {
            //qDebug() << qPrintable(QDBusConnection::sessionBus().lastError().message());
            qDebug() << QDBusConnection::sessionBus().lastError().message();
            exit(1);
        }
        // 呼叫远程
        qDebug() << "QDBusInterface.call(load(" << argv[1] << ")";
        QDBusReply<QString> reply = interface.call("load", QString::fromLocal8Bit(argv[1]));
//        QDBusReply<QString> reply;
//        QStringList args = QApplication::arguments();
//        if(args.length()>1){
//            reply = interface.call("load", args.at(1));
//            qDebug() << "QDBusInterface.call(load(" << args.at(1) << ")";
//        }
        if (reply.isValid()) {
            QString s = reply.value();
            qDebug() << s;
        } else {
            qDebug() << "QDBusReply is invalid!";
        }
        return 0;
    }
}