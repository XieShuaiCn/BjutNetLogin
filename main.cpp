#include "common.h"
#include "WndTrayIcon.h"
#include <QtSingleApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QtSingleApplication app("APP_UUID_BjutNetLogin3", argc, argv);
    if(app.isRunning())
    {
        app.sendMessage("ShowMainWnd");
        return 0;
    }
#ifdef QT_DEBUG
    qDebug() << QtSingleApplication::applicationDirPath() << endl
             << QDir::currentPath() << endl;
#endif
    QDir::setCurrent(app.applicationDirPath());

    WndTrayIcon wti(&app);
    wti.show();
    if(argc > 1 && 0==strcmp(argv[1], "-tray")){   }
    else
    {
        wti.cmdShowMainWnd();
    }
    //wti.cmdShowSettingWnd();
    QObject::connect(&app, &QtSingleApplication::messageReceived, &wti, &WndTrayIcon::reciveMessage);
    return app.exec();
}
