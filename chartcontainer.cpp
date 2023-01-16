#include "chartcontainer.h"

#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QApplication>
#include <QValueAxis>


#include "chartcrosshair.h"
#include "customseries.h"


#include <random>

ChartContainer::ChartContainer(QWidget *parent)
{

    pParent = parent;

    middleMousePressed = false;
    chart = new CustomChart(this);

    setChart(chart);

    setRenderHint(QPainter::Antialiasing);

    setAlignment(Qt::AlignLeft);
    setContextMenuPolicy(Qt::CustomContextMenu);
    //setRubberBand(QChartView::NoRubberBand);

    contextMenu = new QMenu(this);
    contextMenu->addAction("Set Limits", this, &ChartContainer::setLimits);
    contextMenu->addAction("Reset Zoom", this, &ChartContainer::resetZoom);
    contextMenu->addAction("Remove Markers",this,&ChartContainer::removeMarkers);
    contextMenu->addSeparator();    
    contextMenu->addAction("Remove Selected", this, &ChartContainer::clearSelectedSeries);
    contextMenu->addAction("Clear Chart", this, &ChartContainer::clearAllSeries);
    
    m_crosshair = new ChartCrosshair(chart);

    connect(chart, &CustomChart::newStatusMsg, this,[this](QString msg){emit newStatusMessage(msg);});
    connect(chart,&CustomChart::newTraceSelection,this,[this](CustomSeries* pSeries){emit newTraceSelection(pSeries);});
    connect(this, &ChartContainer::customContextMenuRequested, this, &ChartContainer::onCustomContextMenu);

}

ChartContainer::~ChartContainer()
{
}


void ChartContainer::addDataSeries(QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel,int toAxis)
{
     chart->addDataSeries(xData,yData,xLabel,yLabel,toAxis);
}


void ChartContainer::wheelEvent(QWheelEvent *event)
{
    bool xScaleChanged=false;
    if (true) // isSelectedContainer())
    {
        int zStep = 10;
        float dir  = 0;
        QRectF rect = chart->plotArea();
        QPoint numSteps;
        QPoint numDegrees = event->angleDelta() / 8;
        if (!numDegrees.isNull())
        {
            numSteps = numDegrees / 15;
        }

        if (event->modifiers().testFlag(Qt::ShiftModifier))
        { // speed up zooming and scrolling by the same factor
            stepModifier = 5;
        }
        else
        {
            stepModifier = 1;
        }

        if (event->modifiers().testFlag(Qt::ControlModifier))
        {
            zoomFactor = event->angleDelta().y() > 0 ? 0.5 : 2;
            QPointF c = chart->plotArea().center();
            rect.setWidth(zoomFactor * rect.width());
            rect.moveCenter(c);
            chart->zoomIn(rect);
        }

        else if (event->modifiers().testFlag(Qt::AltModifier))
        {
            //only scaling which doesn'T change x axis
            zoomFactor = event->angleDelta().x() > 0 ? 0.5 : 2;

            QPointF c = chart->plotArea().center();
            QPoint cursorPos(QCursor::pos());
            int dy = c.y() - cursorPos.y();
            if (abs(dy) > 25)
            {
                dy /= 5;
            }
            c.setY(c.y() - dy);

            rect.setHeight(zoomFactor * rect.height());
            rect.moveCenter(c);
            chart->zoomIn(rect);
        }
        else
        {
            chart->scroll(10 * pow(2, stepModifier) * numSteps.y(), 0);
        }
        event->accept();
        removeMarkers();
    }
}



void ChartContainer::onCustomContextMenu(const QPoint &point)
{
    contextMenu->exec(this->viewport()->mapToGlobal(point));
}


QList<CustomSeries*> ChartContainer::getSeriesInChart(void)
{
    QList<CustomSeries*> tracies;
    foreach(auto &trace, chart->series())
    {
        tracies.append(static_cast<CustomSeries*>(trace));
    }
    return tracies;
}


