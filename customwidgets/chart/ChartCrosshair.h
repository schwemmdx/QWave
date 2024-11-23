#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "qcursor.h"
#include <QtCharts/QChartGlobal>
#include <QChart>
#include <QtWidgets/QGraphicsItem>

#include "CustomChart.h"
#include "ThemeColors.h"

class ChartCrosshair
{
public:
    ChartCrosshair(CustomChart *chart);
    void updatePosition(QMouseEvent *event);

    void setVisibilty(bool);
    void showCrosshair(void);
    bool visible(void);

private:
    QGraphicsLineItem *m_xLine, *m_yLine, *m_ylowLine, *m_xlowLine, *m_yupLine, *m_xupline;
    QGraphicsTextItem *m_xText, *m_yLeftText, *m_yRightText;
    CustomChart *m_chart;
    QLineF *xLine;
    QLineF *yLine;

    QLineF *lowerXline;
    QLineF *lowerYline;
    QLineF *upperXline;
    QLineF *upperYline;

    QCursor crosshairCursor{Qt::BlankCursor}; //(QPixmap(":/icons/icons/icons8-target-24.png"))};
    bool visibility;

    QColor grayCol{Monokai::HighlightColor};
    QColor textCol{Monokai::SystemBackground};
};

#endif // CROSSHAIR_H
