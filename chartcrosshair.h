#ifndef CROSSHAIR_H
#define CROSSHAIR_H



#include <QtCharts/QChartGlobal>
#include <QChart>
#include <QtWidgets/QGraphicsItem>


class ChartCrosshair
{
public:
    ChartCrosshair(QChart *chart);
    void updatePosition(QMouseEvent* event);

private:
    QGraphicsLineItem *m_xLine, *m_yLine;
    QGraphicsTextItem *m_xText, *m_yText;
    QChart *m_chart;
    QLineF* xLine;
    QLineF* yLine;
    bool ctrlPressed,shiftPressed;

};

#endif // CROSSHAIR_H
