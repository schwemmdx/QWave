#include "customseries.h"

#include <QToolTip>
#include <QCursor>
#include <QKeyEvent>
#include <QThread>
#include <QStatusBar>
#include <theme_colors.h>
#include <QGraphicsTextItem>
#include <QMenu>


CustomSeries::CustomSeries(QObject* parent): QLineSeries(parent)
{
    //not work with ChartView events
    setParent(parent);
    connect(this,&QXYSeries::hovered,this,&CustomSeries::onHover);
    connect(this, &QXYSeries::clicked, this, &CustomSeries::selected);

    //connect(this, &QXYSeries::,this,&CustomSeries::onPointPress);

    selectionState = false;
    setPointsVisible(false);
    setOpacity(0.5);

}

void CustomSeries::setData(QVector<QPointF> data)
{
    this->clear();
    *this << data;
}
void CustomSeries::unselect()
{
    selectionState = false;
    setOpacity(0.5);
    QPen usedPen = pen();
    usedPen.setWidth(1);
    setPen(usedPen);
    setPointsVisible(false);
    setPointLabelsVisible(false);

}

void CustomSeries::selected(const QPointF &point)
{

    if(selectable)
    {

        selectionState = true;
        setOpacity(1);
        QPen usedPen = pen();
        usedPen.setWidth(3);
        setPen(usedPen);
        setPointsVisible(true);

        //setPointLabelsColor(Altium::HighLight2);
        //setPointLabelsVisible(true);
        //setPointLabelsFormat("x: @xPoint \n y: @yPoint");
        emit seriesSelected(this);
    }

}



void CustomSeries::onHover(const QPointF &point,bool state)
{
    if(true){
        
    }
}

void CustomSeries::onPointPress(const QPointF &point)
{
    qDebug() << "Point" << point;
}




bool CustomSeries::isSelected(void)
{
    return selectionState;
}

void CustomSeries::setXLimits(float xmin,float xmax)
{

}
void CustomSeries::setYLimitsL(float ymin,float ymax)
{

}
void CustomSeries::setYLimitsR(float ymin,float ymax)
{

}

