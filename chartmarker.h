#ifndef CHARTMARKER_H
#define CHARTMARKER_H

#include <QtWidgets/QGraphicsItem>
#include <QtCharts/QChartGlobal>
#include <QChart>


#include "theme_colors.h"

class ChartMarker: public QObject
{

    Q_OBJECT

public:
    ChartMarker(QChart* chart);

    void placeMarkerbyClick(QPointF);

private:
    QGraphicsLineItem* m_line;
    QGraphicsTextItem* m_markerNumber;
    QGraphicsTextItem* m_upperText;
    QGraphicsTextItem* m_text;
    QChart* pChart;
    QLineF* line;
    QColor markerRed{QColor(255,184,108)};
    QPen markerPen{markerRed,3};
    QVector<QGraphicsTextItem*> yValues;

};

#endif // CHARTMARKER_H
