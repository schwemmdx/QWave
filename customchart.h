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
    bool isSecondYaxisVisible();

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QValueAxis* yAxisLeft;
    QValueAxis* yAxisRight;
    QValueAxis* xAxis;
    QObject* pParent;
    bool secondYaxisPopulated{false};
    QVector<QColor> neonColors = {
    QColor(255, 0, 255), // Neon Pink
    QColor(0, 255, 255), // Neon Cyan
    QColor(255, 255, 0), // Neon Yellow
    QColor(0, 255, 0), // Neon Green
    QColor(255, 0, 0), // Neon Red
    QColor(0, 0, 255), // Neon Blue
    QColor(255, 128, 0), // Neon Orange
    QColor(0, 128, 255), // Neon Purple
};



};

#endif // CUTOMCHART_H
