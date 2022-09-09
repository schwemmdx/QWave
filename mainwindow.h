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


private slots:

   // void switchSelectedChart(QVector<ChartContainer*>);
    void Ondoubleclicktree(int);
    void keyPressEvent(QKeyEvent*);
    void updateFocusTraceDetails(CustomSeries*);
    void on_actionImportData_triggered();
    void on_actionTest_triggered();
    void updateStatusBar(QString);

    void on_actioncreateData_triggered();

    void on_actionzoomVertically_triggered();

    void on_actionzoomROI_triggered();

    void on_actionZoomHorizontally_triggered();

private:
    QDockWidget* pDataDock;
    DataView* pDataView;

    void appendDataToChart(QVector<QPointF>,int);
    void unselectExcept(CustomSeries*);
    Ui::MainWindow *ui;
    QVector<ChartContainer*> pDockedCharts{};
    CustomSeries* focusTrace{nullptr};



};
#endif // MAINWINDOW_H
