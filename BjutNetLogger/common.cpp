#include "common.h"
#include <QFile>
#include <QTime>

bool g_bAppDebug = false;
QString g_strAppTempPath;
QFile g_fileDebug;
bool g_bDbgFileOpened = false;


QString RandString(int length)
{
    qsrand(QTime::currentTime().msec()
           +QTime::currentTime().second()*1000
           +QTime::currentTime().minute()*60000);
    const QString set("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    QString result;
    for(int i = 0; i < length; ++i) {
        result.append(set[qrand() % 36]);
    }
    return result;
}

void InitDebugFile(const QString &name)
{
    g_fileDebug.setFileName(name);
    g_fileDebug.open(QFile::ReadWrite);
    g_bDbgFileOpened = g_fileDebug.isOpen();
}

void ReleaseDebugFile()
{
    if(g_fileDebug.isOpen()) g_fileDebug.close();
}

void WriteDebugInfo(const QString &status, const QString &content, bool with_time, bool end_line)
{
    WriteDebugInfo(QString("[%1] %2").arg(status).arg(content), with_time, end_line);
}

void WriteDebugInfo(const QString &content, bool with_time, bool end_line)
{
    if(!g_fileDebug.isOpen())
        return;
    if (with_time)
    {
        QDateTime time = QDateTime::currentDateTime();
        QString strCurrentTime = time.toString("[yyyy-MM-dd hh:mm:ss:zzz]");
        QByteArray tempData = strCurrentTime.toUtf8();
        g_fileDebug.write(tempData.data());
    }
    {
        QByteArray tempData = content.toUtf8();
        g_fileDebug.write(tempData.data());
    }
    if(end_line)
    {
        g_fileDebug.write("\n");
    }
}
