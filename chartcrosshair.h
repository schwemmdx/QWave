#ifndef CROSSHAIR_H
#define CROSSHAIR_H



#include "qcursor.h"
#include <QtCharts/QChartGlobal>
#include <QChart>
#include <QtWidgets/QGraphicsItem>


class ChartCrosshair
{
public:
    ChartCrosshair(QChart *chart);
    void updatePosition(QMouseEvent* event);

    void setVisibilty(bool);

private:
    QGraphicsLineItem *m_xLine, *m_yLine;
    QGraphicsTextItem *m_xText, *m_yText;
    QChart *m_chart;
    QLineF* xLine;
    QLineF* yLine;
    QCursor crosshairCursor{(QPixmap(":/icons/icons/icons8-target-24.png"))};
    bool visibility;


};

#endif // CROSSHAIR_H
