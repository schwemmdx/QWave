#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customseries.h"
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

    void selectedSeriesChanged(CustomSeries*);

signals:


private slots:
    void on_actionTest_triggered();
   // void switchSelectedChart(QVector<ChartContainer*>);
    void keyPressEvent(QKeyEvent*);
    void updateFocusTraceDetails(CustomSeries*);
    void on_actionImportData_triggered();

private:
    Ui::MainWindow *ui;
    QVector<ChartContainer*> pDockedCharts{};



};
#endif // MAINWINDOW_H
