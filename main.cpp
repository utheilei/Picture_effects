#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(600,480);
    w.moveToCenter();
    w.show();

    return a.exec();
}
