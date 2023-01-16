#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customseries.h"
#include "chartcontainer.h"
#include "datawidget.h"
#include "cursordockwidget.h"
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
    void updateCursorData(void);


private slots:

   // void switchSelectedChart(QVector<ChartContainer*>);
    void Ondoubleclicktree(int);
    //void keyPressEvent(QKeyEvent*);
    void on_actionImportData_triggered();
    void on_actioncreateData_triggered();
    void on_actionCrosshair_Mode_triggered();

    void on_actionOptions_triggered();


    void on_actiontoggleCursorDock_toggled(bool arg1);

    void on_actiontoggleDataDock_toggled(bool arg1);

private:
    QDockWidget* pDataDock;
    DataWidget* pDataWidget;
    CursorDockWidget* pCursorDock;


    QApplication* pApplication;
    Ui::MainWindow *ui;

    CustomSeries* focusTrace{nullptr};
    OptionsDialog* pOptionDlg;
    QCursor measureCursor{QPixmap(":icons/icons/measure_cursor.png")};
    ChartContainer* chartContainer;

    void appendDataToChart(QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel,int toAxis);
    void unselectExcept(CustomSeries*);
    void setTheme(void);


protected:
    void keyPressEvent(QKeyEvent*) override;




};
#endif // MAINWINDOW_H
