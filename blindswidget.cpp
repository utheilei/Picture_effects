#include "blindswidget.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>

BlindsWidget::BlindsWidget(QWidget *parent) : QWidget(parent)
{
    m_roateAnimation = new QPropertyAnimation(this, "m_factor");
    m_roateAnimation->setDuration(1500);
    m_roateAnimation->setStartValue(0.0);
    m_roateAnimation->setEndValue(1.0);

    connect(m_roateAnimation, &QPropertyAnimation::valueChanged,
            this, &BlindsWidget::animationValueChanged);
}

void BlindsWidget::setPrePixmap(const QPixmap &pixmap)
{
    m_pixamp1 = pixmap;
    update();
}

void BlindsWidget::setNextPixmap(const QPixmap &pixmap)
{
    m_pixamp2 = pixmap;
    update();
}

void BlindsWidget::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;
}

void BlindsWidget::setFactor(float factor)
{
    m_factor = factor;
}

float BlindsWidget::factor()
{
    return m_factor;
}

void BlindsWidget::start()
{
    if (m_roateAnimation->state() == QAbstractAnimation::Stopped)
        m_roateAnimation->start();
}

void BlindsWidget::stop()
{
    if (m_roateAnimation->state() == QAbstractAnimation::Running)
        m_roateAnimation->stop();
}

void BlindsWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (m_pixamp1.isNull() || m_pixamp2.isNull()) {
        return;
    }

    if (m_pixamp1.width() != width() || m_pixamp1.height() != height())
        m_pixamp1 = scalePixmap(m_pixamp1, width(), height());

    if (m_pixamp2.width() != width() || m_pixamp2.height() != height())
        m_pixamp2 = scalePixmap(m_pixamp2, width(), height());

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    int i, n, w, h, x1, y1, x2, y2, dh, ddh;

    w = width();
    h = height();
    x1 = (w - m_pixamp1.width()) / 2;
    y1 = (h - m_pixamp1.height()) / 2;
    x2 = (w - m_pixamp2.width()) / 2;
    y2 = (h - m_pixamp2.height()) / 2;

    painter.drawPixmap(x1, y1, m_pixamp1);

    n = 10;

    if (Qt::Vertical == m_orientation) {
        dh = m_pixamp2.height() / n;
        ddh = m_factor * dh;

        if (ddh < 1)
            return;

        for(i = 0; i < n; i++) {
            painter.drawPixmap(x2, y2 + i * dh, m_pixamp2, 0, i * dh, m_pixamp2.width(), ddh);
        }
    } else {
        dh = m_pixamp2.width() / n;
        ddh = m_factor * dh;

        if (ddh < 1)
            return;

        for(i = 0; i < n; i++) {
            painter.drawPixmap(x2 + i * dh, y2, m_pixamp2, i * dh, 0, ddh, m_pixamp2.height());
        }
    }
}

QPixmap BlindsWidget::scalePixmap(const QPixmap &pixmap, int nMaxWidth, int nMaxHeight)
{
    return pixmap.scaled(nMaxWidth, nMaxHeight,
                             Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void BlindsWidget::animationValueChanged(const QVariant &value)
{
    update();
}
