#ifndef CHARTCONTAINER_H
#define CHARTCONTAINER_H


#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QMenu>

#include "customseries.h"

class ChartContainer : public QChartView
{
    Q_OBJECT

public:
    explicit ChartContainer(QWidget *parent = nullptr);
    ~ChartContainer();
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
    void changeRubberBandBehaviour(QChartView::RubberBand);
    void setTitle(QString);
    void addDataSeries(QVector<QPointF> data);
    void addDataSeries(QVector<double> ,QVector<double> ,QString ,QString );


private:
    QMenu* contextMenu;
    CustomSeries *series;
    QChart *chart;
    qreal zoomFactor{1.0};
    qreal scrollFactor{1.0};
    qreal stepModifier {1.0};



private slots:

    void newMsgFromSeries(QString);
    void wheelEvent(QWheelEvent*);
    void onCustomContextMenu(const QPoint &);
    //Context Menu Entries
    void clearSelectedSeries(void);
    void clearAllSeries(void);
    void resetZoom(void);
    void setLimits(void);





};

#endif // CHARTCONTAINER_H
