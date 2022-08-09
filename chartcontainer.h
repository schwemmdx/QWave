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
    void addDataSeries(QVector<QPointF> data);
    void deselect(void);

    QList<CustomSeries*> tracies;

signals:
    void chartSelected(ChartContainer*);
    void seriesSelectionChanged(CustomSeries*);
    void newStatusMessage(QString);
    void zoomApplied(int);
    void scrollApplied(int);

public slots:
    void selectedSeriesChanged(CustomSeries*);
    bool isSelectedContainer();

private:
    CustomSeries *series;
    QChart *chart;
    qreal zoomFactor{1.0};
    qreal scrollFactor{1.0};
    qreal stepModifier {1.0};

private slots:
    void newMsgFromSeries(QString);
    void wheelEvent(QWheelEvent*);




};

#endif // CHARTCONTAINER_H
