#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QColor>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle("Fusion");

    int fontId = QFontDatabase::addApplicationFont(":/materials/fonts/SF-Pro-Rounded-Regular.otf");
    if (fontId == -1) {
    qWarning() << "Failed to load font!";
    return -1;
    }
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont appFont(fontFamily);
    appFont.setPointSize(12);
    a.setFont(appFont);

    MainWindow w;
    w.set_pMain(&a);
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
