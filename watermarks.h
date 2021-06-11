#ifndef WATERMARKS_H
#define WATERMARKS_H

#include <QTimer>
#include <QWidget>

class Watermarks : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int m_damping READ getDamping WRITE setDamping)
    Q_PROPERTY(int m_refractive READ getRefractive WRITE setRefractive)
    Q_PROPERTY(int m_stoneSize READ getStoneSize WRITE setStoneSize)
    Q_PROPERTY(int m_stoneWeight READ getStoneWeight WRITE setStoneWeight)
    Q_PROPERTY(QPixmap m_image READ getImage WRITE setImage)

public:
    explicit Watermarks(QWidget *parent = Q_NULLPTR);
    ~Watermarks();

    void startAnimation();

    void stopAnimation();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    int m_damping = 100;            //波能衰减系数
    int m_refractive = 100;         //波能折射系数
    int m_stoneSize;          //石头大小
    int m_stoneWeight;        //石头重量
    QPixmap m_image;          //渲染图片

    QImage imageRender;     //渲染像素图片
    QImage imageOrigin;     //渲染源
    QPixmap pixmap;         //渲染图片

    int	imageWidth;         //图片宽度
    int	imageHeight;        //图片高度
    int **waveBuffer1;		//波能缓冲区1
    int **waveBuffer2;		//波能缓冲区2

    bool isStoped = true;          //水波是否结束波动
    bool isPressed = false;         //鼠标是否按下
    QPoint lastPoint;       //鼠标按下出的坐标
    QTimer timer;          //定时器绘制

private slots:
    //执行计算位图进行绘制
    void drawImage();
    //水波步进扩散
    void waveSpread();
    //渲染水波位图
    void waveRender();
    //扔石子
    void dropStone(int x, int y, int m_stoneSize, int m_stoneWeight);

public:
    int getDamping()        const;
    int getRefractive()     const;
    int getStoneSize()      const;
    int getStoneWeight()    const;
    QPixmap getImage()      const;

public Q_SLOTS:
    //设置衰减率系数
    void setDamping(int m_damping);
    //设置折射率系数
    void setRefractive(int m_refractive);
    //设置石头大小
    void setStoneSize(int m_stoneSize);
    //设置石头重量
    void setStoneWeight(int m_stoneWeight);
    //设置背景图片
    void setImage(const QPixmap &pixmap);
};

#endif // WATERMARKS_H
