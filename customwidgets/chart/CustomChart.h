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
