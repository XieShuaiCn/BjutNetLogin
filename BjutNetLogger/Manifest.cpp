#include "Manifest.h"

Manifest::Manifest()
{
}

QStringList Manifest::getList()
{
    return QStringList(m_mapData.values());
}


bool Manifest::loadFromFile(QString filePath)
{}

bool Manifest::loadFromString(QString content)
{}

bool Manifest::loadFromJson(QString json)
{}

bool Manifest::saveFile(QString filePath)
{}

Manifest Manifest::operator ^(const Manifest &list)
{
    Manifest newLst(list);
    QMap<ulonglong, KeyValue> &newData = newLst.m_mapData;
    //遍历两个列表，去除新列表中的原来已存在项
    for(auto it = m_mapData.cbegin(), itend = m_mapData.cend();
        it != itend; ++it) {
        auto newIt = newData.find(it.key());
        //找到对应项
        if(newIt != newData.cend()) {
            //删除相同项
            if(newIt.value().key == it.value().key) {
                newData.erase(newIt);
            }
        }
        else {
            //没找到则插入新项
            newData.insert(it.key(), it.value());
        }
    }
    return newLst;
}


Manifest& Manifest::operator -(const Manifest &list)
{
    Manifest newLst(*this);
    QMap<ulonglong, KeyValue> &newData = newLst.m_mapData;
    //遍历两个列表，去除新列表中的原来已存在项
    for(auto it = list.m_mapData.cbegin(), itend = list.m_mapData.cend();
        it != itend; ++it) {
        auto newIt = newData.find(it.key());
        //找到对应项
        if(newIt != newData.cend()) {
            //删除相同项
            if(newIt.value().key == it.value().key) {
                newData.erase(newIt);
            }
        }
    }
    return newLst;
}
