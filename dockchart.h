#ifndef DOCKCHART_H
#define DOCKCHART_H

#include <QDockWidget>


#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>


namespace Ui {
class DockChart;
}

class DockChart : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockChart(QWidget *parent = nullptr);
    ~DockChart();
public slots:
    void highlight(bool);

private:
    Ui::DockChart *ui;
     QLineSeries *series;
     QChart *chart;
     QChartView *chartView;
};

#endif // DOCKCHART_H
