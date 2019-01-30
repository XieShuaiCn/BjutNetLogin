#include "common.h"
#include "WndMain.h"
#include "WndTrayIcon.h"
#include <QApplication>
#include <QtSingleApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QtSingleApplication app("APP_UUID_BjutNetLogin3", argc, argv);
#ifdef QT_DEBUG
    qDebug() << QtSingleApplication::applicationDirPath() << "\n"
             << QDir::currentPath() << "\n";
#endif

    if(app.isRunning())
    {
        app.sendMessage("ShowMainWnd");
        return 0;
    }
    QDir::setCurrent(app.applicationDirPath());
    WndMain w(&app);
    WndTrayIcon wti(&app, &w);

    QObject::connect(&app, &QtSingleApplication::messageReceived, &wti, &WndTrayIcon::reciveMessage);

    wti.show();

    //0为程序启动目录
    if(argc > 1 && 0==strcmp(argv[1], "-tray")){

    }
    else
    {
        w.show();
    }

    return app.exec();
}
