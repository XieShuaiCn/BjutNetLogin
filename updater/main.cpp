#include <QObject>
#include <QDebug>
#ifdef QT_DEBUG
#include <QDir>
#endif
#include <QApplication>
#include "WndMain.h"
#include "WndInit.h"

int main(int argc, char *argv[])
{
    QMap<QString, QString> mapArgv;
    {
        QString lastArg;
        for(int i = 1; i < argc; ++i)
        {
            if(argv[i][0] == '-') //key or option, insert temporarily
            {
                QString tmp(argv[i]);
                lastArg = tmp.right(tmp.size()-1);
                mapArgv.insert(lastArg, "");
            }
            else //set value
            {
                if(lastArg.size() && mapArgv.contains(lastArg)){
                    mapArgv[lastArg] = QString(argv[i]);
                    lastArg.clear();
                }else {
                    qDebug() << "Error parameter: Unexpected '" << argv[i] << "' after '" << argv[i-1] << "'.";
                    return 0;
                }
            }
        }
    }

    QApplication app(argc, argv);
#ifdef QT_DEBUG
    qDebug() << QDir::currentPath() << endl;
    qDebug() << app.applicationDirPath() << endl;
    qDebug() << app.applicationDisplayName() << endl;
    qDebug() << app.applicationFilePath() << endl;
    qDebug() << app.applicationName() << endl;
    qDebug() << app.applicationPid() << endl;
    qDebug() << app.applicationVersion() << endl;
#endif
    if(mapArgv.contains("update"))
    {
        WndMain wm;
        wm.show();
        return app.exec();
    }
    else {
        WndInit wi(&app);
        wi.show();
        return app.exec();
    }
}
