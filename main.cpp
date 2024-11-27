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

    int fontId = QFontDatabase::addApplicationFont(":/materials/fonts/SF-Pro.ttf");
    if (fontId == -1)
    {
        qWarning() << "Failed to load font!";
        return -1;
    }
    /*
    QFile styleFile("./themes/mac.qss"); // Ensure the QSS file is in your resources
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    }
    else
    {
        qDebug() << "cannot load style!\n";
    }
    */

    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont appFont(fontFamily);
    appFont.setBold(false);
    appFont.setPointSize(11);
    //a.setFont(appFont);

    MainWindow w;
    w.set_pMain(&a);
    /*
         QFile file("./themes/monokai.qss");
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QString stylesheet = file.readAll();
            a.setStyleSheet(stylesheet);
        } else {
            qWarning("Could not open stylesheet file.");
        }
    */
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
