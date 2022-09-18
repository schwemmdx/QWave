#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QColor>
#include "theme_colors.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "QWave_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }


/*
    QFile styleSheet("../test.qss");

    if (!styleSheet.exists())   {
        qDebug() << ("Unable to set stylesheet, file not found\n");
    }
    else   {
        styleSheet.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&styleSheet);
        qApp->setStyleSheet(ts.readAll());

    }
*/

    QPalette darkpalette;
    darkpalette.setColor(QPalette::Window, Altium::BackGround);
    darkpalette.setColor(QPalette::Text,Altium::LightText);
    darkpalette.setColor(QPalette::WindowText, Altium::LightText);
    darkpalette.setColor(QPalette::Base,Altium::BackGround);
    darkpalette.setColor(QPalette::AlternateBase, Altium::BackGround2);
    darkpalette.setColor(QPalette::ToolTipBase, Altium::BackGround.lighter());
    darkpalette.setColor(QPalette::ToolTipText, Altium::LightText),
    darkpalette.setColor(QPalette::Text, Altium::LightText);
    darkpalette.setColor(QPalette::Button, Altium::BackGround.lighter());
    darkpalette.setColor(QPalette::ButtonText, Altium::LightText);
    darkpalette.setColor(QPalette::BrightText, Altium::LightText.lighter());
    darkpalette.setColor(QPalette::Highlight, Altium::Highlight);
    darkpalette.setColor(QPalette::HighlightedText,Altium::LightText);

    


    a.setPalette(darkpalette);
    MainWindow w;
    //w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
