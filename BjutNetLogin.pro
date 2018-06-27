#-------------------------------------------------
#
# Project created by QtCreator 2018-05-31T15:21:46
#
#-------------------------------------------------

QT       += core \
            gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BjutNetLogin
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


RC_FILE += version.rc
#RC_ICONS = logo.ico

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = common.h


SOURCES += \
    main.cpp \
    common.cpp \
    BjutNet.cpp \
    WndMain.cpp \
    WndTrayIcon.cpp

HEADERS += \
    common.h \
    BjutNet.h \
    WndMain.h \
    WndTrayIcon.h

DISTFILES += \
    C:/Users/XIESHUAI/.bjutnet/account.json \
    /home/xieshuai/.bjutnet/account.json \
    ReadMe.txt \
    version.rc

RESOURCES += \
    all.qrc

include(qtsingleapplication/src/qtsingleapplication.pri)
