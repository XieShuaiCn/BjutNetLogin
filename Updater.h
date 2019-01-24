#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = nullptr);

    bool checkUpdate();
    bool needUpdate();

    const QString &getOldVersion() { return m_strOldVersion; }
    const QString &getNewVersion() { return m_strNewVersion; }
    int getOldVersionNum() { return m_nOldVersion; }
    int getNewVersionNum() { return m_nNewVersion; }
    QString getNewIntroduction() { return m_strNewIntroduction; }
protected:
    int getUrl(QUrl url, QString &content);
    int m_nNewVersion = 0;
    int m_nOldVersion = 0;
    QString m_strNewVersion;
    QString m_strOldVersion;
    QString m_strNewIntroduction;
    QString m_strOnlineFileURL;
    const QString m_strHostName = "http://bnl.hrrcn.com/";
};

#endif // UPDATER_H
