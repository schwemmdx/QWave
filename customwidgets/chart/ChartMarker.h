#ifndef CHARTMARKER_H
#define CHARTMARKER_H

#include <QtWidgets/QGraphicsItem>
#include <QGraphicsItem>
#include <QtCharts/QChartGlobal>
#include <QChart>
#include <QHoverEvent>
#include <QResizeEvent>
#include <QWidget>

#include "ThemeColors.h"

class ChartMarker : public QGraphicsWidget
{

    Q_OBJECT

public:
    ChartMarker(QChart *chart);

    void placeMarkerbyClick(QPointF, int);
    void passEvent(QMouseEvent *);
    bool isUnderMouse(QMouseEvent *);
    void remove(void);
    void resizeEvent(QResizeEvent *event);

    QList<QPointF> intersectPoints;

private:
    QGraphicsLineItem *m_line;
    QGraphicsTextItem *m_markerNumber;
    QGraphicsTextItem *m_upperText;
    QGraphicsTextItem *m_text;

    QChart *pChart;
    QLineF *line;
    QColor markerColor{Monokai::TertiaryBackground};
    // QColor(255,184,108)};
    QPen markerPen{QPen(markerColor, 1.25)};
    QVector<QGraphicsTextItem *> yValues;

protected:
    // virtual void mousePressEvent(QMouseEvent*) override;
    // void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override
    QRectF boundingRect() const override;
};

#endif // CHARTMARKER_H
