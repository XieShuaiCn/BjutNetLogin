#include "HPanel.h"
#include <QPainter>
#include <QFrame>
#include <QPaintEvent>
#include <QPen>

HPanel::HPanel(QWidget* parent, Qt::WindowFlags f) :
    QFrame(parent, f),
    m_bShowText(false),
    m_dBorderWidth(1.),
    m_dBorderRound(5.),
    m_colorBorder(QColor::fromRgb(180,180,180))
{
    m_lblText = new QLabel(this);
    m_lblText->setGeometry(QRect(25, 0, 100, 20));
}

void HPanel::setText(const QString &text)
{
    if(text.size())
    {
        const auto fm = m_lblText->fontMetrics();
        m_rectText = fm.boundingRect(text);
        m_bShowText = true;
    }
    else {
        m_bShowText = false;
    }
    m_lblText->setText(text);
}

void HPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // 设置反锯齿
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    {
        QPen pen(m_colorBorder);
        pen.setWidthF(m_dBorderWidth);
        painter.setPen(pen);
    }
    if(m_bShowText){
        double width = this->width();
        double height = this->height();
        double t_height_2 = 3+0.5*m_rectText.height();
        double t_width = m_rectText.width();

        painter.drawArc(QRectF(width-m_dBorderRound*2, t_height_2, m_dBorderRound*2, m_dBorderRound*2), 0.*16., 90.*16.);
        painter.drawArc(QRectF(0., t_height_2, m_dBorderRound*2, m_dBorderRound*2), 90.*16., 90.*16.);
        painter.drawArc(QRectF(0., height-m_dBorderRound*2-m_dBorderWidth, m_dBorderRound*2, m_dBorderRound*2), 180.*16., 90.*16.);
        painter.drawArc(QRectF(width-m_dBorderRound*2-m_dBorderWidth, height-m_dBorderRound*2-m_dBorderWidth, m_dBorderRound*2, m_dBorderRound*2), 270.*16., 90.*16.);

        painter.drawLine(QPointF(m_dBorderRound, t_height_2), QPointF(20., t_height_2));
        painter.drawLine(QPointF(30.+t_width, t_height_2), QPointF(width-m_dBorderRound, t_height_2));
        painter.drawLine(QPointF(width-m_dBorderWidth, t_height_2+m_dBorderRound), QPointF(width-m_dBorderWidth, height-m_dBorderRound));
        painter.drawLine(QPointF(m_dBorderRound, height-m_dBorderWidth), QPointF(width-m_dBorderRound, height-m_dBorderWidth));
        painter.drawLine(QPointF(0., t_height_2+m_dBorderRound), QPointF(0., height-m_dBorderRound));
    }
    else {
        painter.drawRoundedRect(QRectF(0., 10., this->width()-m_dBorderWidth, this->height()-m_dBorderWidth-10.), m_dBorderRound, m_dBorderRound);
    }
}
