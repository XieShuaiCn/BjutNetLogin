#include "BjutNet.h"
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>

BjutNet::BjutNet(WndMain *mainWindow)
{
    m_wndMain = mainWindow;
    connect(&m_webLgn, &WebLgn::message, this, &BjutNet::message);
    connect(&m_webJfself, &WebJfself::message, this, &BjutNet::message);
}


bool BjutNet::loadAccount(const QString path)
{
    QFileInfo *fi;
    if(path.length() == 0)
    {
        QString conf_file_home = (QDir::homePath() + "/.bjutnet/account.json");
        fi = new QFileInfo(conf_file_home);
        if(!fi->exists())
        {
#ifndef Q_OS_WIN
            delete fi;
            QString conf_file_etc = "/etc/bjutnet.d/account.json";
            fi = new QFileInfo(conf_file_etc);
            if(!fi->exists())
            {
#endif
                delete fi;
                QString conf_file_model = (QDir::currentPath() + "/account.json");
                fi = new QFileInfo(conf_file_model);
                if(!fi->exists())
                {
                    emit message(QDateTime::currentDateTime(), "cannot find any configure files.\n");
                    delete fi;
                    return false;
                }
#ifndef Q_OS_WIN
            }
#endif
        }
    }
    else
    {
        fi = new QFileInfo(path);
    }
    if(!fi->exists())
    {
        emit message(QDateTime::currentDateTime(), "cannot find the configure file("+fi->fileName()+").\n");
        delete fi;
        return false;
    }
    if(fi->isReadable())
    {
        QFile f(fi->filePath());
        delete fi;
        f.open(QFile::ReadOnly);
        QJsonParseError jp_err;
        const QJsonDocument jd = QJsonDocument::fromJson(f.readAll(), &jp_err);
        f.close();
        if(jp_err.error == QJsonParseError::NoError)
        {
            const QJsonObject jo = jd.object();
            if(jo.contains("account"))
            {
                m_strAccount = jo["account"].toString();
            }
            if(jo.contains("password"))
            {
                m_strPassword = jo["password"].toString();
            }
            if(jo.contains("type"))
            {
                m_loginType = WebLgn::LoginType(jo["type"].toInt()+1);
            }
            if(jo.contains("ipv4"))
            {
                m_loginType = WebLgn::LoginType(m_loginType & ((jo["ipv4"].toInt() % 2) * int(WebLgn::IPv4)));
            }
            if(jo.contains("ipv6"))
            {
                m_loginType = WebLgn::LoginType(m_loginType & ((jo["ipv6"].toInt() % 2) * int(WebLgn::IPv6)));
            }
            synchronizeAccount();
            return true;
        }
        else{
            emit message(QDateTime::currentDateTime(), jp_err.errorString());
            return false;
        }
    }
    return false;
}

bool BjutNet::saveAccount(const QString path)
{
    QFile f(path);
    QDir d;
    if(path.length() == 0)
    {

        QString conf_file_home = (QDir::homePath() + "/.bjutnet/account.json");
        f.setFileName(conf_file_home);
        if(!f.exists())
        {
#ifndef Q_OS_WIN
            QString conf_file_etc = "/etc/bjutnet.d/account.json";
            f.setFileName(conf_file_etc);
            //etc没有写入权限
            if(!f.exists() || !f.open(QFile::WriteOnly) || !f.isWritable())
            {
#endif
                QString conf_file_model = (QDir::currentPath() + "/account.json");
                f.setFileName(conf_file_model);
                if(!f.exists() || !f.open(QFile::WriteOnly) || !f.isWritable())
                {
                    //不存在，则创建目录
                    d.setPath(QDir::homePath());
                    d.mkdir(".bjutnet");
                    f.setFileName(conf_file_home);
                }
#ifndef Q_OS_WIN
            }
#endif
        }
    }
    if(f.isOpen() || f.open(QFile::WriteOnly))//文件已打开或打开成功
    {
        QJsonObject jo;
        jo.insert("account", m_strAccount);
        jo.insert("password", m_strPassword);
        jo.insert("type", int(m_loginType) - 1);
        QJsonDocument jd;
        jd.setObject(jo);
        QByteArray data= jd.toJson(QJsonDocument::Compact);
        f.write(data);
        f.close();
        return true;
    }
    return false;
}

void BjutNet::synchronizeAccount()
{
    m_webLgn.setAccount(m_strAccount);
    m_webLgn.setPassword(m_strPassword);
    m_webLgn.setLoginType(m_loginType);
    m_webJfself.setAccount(m_strAccount);
    m_webJfself.setPassword(m_strPassword);
}
