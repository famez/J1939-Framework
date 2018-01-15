#include "mainwindow.h"
#include <QApplication>

#include <J1939Factory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    J1939::J1939Factory::getInstance().registerPredefinedFrames();

    return a.exec();
}
