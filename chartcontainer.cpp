#include "chartcontainer.h"


#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>


#include "theme_colors.h"

#include "customseries.h"


#include <random>

ChartContainer::ChartContainer(QWidget *parent)
{
    series = new CustomSeries(this);
    connect(series,&CustomSeries::seriesSelected,this,&ChartContainer::selectedSeriesChanged);
    connect(series,&CustomSeries::newStatusMessage,this,&ChartContainer::newMsgFromSeries);


    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    tracies.append(series);

    chart->createDefaultAxes();
    chart->setMargins(QMargins(8,8,8,8));
    chart->setBackgroundRoundness(8);
    setChart(chart);
    //setRubberBand(QChartView::RectangleRubberBand);
    setRenderHint(QPainter::Antialiasing);
    setAlignment(Qt::AlignLeft);



}

ChartContainer::~ChartContainer()
{

}

void ChartContainer::setTitle(QString title)
{
    chart->setTitle(title);

}


void ChartContainer::selectedSeriesChanged(CustomSeries* trace)
{
 emit seriesSelectionChanged(trace);
}

void ChartContainer::newMsgFromSeries(QString msg)
{
    emit newStatusMessage(msg);
}

void ChartContainer::wheelEvent(QWheelEvent* event)
{
    if(isSelectedContainer())
    {
        int zStep = 10;
        float dir = 0;
        QRectF rect = chart->plotArea();
        QPoint numSteps;
        QPoint numDegrees = event->angleDelta()/8;
        if (!numDegrees.isNull()) {
             numSteps = numDegrees/15;
        }

        if(event->modifiers().testFlag(Qt::ShiftModifier))
        {   //speed up zooming and scrolling by the same factor
            stepModifier = 5;
        }
        else
        {
            stepModifier = 1;
        }

        if(event->modifiers().testFlag(Qt::ControlModifier))
         {
            zoomFactor = event->angleDelta().y() > 0 ? 0.5 : 2;
            QPointF c = chart->plotArea().center();
            rect.setWidth(zoomFactor*rect.width());
            rect.moveCenter(c);
            chart->zoomIn(rect);
         }
         else
         {
             chart->scroll(10*pow(2,stepModifier)*numSteps.y(),0);
         }

        event->accept();
    }
}

bool ChartContainer::isSelectedContainer(void)
{
    bool selectionState = false;
    foreach (auto &trace, chart->series()) {
        if(reinterpret_cast<CustomSeries*>(trace)->isSelected())
        {
            selectionState =  true;
            break;
        }
    }
    return selectionState;
}
