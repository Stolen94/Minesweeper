#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setDoubleClickInterval(600);//??????????????


    MainWindow w;
    w.setWindowFlags(Qt::WindowCloseButtonHint);//Leaves only "close" button for the window

    w.show();
    return a.exec();
}
