#ifndef XCURSOR_H
#define XCURSOR_H

#include <QObject>
#include <QGraphicsItem>
#include <QWidget>
#include <QChart>

#include "ThemeColors.h"

class XCursor : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit XCursor(QChart* chart);
    double getPos();

public slots:
    void placeByClick(QPointF,int num);
    void remove();

private:

    QGraphicsLineItem* m_line;
    QGraphicsTextItem* m_markerNumber;
    QGraphicsTextItem* m_upperText;
    QGraphicsTextItem* m_text;

    QChart* pChart;
    QLineF* line;
    QColor markerRed{Altium::HighLight2};

    QPen markerPen{markerRed,2};
    QVector<QGraphicsTextItem*> yValues;
    double mappedPosition;

};

#endif // XCURSOR_H
