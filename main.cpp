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
/*
    QPalette darkpalette;
    darkpalette.setColor(QPalette::Window, Theme::BackGround);
    darkpalette.setColor(QPalette::Text,Theme::ForeGround);
    darkpalette.setColor(QPalette::WindowText, Theme::ForeGround);
    darkpalette.setColor(QPalette::Base,Theme::BackGround2);
    darkpalette.setColor(QPalette::AlternateBase, Theme::BackGround2);
    darkpalette.setColor(QPalette::ToolTipBase, Theme::BackGround.lighter());
    darkpalette.setColor(QPalette::ToolTipText, Theme::ForeGround),
    darkpalette.setColor(QPalette::Text, Theme::ForeGround);
    darkpalette.setColor(QPalette::Button, Theme::BackGround.lighter());
    darkpalette.setColor(QPalette::ButtonText, Theme::ForeGround);
    darkpalette.setColor(QPalette::BrightText, Theme::ForeGround.lighter());
    darkpalette.setColor(QPalette::Highlight, Theme::Green);
    darkpalette.setColor(QPalette::HighlightedText,Theme::Red);

*/
    //QFile stylesheet("../../QWave/SyNet.qss");


  //  a.setPalette(darkpalette);
    MainWindow w;
    //w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
