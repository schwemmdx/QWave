#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QColor>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle("Fusion");


    MainWindow w;
    w.set_pMain(&a);
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
