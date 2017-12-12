#include "mainwindow.h"
#include <QApplication>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   // w.setWindowFlags(w.windowFlags()& ~Qt::WindowMaximizeButtonHint&  ~Qt::WindowMinimizeButtonHint);
   // w.setWindowFlags(Qt::FramelessWindowHint);
    w.resize(750,500);
    w.show();
    return a.exec();
}
