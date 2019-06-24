#include "WndSetting.h"
#include "WndTrayIcon.h"
#include "BjutNet.h"
#include <QApplication>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QSettings>

WndSetting::WndSetting(WndTrayIcon *tray, QWidget *parent) :
    QWidget(parent),
    m_tray(tray)
{
    initUI();
    QWidget::setAttribute(Qt::WA_QuitOnClose,false);
    connect(m_chkAutoRun, &QCheckBox::clicked, this, &WndSetting::on_chkAutoRun_clicked);
    connect(m_btnApply, &QPushButton::clicked, this, &WndSetting::on_btnApply_clicked);
    connect(m_btnApplyLogin, &QPushButton::clicked, this, &WndSetting::on_btnApplyLogin_clicked);
}

void WndSetting::show()
{
    QWidget::show();

    QSettings autoRun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    m_chkAutoRun->setChecked(autoRun.contains("BjutNetLogin"));

    if(m_tray)
    {
        BjutNet *net = m_tray->getBjutNet();
        m_editAccount->setText(net->getAccount());
        m_cmbType->setCurrentIndex(net->getLoginType()-1);
    }
}

void WndSetting::on_chkAutoRun_clicked(bool checked)
{
#ifdef Q_OS_WIN
    QSettings autoRun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(checked)
    {
        autoRun.setValue("BjutNetLogin",
                         QApplication::applicationFilePath().replace(QChar('/'), QChar('\\'), Qt::CaseInsensitive)
                         +" -tray");
    }
    else
    {
        autoRun.remove("BjutNetLogin");
    }
    m_chkAutoRun->setChecked(autoRun.contains("BjutNetLogin"));
#else
    UNUSED(checked);
#endif
}

void WndSetting::on_chkDebug_clicked(bool checked)
{
    g_bAppDebug = checked;
}

void WndSetting::on_btnApply_clicked()
{
    if(m_tray)
    {
        BjutNet *net = m_tray->getBjutNet();
        net->setAccount(m_editAccount->text());
        net->setPassword(m_editPassword->text());
        net->setLoginType(m_cmbType->currentIndex() + 1);
        if(net->saveAccount())
        {
            QMessageBox::information(this, "保存账号","账号信息保存成功");
        }
    }
    else {
        QMessageBox::information(this, "保存账号","账号信息未保存");
    }
}

void WndSetting::on_btnApplyLogin_clicked()
{
    on_btnApply_clicked();
    if(m_tray)
    {
        m_tray->cmdLoginLgn();
    }
}
