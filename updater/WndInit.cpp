#include "WndInit.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QProcess>

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

WndInit::WndInit(QApplication *app)
    : QProgressDialog("正在初始化更新", "取消", 0, 100, nullptr, Qt::Tool),
      m_bCancel(false),
      m_app(app)
{
    Q_ASSERT(app!=nullptr);
    m_strBinName = app->applicationName();
    m_strBinDir = app->applicationDirPath();
    m_strBinFile = QDir(m_strBinDir).absoluteFilePath(m_strBinName);
    this->setWindowTitle("网关登录器 更新");
    connect(this, &QProgressDialog::canceled, this, &WndInit::cancel_init);
    connect(&m_tmInit, &QTimer::timeout, this, &WndInit::init_updater);
}

void WndInit::cancel_init()
{
    m_bCancel = true;
}

void WndInit::show()
{
    QProgressDialog::show();
    m_tmInit.setSingleShot(true);
    m_tmInit.setInterval(10);
    m_tmInit.start();
}

void WndInit::init_updater()
{
    m_tmInit.stop();
    // create temp file.
    QString tmpName = QDir::temp().absoluteFilePath("BjutNetLogin_updater_.tmp");
    for(int i =0; i < 30; ++i)
    {
        QString rs = RandString(6);
        tmpName = QDir::temp().absoluteFilePath("BjutNetLogin_updater_" + rs + ".tmp");
        QDir tmpDir(tmpName);
        if(tmpDir.exists()){
            if(!tmpDir.rmpath(tmpName) && i > 10)
                break;
        }
        if(!tmpDir.exists()){
            break;
        }
    }
    if(QDir("/").mkpath(tmpName))
    {
        this->setValue(40);
    }
    else
    {
        this->setLabelText("临时目录创建失败");
    }
    m_strTmpDir = tmpName;
    m_strTmpFile = QDir(m_strTmpDir).absoluteFilePath(m_strBinName);
    // copy file
    if(QFile::copy(m_strBinFile, m_strTmpFile))
    {
        this->setValue(80);
    }
    else
    {
        this->setLabelText("更新初始化失败");
    }

    QStringList args;
    args.push_back("-update");
    args.push_back("-dir");
    args.push_back(m_strBinDir);
    if(QProcess::startDetached(m_strTmpFile, args, m_strBinDir))
    {
        this->setValue(100);
        m_app->exit(0);
    }
    else
    {
        this->setLabelText("更新启动失败");
    }
}
