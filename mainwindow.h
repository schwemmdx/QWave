#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customseries.h"
#include "chartcontainer.h"
#include "dataview.h"

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
    void loadFromFile(QString);
    void rubberBandChangeRequest(QChartView::RubberBand);
    void changeCrosshairVisibility(bool);


private slots:

   // void switchSelectedChart(QVector<ChartContainer*>);
    void Ondoubleclicktree(int);
    //void keyPressEvent(QKeyEvent*);
    void updateFocusTraceDetails(CustomSeries*);
    void on_actionImportData_triggered();
    void on_actionTest_triggered();
    void updateStatusBar(QString);

    void on_actioncreateData_triggered();

    void on_actionzoomVertically_triggered();

    void on_actionzoomROI_triggered();

    void on_actionZoomHorizontally_triggered();

    void on_actionCrosshair_Mode_triggered();

    void on_actionMeasure_triggered();

    void on_actiontoggleDataView_triggered();

    void on_actionToggleTools_triggered();

private:
    QDockWidget* pDataDock;
    DataView* pDataView;

    void appendDataToChart(QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel);
    void unselectExcept(CustomSeries*);
    Ui::MainWindow *ui;
    QVector<ChartContainer*> pDockedCharts{};
    CustomSeries* focusTrace{nullptr};
    QCursor measureCursor{QPixmap(":icons/icons/measure_cursor.png")};




};
#endif // MAINWINDOW_H
