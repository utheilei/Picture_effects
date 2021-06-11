#include "mainwindow.h"
#include "imagedisplay.h"
#include "listview.h"
#include "blindswidget.h"
#include "watermarks.h"
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QStackedWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , preBtn(new QPushButton("上一张",this))
    , nextBtn(new QPushButton("下一张",this))
    , m_listView (new ListView(this))
{
    QTimer *timer = new QTimer(this);
    preBtn->setObjectName("preBtn");
    nextBtn->setObjectName("nextBtn");

    QWidget *widget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    widget->setLayout(mainLayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(preBtn);
    layout->addWidget(nextBtn);

    QVBoxLayout *vlayout = new QVBoxLayout;
    m_stackedWidget = new QStackedWidget(this);
    vlayout->addWidget(m_stackedWidget);
    vlayout->addLayout(layout);

    mainLayout->addWidget(m_listView);
    mainLayout->addLayout(vlayout);

    m_label = new QLabel(this);
    this->setCentralWidget(widget);

    QImage image(":/images/pixmap1.jpeg");
    //image = image.mirrored(true, false);
    m_label->setPixmap(QPixmap::fromImage(image));

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setOpacity(opacity);
    m_label->setGraphicsEffect(effect);

    connect(timer, &QTimer::timeout, [=](){
        if (opacity >= 1) {
            timer->stop();
            return;
        }
        opacity += 0.1;
        effect->setOpacity(opacity);
        m_label->setGraphicsEffect(effect);
    });

    timer->start(200);

    m_list << ":/images/pixmap1.jpeg" << ":/images/pixmap2.jpg"
           <<":/images/pixmap3.jpg" << ":/images/pixmap4.jpg";
    imageWidget = new ImageWidget(this);
    imageWidget->initImageList(m_list);
    imageWidget->setPixmap(QPixmap(m_list[m_index]));

    m_blindsWidget = new BlindsWidget(this);
    //m_blindsWidget->setOrientation(Qt::Horizontal);
    m_blindsWidget->setPrePixmap(QPixmap(m_list[m_index]));
    m_blindsWidget->setNextPixmap(QPixmap(m_list[m_index + 1]));

    m_watermarks = new Watermarks(this);

    m_stackedWidget->addWidget(m_label);
    m_stackedWidget->addWidget(imageWidget);
    m_stackedWidget->addWidget(m_blindsWidget);
    m_stackedWidget->addWidget(m_watermarks);
    m_stackedWidget->setCurrentIndex(2);

    connect(preBtn, &QPushButton::clicked, this, &MainWindow::changeImage);
    connect(nextBtn, &QPushButton::clicked, this, &MainWindow::changeImage);
    connect(m_listView, &ListView::clicked, this, &MainWindow::changeWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::changeImage()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());

    if (m_stackedWidget->currentIndex() == 1) {
        if ("preBtn" == btn->objectName()) {
            if (m_index - 1 < 0)
                return;
            m_index--;
        } else if("nextBtn" == btn->objectName()) {
            if (m_index + 1 >= m_list.count())
                return;
            m_index++;
        }

        imageWidget->setNextPixmap(m_index);
        imageWidget->start();
    } else if (m_stackedWidget->currentIndex() == 2) {
        m_blindsWidget->setPrePixmap(QPixmap(m_list[m_index]));
        if ("preBtn" == btn->objectName()) {
            if (m_index - 1 < 0)
                return;
            m_index--;
        } else if("nextBtn" == btn->objectName()) {
            if (m_index + 1 >= m_list.count())
                return;
            m_index++;
        }

        m_blindsWidget->setNextPixmap(QPixmap(m_list[m_index]));
        m_blindsWidget->start();
    }
}

void MainWindow::changeWidget(const QModelIndex &index)
{
    m_stackedWidget->setCurrentIndex(index.row());
}

void MainWindow::moveToCenter()
{
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}
