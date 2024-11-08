#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QColor>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle("Fusion");
    
    //QFile stylesheetFile("./themes/altium.qss");
    //stylesheetFile.open(QFile::ReadOnly);
    //QString stylesheet = QLatin1String(stylesheetFile.readAll());
    //a.setStyleSheet(stylesheet);
    MainWindow w;
    w.set_pMain(&a);
    //w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