void ChartContainer::clearSelectedSeries(void)
{

    foreach (auto trace,getSeriesInChart())
    {

        if (trace->isSelected())
        {
            chart->removeSeries(trace);
        }
    }
    auto [l,r] = getNumSeriesPerAxis();
    if(r==0)
    {
        chart->hideRYAxis();
    }
}


void ChartContainer::clearAllSeries(void)
{
    auto answer = QMessageBox::question(this, "Clear Entire Chart?",
                                        "Are you sure to remove all tracies from the chart ?");
    if (QMessageBox::Yes == answer)
    {
        chart->removeAllSeries();
        chart->hideRYAxis();
    }
}

void ChartContainer::resetZoom(void)
{
    chart->zoomReset();
    removeMarkers();
}
void ChartContainer::setLimits(void)
{

}


void ChartContainer::mouseMoveEvent(QMouseEvent *event)
{
    if(m_crosshair->visible())
    {
        m_crosshair->updatePosition(event);
    }
    else{
        m_crosshair->setVisibilty(false);
        QChartView::mouseMoveEvent(event);
    }
}

void ChartContainer::setCrosshairVisibility(bool vis)
{
    m_crosshair->setVisibilty(vis);
}

bool ChartContainer::isCrosshairVisible(void)
{
    return m_crosshair->visible();
}

void ChartContainer::mousePressEvent(QMouseEvent* event)
{

    switch(event->button())
    {
        case Qt::MiddleButton:
        {
            middleMousePressed = true;
            middlePressStartPos = event->pos();
            QApplication::setOverrideCursor(Qt::ClosedHandCursor);

            break;
        }
        case Qt::LeftButton:
        {
            if(m_crosshair->visible())
            {

                requestNewMarker(event->pos());
                //emit markerRequested(event->pos());
                event->accept();
            }

            else
            {
                QChartView::mousePressEvent(event);
            }
          
            break;
        }
        default:
        {
         QChartView::mousePressEvent(event);
        }
    }

    //setCursor(Qt::ArrowCursor);
}


void ChartContainer::requestNewMarker(QPointF pt)
{
    if(chartMarkers.length() <9)
    {

        ChartMarker* marker = new ChartMarker(chart);
        chartMarkers.append(marker);
        marker->placeMarkerbyClick(pt,chartMarkers.length());

        foreach(auto &ser, chart->series())
        {
            if(reinterpret_cast<CustomSeries*>(ser)->isSelected())
            {
                //emit updateCursorData();
            }

        }





    }
    else
    {
        auto answer = QMessageBox::question(this, "Dude, chill!",
                                            "There are plenty of Markers already present, you need to clear them in order to set new ones!\n Do you want to clear all them now ?");
        if (QMessageBox::Yes == answer)
        {
            removeMarkers();
        }
    }

}

void ChartContainer::mouseReleaseEvent(QMouseEvent* event)
{
    switch(event->button())
    {
        case Qt::MiddleButton:
        {
            middleMousePressed = false;
            middlePressEndPos = event->pos();
            auto dx = middlePressStartPos.x()-middlePressEndPos.x();
            auto dy = middlePressEndPos.y()-middlePressStartPos.y();
            chart->scroll(dx,dy);
            QApplication::restoreOverrideCursor();
            event->accept();
            break;
        }
        default:
        {
        QChartView::mouseReleaseEvent(event);
        }
    }
    //event->accept();
    //setCursor(Qt::ArrowCursor);
}

void ChartContainer::removeMarkers(void)
{
    foreach(auto &marker,chartMarkers)
    {
        marker->remove();
        marker->~ChartMarker();
    }
    chartMarkers.clear();
}

std::tuple<int,int> ChartContainer::getNumSeriesPerAxis(void)
{
    int l=0,r = 0;
    foreach(auto &ser,chart->series())
    {
        foreach(auto &ax, ser->attachedAxes())
        {
            if(ax->alignment() == Qt::AlignRight)
            {
                r++;
            }
            if(ax->alignment()== Qt::AlignLeft)
            {
                l++;
            }
        }

    }
    return std::tie(l,r);
}


