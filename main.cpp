#include "common.h"
#include "WndMain.h"
#include "WndTrayIcon.h"
#include <QApplication>
#include <QtSingleApplication>

int main(int argc, char *argv[])
{
    QtSingleApplication a("APP_UUID_BjutNetLogin2", argc, argv);

    if(a.isRunning())
    {
        a.sendMessage("ShowMainWnd");
        return 0;
    }
    WndMain w;
    WndTrayIcon wti(&a, &w);

    QObject::connect(&a, &QtSingleApplication::messageReceived, &wti, &WndTrayIcon::reciveMessage);

    wti.show();

    //0为程序启动目录
    if(argc > 1 && 0==strcmp(argv[1], "-tray")){

    }
    else
    {
        w.show();
    }

    return a.exec();
}
