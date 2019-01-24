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

#endif // COMMON_H
