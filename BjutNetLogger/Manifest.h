#ifndef MANIFEST_H
#define MANIFEST_H

#include <QMap>
#include <QStringList>

class Manifest
{
public:
    typedef unsigned long long ulonglong;
    Manifest();
    bool loadFromFile(QString filePath);
    bool loadFromString(QString content);
    bool loadFromJson(QString json);
    bool saveFile(QString filePath);
    QStringList getList();
    //返回两个清单的差异项组成的新清单
    Manifest& operator ^(const Manifest &list);
    //返回当前清单项中不包含在list中的新清单
    Manifest& operator -(const Manifest &list);
    struct KeyValue{
        QString key;
        QString value;
    };
private:
    QMap<ulonglong, KeyValue> m_mapData;
};

#endif // MANIFEST_H
