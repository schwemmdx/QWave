#ifndef CUTOMCHART_H
#define CUTOMCHART_H

#include <QChart>
#include <QObject>
#include <QWidget>
#include <QValueAxis>
#include <QLogValueAxis>

#include "CustomSeries.h"



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
    CustomSeries* addDataSeries(QVector<double> x, QVector<double> y, QString xUnit, QString yUnit, Qt::Alignment align);
    void setGridVisibility(int axis,bool visibility);
    bool isSecondYaxisVisible();
    void setLogYLScale(bool arg);
    void setLogYRScale(bool arg);
    void setLogXScale(bool arg);
    
    
protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void replaceAxis(QAbstractAxis* oldAx, QAbstractAxis* newAx,enum Qt::AlignmentFlag);
    void applyAxisStyles(QAbstractAxis* axis, const QString& labelFormat, const QBrush& labelBrush, bool hideGridLines);
    
    void updateAxisRange(QAbstractAxis *axis);
    QValueAxis* yLeftLin;
    QValueAxis* yRightLin;
    QLogValueAxis* yLeftLog;
    QLogValueAxis* yRightLog;
    QValueAxis* xAxisLin;
    QLogValueAxis* xAxisLog;
    QObject* pParent;
    bool useLogScale{false};
    bool secondYaxisPopulated{false};
    void setupLinAxis();
    void setupLogAxis();

    bool useLogLeftY{false};
    bool useLogRightY{false};
    bool useLogX{false};

    QVector<QColor> neonColors = {
    QColor(31, 119, 180),   // Blue
    QColor(255, 127, 14),   // Orange
    QColor(44, 160, 44),    // Green
    QColor(214, 39, 40),    // Red
    QColor(148, 103, 189),  // Purple
    QColor(140, 86, 75),    // Brown
    QColor(227, 119, 194),  // Pink
    QColor(127, 127, 127),  // Gray
    QColor(188, 189, 34),   // Yellow-Green
    QColor(23, 190, 207)    // Cyan
};

};


#endif // CUTOMCHART_H
