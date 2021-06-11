#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H

#include <QWidget>

class QPropertyAnimation;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);

    void setPixmap(const QPixmap &pixmap);

    void setNextPixmap(int index);

    QPixmap pixmap() const;

    void scalePixmap(const QPixmap &pixmap,int nMaxWidth,int nMaxHeight);

    void initImageList(const QStringList &strImagePaths);

    void start();

    void stop();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void animationFinished();

public slots:
    void animationValueChanged(const QVariant &value);

private:
    QPropertyAnimation *m_roateAnimation;
    QPixmap m_nextPixmap;
    QPixmap m_pixmap;
    QList<QPixmap> m_lstImages;
    int m_nOpacity = 255;
    int m_dRoateAngle = 0;
};

#endif // IMAGEDISPLAY_H
