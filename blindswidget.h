#ifndef BLINDSWIDGET_H
#define BLINDSWIDGET_H

#include <QWidget>

class QPropertyAnimation;

class BlindsWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float m_factor READ factor WRITE setFactor)

public:
    explicit BlindsWidget(QWidget *parent = nullptr);

    void setPrePixmap(const QPixmap &pixmap);

    void setNextPixmap(const QPixmap &pixmap);

    void setOrientation(Qt::Orientation orientation);

    void setFactor(float factor);

    float factor();

    void start();

    void stop();

protected:
    void paintEvent(QPaintEvent *event) override;

    QPixmap scalePixmap(const QPixmap &pixmap,int nMaxWidth,int nMaxHeight);

signals:

public slots:
    void animationValueChanged(const QVariant &value);

private:
    QPropertyAnimation *m_roateAnimation;
    QPixmap m_pixamp1;
    QPixmap m_pixamp2;
    float m_factor = 0.0;
    Qt::Orientation m_orientation = Qt::Vertical;
};

#endif // BLINDSWIDGET_H
