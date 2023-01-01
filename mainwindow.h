#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customseries.h"
#include "chartcontainer.h"
#include "dataview.h"
#include "optionsdialog.h"


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
    void set_pMain(QApplication*);

signals:
    void loadFromFile(QString);
    void rubberBandChangeRequest(QChartView::RubberBand);
    void changeCrosshairVisibility(bool);
    void chartThmeChangeRequest(int);


private slots:

   // void switchSelectedChart(QVector<ChartContainer*>);
    void Ondoubleclicktree(int);
    //void keyPressEvent(QKeyEvent*);
    void updateFocusTraceDetails(CustomSeries*);
    void on_actionImportData_triggered();
    void updateStatusBar(QString);

    void on_actioncreateData_triggered();
    //void on_actionzoomVertically_triggered();
    //void on_actionzoomROI_triggered();
    //void on_actionZoomHorizontally_triggered();
    void on_actionCrosshair_Mode_triggered();
    void on_actionMeasure_triggered();
    void on_actiontoggleDataView_triggered();
    void on_actionOptions_triggered();


private:
    QDockWidget* pDataDock;
    DataView* pDataView;
    QApplication* pApplication;
    Ui::MainWindow *ui;

    CustomSeries* focusTrace{nullptr};
    OptionsDialog* pOptionDlg;
    QCursor measureCursor{QPixmap(":icons/icons/measure_cursor.png")};
    ChartContainer* chartContainer;

    void appendDataToChart(QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel);
    void unselectExcept(CustomSeries*);
    void setTheme(void);


protected:
    void keyPressEvent(QKeyEvent*) override;




};
#endif // MAINWINDOW_H
