#include "chartcontainer.h"

#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QApplication>
#include <QValueAxis>

#include "theme_colors.h"
#include "chartcrosshair.h"
#include "customseries.h"

#include <random>

ChartContainer::ChartContainer(QWidget *parent)
{

    pParent = parent;

    middleMousePressed = false;
    chart = new QChart();

    chart->setAcceptedMouseButtons(Qt::LeftButton);
    chart->legend()->hide();

    QBrush bgBrush(Altium::BackGround);
    chart->setBackgroundBrush(bgBrush);
    chart->setMargins(QMargins(5, 5, 5, 5));
    chart->setBackgroundRoundness(5);

    setChart(chart);
    chart->setParent(this);
    setRenderHint(QPainter::Antialiasing);

    setAlignment(Qt::AlignLeft);
    setContextMenuPolicy(Qt::CustomContextMenu);
    //setRubberBand(QChartView::NoRubberBand);
    chart->setAnimationDuration(250);

    contextMenu = new QMenu(this);
    contextMenu->addAction("Set Limits", this, &ChartContainer::setLimits);
    contextMenu->addAction("Reset Zoom", this, &ChartContainer::resetZoom);
    contextMenu->addAction("Remove Markers",this,&ChartContainer::removeMarkers);
    contextMenu->addSeparator();
    contextMenu->addAction("Remove Selected", this, &ChartContainer::clearSelectedSeries);
    contextMenu->addAction("Clear Chart", this, &ChartContainer::clearAllSeries);

    m_crosshair = new ChartCrosshair(chart);


    connect(this, &ChartContainer::customContextMenuRequested, this, &ChartContainer::onCustomContextMenu);



    //Create Axis Objects for the chart
    this->leftYAxis= new QValueAxis;
    this->xAxis = new QValueAxis;

    chart->addAxis(this->leftYAxis, Qt::AlignLeft);
    chart->addAxis(this->xAxis, Qt::AlignBottom);


}

ChartContainer::~ChartContainer()
{
}

void ChartContainer::setTitle(QString title)
{
    chart->setTitle(title);
}

void ChartContainer::addDataSeries(QVector<double> x, QVector<double> y, QString xUnit, QString yUnit)
{
    //CustomSeries should be child of chart
    CustomSeries* series = new CustomSeries(this);

    connect(series,&QXYSeries::clicked,series,&CustomSeries::selected);
    connect(series, &CustomSeries::seriesSelected, this, &ChartContainer::selectedSeriesChanged);
    connect(series, &CustomSeries::newStatusMessage, this, &ChartContainer::newMsgFromSeries);

    auto [xMin,xMax] = std::minmax_element(x.begin(), x.end());
    auto [yMin, yMax] = std::minmax_element(y.begin(),y.end());

    QPointF ptBuf;
    QVector<QPointF> dataBuf;
    for (uint i = 0; i < x.count(); i++)
    {
        ptBuf.setX(x[i]);
        ptBuf.setY(y[i]);
        dataBuf.append(ptBuf);
    }

    chart->update();

    this->xAxis->setRange(*xMin,*xMax);
    this->leftYAxis->setRange(*yMin,*yMax);

    series->setData(dataBuf);
    chart->addSeries(series);

    QPen pen;
    QBrush txtBrush,xBrush;
    xBrush.setColor(Altium::LightText);
    pen.setColor(series->pen().color());
    txtBrush.setColor(pen.color());

    pen.setWidth(2);
    //this->leftYAxis->setLinePen(pen);
    this->leftYAxis->setTitleText(yUnit);
    this->xAxis->setTitleText(xUnit);

    this->leftYAxis->setTitleBrush(xBrush);
    this->leftYAxis->setLabelsAngle(45);
    this->leftYAxis->setLabelsBrush(xBrush);
    this->leftYAxis->applyNiceNumbers();

    this->xAxis->applyNiceNumbers();
    this->xAxis->setTitleBrush(xBrush);
    this->xAxis->setLabelsBrush(xBrush);
    this->xAxis->setGridLineVisible(false);
    this->leftYAxis->setGridLineVisible(false);
    //axisX->setGridLineColor(Altium::BackGround.lighter(300));
    //axisY->setGridLineColor(Altium::BackGround.lighter(300));

    series->attachAxis(this->xAxis);
    series->attachAxis(this->leftYAxis);

    tracies.append(series);

}

void ChartContainer::selectedSeriesChanged(CustomSeries *trace)
{
    emit seriesSelectionChanged(trace);
}

void ChartContainer::newMsgFromSeries(QString msg)
{
    emit newStatusMessage(msg);
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

void ChartContainer::clearSelectedSeries(void)
{
    int i = 0;
    foreach (auto trace, tracies)
    {
        if (trace->isSelected())
        {
            chart->removeSeries(trace);
            tracies.removeAt(i);
        }
        i++;
    }
}
void ChartContainer::clearAllSeries(void)
{
    auto answer = QMessageBox::question(this, "Clear Entire Chart?",
                                        "Are you sure to remove all tracies from the chart ?");
    if (QMessageBox::Yes == answer)
    {
        chart->removeAllSeries();
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
   m_crosshair->updatePosition(event);
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
               // chart->mousePressEvent(event);
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
        event->ignore();
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

