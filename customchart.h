#ifndef CUTOMCHART_H
#define CUTOMCHART_H

#include <QChart>
#include <QObject>
#include <QWidget>
#include <QValueAxis>

#include "customseries.h"

class CustomChart : public QChart
{
    Q_OBJECT

public:
    explicit CustomChart(QObject* parent=nullptr);
    void hideRYAxis();


signals:
    void newStatusMsg(QString);
    void newTraceSelection(CustomSeries*);
public slots:
    void addDataSeries(QVector<double> x, QVector<double> y, QString xUnit, QString yUnit, int toAxis);
    void setGridVisibility(int axis,bool visibility);

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QValueAxis* yAxisLeft;
    QValueAxis* yAxisRight;
    QValueAxis* xAxis;
    QObject* pParent;



};

#endif // CUTOMCHART_H
