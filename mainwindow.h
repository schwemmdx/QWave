#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chartcontainer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionTest_triggered();
    void switchSelectedChart(QVector<ChartContainer*>);

private:
    Ui::MainWindow *ui;
    QVector<QWidget*> pDockedCharts{};
};
#endif // MAINWINDOW_H
