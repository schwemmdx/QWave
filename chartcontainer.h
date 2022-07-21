#ifndef CHARTCONTAINER_H
#define CHARTCONTAINER_H


#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include "customseries.h"

class ChartContainer : public QChartView
{
    Q_OBJECT

public:
    explicit ChartContainer(QWidget *parent = nullptr);
    ~ChartContainer();
    void setTitle(QString);
    void deselect(void);

    QList<CustomSeries*> tracies;

signals:
    void chartSelected(ChartContainer*);
    void seriesSelectionChanged(CustomSeries*);
    void newStatusMessage(QString);

public slots:
    void selectedSeriesChanged(CustomSeries*);

private:
    CustomSeries *series;
    QChart *chart;

private slots:
    void newMsgFromSeries(QString);




};

#endif // CHARTCONTAINER_H
