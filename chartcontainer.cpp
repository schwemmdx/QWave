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


ChartContainer::ChartContainer(QWidget *parent)
{

    pParent = parent;

    middleMousePressed = false;
    chart = new CustomChart(this);

    setChart(chart);

    setRenderHint(QPainter::Antialiasing);
    
    setAlignment(Qt::AlignLeft);
    setContextMenuPolicy(Qt::CustomContextMenu);
    //setMouseTracking(true);
    setRubberBand(QChartView::RectangleRubberBand);

    contextMenu = new QMenu(this);
    //contextMenu->addAction("Set Limits", this, &ChartContainer::setLimits);
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
     update();
}


void ChartContainer::wheelEvent(QWheelEvent *event)
{
    QRectF rect = chart->plotArea();
    zoomFactor = event->angleDelta().y() > 0 ? 0.75 : 1.25;
    QPointF c = chart->plotArea().center();

    switch(event->modifiers())
    {
        case Qt::ControlModifier:
            {

                rect.setWidth(zoomFactor * rect.width());
                rect.setHeight(zoomFactor* rect.height());
                rect.moveCenter(c);
                chart->zoomIn(rect);
                event->accept();

                removeMarkers();
                break;
            }
        case Qt::AltModifier:
        {

            zoomFactor = event->angleDelta().x() > 0 ? 0.5 : 2;

            QPointF c = chart->plotArea().center();
           

            rect.setWidth(zoomFactor * rect.width());
            rect.moveCenter(c);
            chart->zoomIn(rect);
            break;
        }
        case Qt::ShiftModifier:
        {

            rect.setHeight(zoomFactor * rect.height());
            rect.moveCenter(c);
            chart->zoomIn(rect);
            event->accept();
            removeMarkers();
            break;
        }
        default:
            QChartView::wheelEvent(event);
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

    }

    if(middleMousePressed)
    {
        auto dPos = event->pos() - middlePressStartPos;
        chart->scroll(-dPos.x(), dPos.y());

        middlePressStartPos = event->pos();
        event->accept();


    }

    QChartView::mouseMoveEvent(event);
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
        case Qt::RightButton:
        {
            event->accept();
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
        case Qt::RightButton:
        {
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


