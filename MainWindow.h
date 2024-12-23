#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QStackedWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QTreeView>
#include <QShortcut>

#include "CustomSeries.h"
#include "ChartContainer.h"
#include "ThemeManager.h"
#include "SettingsTree.h"
#include "OptionsDialog.h"

#include "Message.h"
#include "CSVData.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
} QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void selectedSeriesChanged(CustomSeries *);
    void set_pMain(QApplication *);

signals:
    void loadFromFile(QString);
    void rubberBandChangeRequest(QChartView::RubberBand);
    void changeCrosshairVisibility(bool);
    void chartThmeChangeRequest(int);
    void updateCursorData(void);

private slots:

    void on_actionImportData_triggered();
    void on_actionCrosshair_Mode_triggered();

    void on_actionOptions_triggered();

    void on_actiontoggleCursorDock_toggled(bool arg1);
    void on_actiontoggleDataDock_toggled(bool arg1);

    void on_action_ToggleYLeft_triggered(bool checked);

    void on_action_toggleXlog_triggered(bool checked);

    void on_actionToggleYRightLog_triggered(bool checked);

private:
    QTreeView *dataView;
    CSVData *csvData;
    QApplication *pApplication;
    Ui::MainWindow *ui;
    QHBoxLayout *mainLayout;

    CustomSeries *focusTrace{nullptr};
    SettingsTree *settings;
   
    QCursor measureCursor{};
    ChartContainer *chartContainer;
    QString openDlgStartPath{QDir::homePath()};
    QShortcut *toggleStackWidgetShort;
    bool isStackWidgetVisible{true};
    QMap<QString, QString> qssFiles;
    Message* progressMessage{nullptr};

    void unselectExcept(CustomSeries *);
    void applyNewOptions();
    void toggleStackWidget();
    void loadStyles();
    void processNextFile(QStringList files);

protected:
    void keyPressEvent(QKeyEvent * event) override;
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MAINWINDOW_H
