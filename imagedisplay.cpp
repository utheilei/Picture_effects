#include "imagedisplay.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>

#define ROTATE_ANGLE 180

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{
    m_roateAnimation = new QPropertyAnimation(this, "angle");
    m_roateAnimation->setDuration(1500);
    m_roateAnimation->setStartValue(0);
    m_roateAnimation->setEndValue(ROTATE_ANGLE);

    connect(m_roateAnimation, &QPropertyAnimation::valueChanged,
            this, &ImageWidget::animationValueChanged);
}

void ImageWidget::setPixmap(const QPixmap &pixmap)
{
    if (m_pixmap != pixmap) {
        m_pixmap = pixmap;
        update();
    }
}

void ImageWidget::setNextPixmap(int index)
{
    m_nextPixmap = m_lstImages[index];
}

QPixmap ImageWidget::pixmap() const
{
    return m_pixmap;
}

void ImageWidget::scalePixmap(const QPixmap& pixmap,int nMaxWidth,int nMaxHeight)
{
    m_pixmap = pixmap.scaled(nMaxWidth, nMaxHeight,
                             Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void ImageWidget::initImageList(const QStringList &strImagePaths)
{
    m_lstImages.clear();

    foreach(QString str,strImagePaths)
    {
        m_lstImages.append(QPixmap(str));
    }
}

void ImageWidget::start()
{
    if (m_roateAnimation->state() == QAbstractAnimation::Stopped)
        m_roateAnimation->start();
}

void ImageWidget::stop()
{
    if (m_roateAnimation->state() == QAbstractAnimation::Running)
        m_roateAnimation->stop();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (m_pixmap.width() != width() || m_pixmap.height() != height())
        scalePixmap(m_pixmap, width(), height());

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect(),QPixmap(":/images/back.jpg"));

    QTransform transform;
    transform.translate(rect().center().x(), rect().center().y());
    transform.rotate(m_dRoateAngle,Qt::Axis::YAxis);
    painter.setTransform(transform);
    painter.drawPixmap(-width()/2, -height()/2, m_pixmap);

//    //画倒影
//    painter.setOpacity(0.2);
//    transform.scale(1,-1);
//    painter.setTransform(transform);
//    painter.drawPixmap(0, int(-m_pixmap.height()*2.05), m_pixmap);
    //    painter.resetTransform();
}

void ImageWidget::animationValueChanged(const QVariant &value)
{
    if (value.toInt() < 90) {
        m_dRoateAngle = value.toInt();
        update();
    } else if(90 == value.toInt()) {
        m_dRoateAngle = -90;
        setPixmap(m_nextPixmap);
    } else if(value.toInt() > 90) {
        m_dRoateAngle = value.toInt() - ROTATE_ANGLE;
        update();
    } else if (ROTATE_ANGLE == value.toInt()) {
        m_dRoateAngle = 0;
        emit animationFinished();
    }
}
