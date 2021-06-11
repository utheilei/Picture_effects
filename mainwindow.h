#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QStackedWidget;
class ImageWidget;
class QPushButton;
class ListView;
class BlindsWidget;
class Watermarks;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void moveToCenter();

public slots:
    void changeImage();
    void changeWidget(const QModelIndex &index);

private:
    QLabel *m_label = Q_NULLPTR;
    qreal opacity = 0.0;
    QStackedWidget *m_stackedWidget = Q_NULLPTR;
    BlindsWidget *m_blindsWidget = Q_NULLPTR;
    ImageWidget *imageWidget = Q_NULLPTR;
    Watermarks *m_watermarks = Q_NULLPTR;
    QPushButton *preBtn = Q_NULLPTR;
    QPushButton *nextBtn = Q_NULLPTR;
    int m_index = 0;
    QStringList m_list;
    ListView *m_listView = Q_NULLPTR;
};

#endif // MAINWINDOW_H
