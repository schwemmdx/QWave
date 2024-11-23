
#include <QToolTip>
#include <QCursor>
#include <QKeyEvent>
#include <QThread>
#include <QStatusBar>
#include <QGraphicsTextItem>
#include <QMenu>

#include "CustomSeries.h"
#include "MessageQueue.h"
#include "ScientificFormatter.h"
#include <ThemeColors.h>

CustomSeries::CustomSeries(QObject* parent): QLineSeries(parent)
{
    //not work with ChartView events
    setParent(parent);
    //connect(this,&QXYSeries::hovered,this,&CustomSeries::onHover);
    connect(this, &QXYSeries::clicked, this, &CustomSeries::selected);

    //connect(this, &QXYSeries::,this,&CustomSeries::onPointPress);
    selectionState = false;
    setPointsVisible(false);
    setOpacity(0.75);

    pen().setWidth(4);
 
}

void CustomSeries::setData(QVector<QPointF> d,QString label)
{
    this->clear();
    *this << d;
    setName(label);
    setLimits(d);
}
void CustomSeries::unselect()
{
    selectionState = false;
    setOpacity(0.75);
    QPen usedPen = pen();
    usedPen.setWidth(4);
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
        usedPen.setWidth(6);
        setPen(usedPen);
        setPointsVisible(true);
        
        setPointLabelsColor(pen().color());
        //setPointLabelsVisible(true);
        //setPointLabelsFormat("x: @xPoint \n y: @yPoint");
        emit seriesSelected(this);
    }

}



void CustomSeries::onHover(const QPointF &point,bool state)
{
    MessageQueue* q = MessageQueue::instance();
    q->addMessage("x: "+ScientificFormatter::toScientificSuffix(point.x())+
    "\ny: "+ScientificFormatter::toScientificSuffix(point.y()),Monokai::PrimaryLabel);
    
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

int CustomSeries::findClosestPointIndex(const QPointF &chartPosition)
{
    const QList<QPointF> &pointList = points();

    // Check for an empty series
    if (pointList.isEmpty()) {
        
        return -1;
    }

    // Get the x-coordinate of the vertical line (target x position)
    double targetX = chartPosition.x();

    // Use std::min_element to find the index of the closest point by x distance
    auto it = std::min_element(pointList.begin(), pointList.end(),
        [targetX](const QPointF &p1, const QPointF &p2) {
            double dist1 = std::abs(p1.x() - targetX);
            double dist2 = std::abs(p2.x() - targetX);
            return dist1 < dist2;
        }
    );

    // Check if std::min_element found a valid iterator
    if (it == pointList.end()) {
        qDebug() << "Error: No minimum element found.";
        return -1;
    }

    // Return the index of the closest point
    int closestIndex = std::distance(pointList.begin(), it);
    qDebug() << "Index of closest point by x-component:" << closestIndex;
    return closestIndex;
}

void CustomSeries::setLimits(QVector<QPointF> data)
{
    QVector<float> xBuf,yBuf;
    foreach(auto &pt,data)
    {
        xBuf.append(pt.x());
        yBuf.append(pt.y());
    }
    auto [min,max] = std::minmax(xBuf.begin(),xBuf.end());
    xmin = *min;
    xmax = *max;
    auto [min1,max1] = std::minmax(yBuf.begin(),yBuf.end());
    ymin = *min1;
    ymax = *max1;
    
}