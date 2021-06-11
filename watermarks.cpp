#include "watermarks.h"

#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

int **generateMatrix(int rows,int columns)
{
    int **numbers=new int*[rows];

    for(int i=0;i<rows;i++){
        numbers[i]=new int[columns];
        for(int j=0;j<columns;j++)
            numbers[i][j] = 0;
    }

    return numbers;
}

Watermarks::Watermarks(QWidget *parent) : QWidget(parent)
{
    connect(&timer, &QTimer::timeout, [=](){
        drawImage();
    });
}

Watermarks::~Watermarks()
{

}

void Watermarks::startAnimation()
{
    if (!timer.isActive())
        timer.start(20);
}

void Watermarks::stopAnimation()
{
    if (timer.isActive())
        timer.stop();
}

void Watermarks::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        dropStone(lastPoint.x(), lastPoint.y(), 20, 700);
        startAnimation();
        isPressed = true;
    }

    QWidget::mousePressEvent(event);
}

void Watermarks::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressed) {
        lastPoint = event->pos();
        dropStone(lastPoint.x(), lastPoint.y(), 20, 50);
    }

    QWidget::mouseMoveEvent(event);
}

void Watermarks::mouseReleaseEvent(QMouseEvent *event)
{
    isPressed = false;

    QWidget::mouseReleaseEvent(event);
}

void Watermarks::paintEvent(QPaintEvent *)
{
    if(m_image.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    //如果停止阶段则绘制原图,否则绘制渲染图
    if (isStoped) {
        painter.drawPixmap(this->rect(), m_image);
    } else {
        painter.drawPixmap(this->rect(), pixmap);
    }
}

void Watermarks::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    m_image = QPixmap(":/images/b31a3e31d4c8cca7c289b8969d28c0b0.jpeg");
    m_image = m_image.scaled(event->size().width(), event->size().height(), Qt::KeepAspectRatio);
    imageWidth = m_image.width();
    imageHeight = m_image.height();
    imageOrigin = m_image.toImage();
    imageRender = QImage(QSize(imageWidth, imageHeight), QImage::Format_RGB32);

    waveBuffer1 = generateMatrix(imageWidth, imageHeight);
    waveBuffer2 = generateMatrix(imageWidth, imageHeight);
}

void Watermarks::drawImage()
{
    if(isStoped) {
        timer.stop();
    } else {
        this->waveRender();
        this->update();
    }
}

void Watermarks::waveSpread()
{
    quint32 counter = 0;
    // 波能渐变
    for(int h = 1; h < imageHeight - 1; h++) {
        for(int w = 1; w < imageWidth - 1; w++) {
            //波能扩散
            waveBuffer2[w][h] = ((waveBuffer1[w - 1][h] + waveBuffer1[w + 1][h] + waveBuffer1[w][h - 1] + waveBuffer1[w][ h + 1]) >> 1) - waveBuffer2[w][h];
            //波能衰减
            waveBuffer2[w][h] -= (waveBuffer2[w][h] >> m_damping);
            //判断波动释放结束
            if(waveBuffer1[w][h] - waveBuffer2[w][h]) {
                counter++;
            }
        }
    }

    //交换缓冲区
    int **temp = waveBuffer1;
    waveBuffer1 = waveBuffer2;
    waveBuffer2 = temp;

    //设置释放完成
    isStoped = counter ? false : true;
}

void Watermarks::waveRender()
{
    //先改变波纹振幅
    waveSpread();
    if(isStoped) {
        return;
    }

    int offsetX = 0;
    int offsetY = 0;

    //扫描位图
    for(int y = 1; y < imageHeight - 1; y++) {
        for(int x = 1; x < imageWidth - 1; x++) {
            //根据波幅计算位图数据偏移值,渲染点(x,y)对应原始图片(offsetX,offsetY)
            offsetX = x + ((waveBuffer1[x - 1][y] - waveBuffer1[x + 1][y]) >> m_refractive);
            offsetY = y + ((waveBuffer1[x][y - 1] - waveBuffer1[x][y + 1]) >> m_refractive);
            //复制象素
            if(0 <= offsetX && offsetX < imageWidth - 1 && 0 <= offsetY && offsetY < imageHeight - 1) {
                QRgb color = imageOrigin.pixel(offsetX, offsetY);
                imageRender.setPixel(x, y, color);
            }
        }
    }

    pixmap.convertFromImage(imageRender);
}

void Watermarks::dropStone(int x, int y, int stoneSize, int stoneWeight)
{
    isStoped = false;
    int posX = 0;
    int posY = 0;

    for(int h = -stoneSize; h < stoneSize; h++) {
        for(int w = -stoneSize; w < stoneSize; w++) {
            posX = x + w;
            posY = y + h;

            //控制范围,不能超出图片
            if(posX < 0 || posX >= imageWidth || posY < 0 || posY >= imageHeight) {
                continue;
            }

            //点(w,h)在一个圆形区域内,初始化波能缓冲区1
            if(h * h + w * w <= stoneSize * stoneSize) {
                waveBuffer1[posX][posY] = -stoneWeight;
            }
        }
    }
}

int Watermarks::getDamping() const
{
    return m_damping;
}

int Watermarks::getRefractive() const
{
    return m_refractive;
}

int Watermarks::getStoneSize() const
{
    return m_stoneSize;
}

int Watermarks::getStoneWeight() const
{
    return m_stoneWeight;
}

QPixmap Watermarks::getImage() const
{
    return m_image;
}

void Watermarks::setDamping(int damping)
{
    m_damping = damping;
}

void Watermarks::setRefractive(int refractive)
{
    m_refractive = refractive;
}

void Watermarks::setStoneSize(int stoneSize)
{
    m_stoneSize = stoneSize;
}

void Watermarks::setStoneWeight(int stoneWeight)
{
    m_stoneWeight = stoneWeight;
}

void Watermarks::setImage(const QPixmap &pixmap)
{
    m_image = pixmap;
}
