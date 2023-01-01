#include "customseries.h"

#include <QToolTip>
#include <QCursor>
#include <QKeyEvent>
#include <QThread>
#include <QStatusBar>
#include <theme_colors.h>


CustomSeries::CustomSeries(QObject* parent):QLineSeries(parent)
{
    //not work with ChartView events
    //connect(this, &QXYSeries::clicked, this, &CustomSeries::selected);
    connect(this, &QXYSeries::doubleClicked,this,&CustomSeries::selected);

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
        usedPen.setWidth(5);
        setPen(usedPen);
        setPointsVisible(true);
        //setPointLabelsColor(Altium::HighLight2);
        //setPointLabelsVisible(true);


        //setPointLabelsFormat("x: @xPoint \n y: @yPoint");
        emit seriesSelected(this);
    }

}

/*

void CustomSeries::mouseHover(const QPointF point,bool state)
{
    if(isSelected()){
        QPalette pal;
        pal.setColor(QPalette::ToolTipBase, Altium::BackGround2);
        pal.setColor(QPalette::ToolTipText, Altium::LightText);
;
        QPoint pos = QCursor::pos();

        QToolTip* info;
        QToolTip::setPalette(pal);
        info->showText(pos,
                       "X: "
                       +QString::number(point.x())
                       +"\nY: "
                       +QString::number(point.y()),nullptr,QRect(),-1);
        emit newStatusMessage("X: " +QString::number(point.x())+" , Y: "+QString::number(point.y()));
    }
}

*/


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

