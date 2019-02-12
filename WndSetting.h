#ifndef WNDSETTING_H
#define WNDSETTING_H

#include <QWidget>

class WndSetting : public QWidget
{
    Q_OBJECT
public:
    explicit WndSetting(QWidget *parent = nullptr);

signals:

public slots:
    void initUI();
};

#endif // WNDSETTING_H
