#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QException>

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#define PROPERTY_READ(type, name, var) \
    type get##name() const { return (var); }

#define PROPERTY_READ_CONST(type, name, var) \
    const type &get##name() const { return (var); }

#define PROPERTY_WRITE(type, name, var) \
    void set##name(type value) { var = value; }

#define PROPERTY_READ_WRITE(type, name, var) \
    PROPERTY_READ(type, name, var) \
    PROPERTY_WRITE(type, name, var)

extern bool g_bAppDebug;
extern QString g_strAppTempPath;

QString RandString(int length);

void InitDebugFile(const QString &name);

void WriteDebugInfo(const QString &content, bool with_time = true, bool end_line = true);

void WriteDebugInfo(const QString &status, const QString &content, bool with_time = true, bool end_line = true);

void ReleaseDebugFile();

#endif // COMMON_H
