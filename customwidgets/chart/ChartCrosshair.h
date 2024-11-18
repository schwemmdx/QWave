#ifndef CROSSHAIR_H
#define CROSSHAIR_H



#include "qcursor.h"
#include <QtCharts/QChartGlobal>
#include <QChart>
#include <QtWidgets/QGraphicsItem>

#include "CustomChart.h"

class ChartCrosshair
{
public:
    ChartCrosshair(CustomChart *chart);
    void updatePosition(QMouseEvent* event);


    void setVisibilty(bool);
    void showCrosshair(void);
    bool visible(void);

private:
    QGraphicsLineItem *m_xLine, *m_yLine;
    QGraphicsTextItem *m_xText, *m_yLeftText, *m_yRightText;
    CustomChart *m_chart;
    QLineF* xLine;
    QLineF* yLine;
    QCursor crosshairCursor{Qt::BlankCursor};//(QPixmap(":/icons/icons/icons8-target-24.png"))};
    bool visibility;


};

#endif // CROSSHAIR_H
