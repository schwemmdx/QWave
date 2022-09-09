#include "chartcontainer.h"


#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QMessageBox>


#include "theme_colors.h"

#include "customseries.h"


#include <random>

ChartContainer::ChartContainer(QWidget *parent)
{
    series = new CustomSeries(this);



    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    tracies.append(series);

    chart->createDefaultAxes();
    chart->setMargins(QMargins(8,8,8,8));
    chart->setBackgroundRoundness(8);
    setChart(chart);

    setRenderHint(QPainter::Antialiasing);
    chart->setAnimationDuration(250);
    setAlignment(Qt::AlignLeft);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setRubberBand(QChartView::NoRubberBand);


    contextMenu = new QMenu(this);
    contextMenu->addAction("Set Limits",this,&ChartContainer::setLimits);
    contextMenu->addAction("Reset Zoom",this,&ChartContainer::resetZoom);
    contextMenu->addSeparator();
    contextMenu->addAction("Remove Selected",this,&ChartContainer::clearSelectedSeries);
    contextMenu->addAction("Clear Chart",this,&ChartContainer::clearAllSeries);





    connect(series,&CustomSeries::seriesSelected,this,&ChartContainer::selectedSeriesChanged);
    connect(series,&CustomSeries::newStatusMessage,this,&ChartContainer::newMsgFromSeries);
    connect(this,&ChartContainer::customContextMenuRequested,this,&ChartContainer::onCustomContextMenu);
}

ChartContainer::~ChartContainer()
{

}

void ChartContainer::setTitle(QString title)
{
    chart->setTitle(title);
}

void ChartContainer::addDataSeries(QVector<QPointF> data)
{
    series = new CustomSeries(this);
    series->setData(data);
    tracies.append(series);
    chart->addSeries(series);

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

        else if(event->modifiers().testFlag(Qt::AltModifier))
        {
            zoomFactor = event->angleDelta().x() > 0 ? 0.5 : 2;

            QPointF c = chart->plotArea().center();
            QPoint cursorPos(QCursor::pos());
            int dy = c.y() - cursorPos.y();
            if(abs(dy) > 25 )
            {
                dy/=5;
            }
            c.setY(c.y()-dy);

            rect.setHeight(zoomFactor*rect.height());
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

void ChartContainer::onCustomContextMenu(const QPoint &point)
{
    contextMenu->exec(this->viewport()->mapToGlobal(point));
}

void ChartContainer::clearSelectedSeries(void)
{
    int i = 0;
    foreach(auto trace,tracies)
    {
        if(trace->isSelected())
        {
            chart->removeSeries(trace);
            tracies.removeAt(i);
        }
        i++;

    }
}
void ChartContainer::clearAllSeries(void)
{
    auto answer = QMessageBox::question(this,"Clear Entire Chart?",
                          "Are you sure to remove all tracies from the chart ?");
    if(QMessageBox::Yes == answer)
    {
        chart->removeAllSeries();
    }
}

void ChartContainer::resetZoom(void)
{
    chart->resetTransform();
}
void ChartContainer::setLimits(void)
{

}

void ChartContainer::changeRubberBandBehaviour(QChartView::RubberBand rb)
{
    setRubberBand(rb);
}
