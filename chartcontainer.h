#ifndef CHARTCONTAINER_H
#define CHARTCONTAINER_H


#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>


class ChartContainer : public QChartView
{
    Q_OBJECT

public:
    explicit ChartContainer(QWidget *parent = nullptr);
    ~ChartContainer();
    void setTitle(QString);
    void deselect(void);

signals:
    void chartSelected(ChartContainer*);

private:
    QLineSeries *series;
    QChart *chart;

    void mousePressEvent ( QMouseEvent * event );
    /*
    for(auto &chartContainer: pDockedCharts)
    {
         ChartContainer* chart = static_cast<ChartContainer*>(chartContainer);
         chart->deselect();
    }
    */
};

#endif // CHARTCONTAINER_H
