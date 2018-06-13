#include "common.h"
#include "WndMain.h"
#include "WndTrayIcon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WndMain w;
    WndTrayIcon wti(&a, &w);

    wti.show();
    w.show();

    return a.exec();
}
