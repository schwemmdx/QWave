
#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QApplication>
#include <QValueAxis>
#include <QPair>

#include "ChartContainer.h"
#include "ChartCrosshair.h"
#include "CustomSeries.h"
#include "MessageQueue.h"

ChartContainer::ChartContainer(QWidget *parent)
{

    pParent = parent;

    middleMousePressed = false;
    chart = new CustomChart(this);

    setChart(chart);

    setRenderHint(QPainter::Antialiasing);

    setAlignment(Qt::AlignLeft);
    setContextMenuPolicy(Qt::CustomContextMenu);
    // setMouseTracking(true);
    setRubberBand(QChartView::RectangleRubberBand);

    contextMenu = new QMenu(this);
    // contextMenu->addAction("Set Limits", this, &ChartContainer::setLimits);
    contextMenu->addAction("Reset Zoom", this, &ChartContainer::resetZoom);
    contextMenu->addAction("Remove Markers", this, &ChartContainer::removeMarkers);
    contextMenu->addSeparator();
    contextMenu->addAction("Remove Selected", this, &ChartContainer::clearSelectedSeries);
    contextMenu->addAction("Clear Chart", this, &ChartContainer::clearAllSeries);

    m_crosshair = new ChartCrosshair(chart);

    connect(chart, &CustomChart::newStatusMsg, this, [this](QString msg)
            { emit newStatusMessage(msg); });
    connect(chart, &CustomChart::newTraceSelection, this, [this](CustomSeries *pSeries)
            { emit newTraceSelection(pSeries); });
    connect(this, &ChartContainer::customContextMenuRequested, this, &ChartContainer::onCustomContextMenu);

    // Chart Background
    chart->setBackgroundBrush(Monokai::SystemBackground);
    chart->setBackgroundPen(QPen(Monokai::SystemBackground, 1));
    setBackgroundBrush(QBrush(Monokai::SystemBackground));
    // Plot Area Background
    QBrush plotAreaBrush(Monokai::SystemBackground);
    plotAreaBrush.setStyle(Qt::Dense1Pattern);
    chart->setPlotAreaBackgroundBrush(plotAreaBrush);
    chart->setPlotAreaBackgroundVisible(true);

    // Title
    chart->setTitleBrush(Monokai::PrimaryLabel);
    QFont titleFont = chart->titleFont();
    titleFont.setPointSize(10);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);

    // Axes
    for (QAbstractAxis *axis : chart->axes()) {
        // Axis Line
        axis->setLinePen(QPen(Monokai::SecondaryLabel, 1));

        // Axis Labels
        axis->setLabelsBrush(Monokai::PrimaryLabel);
        
        QFont labelFont = axis->labelsFont();
        labelFont.setPointSize(9);
        axis->setLabelsFont(labelFont);

        // Grid Lines
        axis->setGridLinePen(QPen(Monokai::TertiaryBackground, 0.5, Qt::DashLine));

        // Minor Grid Lines
        axis->setMinorGridLinePen(QPen(Monokai::TertiaryBackground, 0.5, Qt::DotLine));

        // Shades between grid lines
        //axis->setShadesBrush(QBrush(Monokai::PrimaryFill));
        //axis->setShadesVisible(true);
    }

    // Legend
    QLegend *legend = chart->legend();
    legend->setBrush(Monokai::SecondaryBackground);
    legend->setLabelBrush(Monokai::SecondaryLabel);
    QFont legendFont = legend->font();
    legendFont.setPointSize(10);
    legend->setFont(legendFont);
    legend->setBorderColor(Monokai::BorderColor);
    
   
}


ChartContainer::~ChartContainer()
{
}

/*
void ChartContainer::addDataSeries(QVector<double> xData,
QVector<double> yData,
QString xLabel,
QString yLabel,
int toAxis)
{
     chart->addDataSeries(xData,yData,xLabel,yLabel,toAxis);
     update();
}
*/

void ChartContainer::addSeriesToChart(const QString &label,
                                      const QVector<double> &xData,
                                      const QVector<double> &yData,
                                      Qt::Alignment alignment)
{

    // create a new series and add it to the map with its label as identifier
    // hopefully there is no label clash?

    chartContent[label] = chart->addDataSeries(xData, yData, "", "", alignment);

    update();
    // update all axis for the new values

    chart->legend()->setVisible(true);
}

void ChartContainer::removeSeriesFromChart(const QString &label)
{
    if (chartContent.contains(label))
    {
        chart->removeSeries(chartContent[label]);
        chartContent.remove(label);
    }
}

void ChartContainer::wheelEvent(QWheelEvent *event)
{
    QRectF rect = chart->plotArea();
    zoomFactor = event->angleDelta().y() > 0 ? 0.75 : 1.25;
    QPointF c = chart->plotArea().center();

    switch (event->modifiers())
    {
    case Qt::ControlModifier:
    {

        rect.setWidth(zoomFactor * rect.width());
        rect.setHeight(zoomFactor * rect.height());
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

QList<CustomSeries *> ChartContainer::getSeriesInChart(void)
{
    return chartContent.values();
}

void ChartContainer::clearSelectedSeries(void)
{
    MessageQueue *q = MessageQueue::instance();
    q->addWarning("Unsupported: Please unselect trace in Tree.");
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
    update();
}
void ChartContainer::setLimits(void)
{
}

void ChartContainer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_crosshair->visible())
    {
        m_crosshair->updatePosition(event);
    }
    else
    {
        m_crosshair->setVisibilty(false);
    }

    if (middleMousePressed)
    {
        auto dPos = event->pos() - middlePressStartPos;
        chart->scroll(-dPos.x(), dPos.y());

        middlePressStartPos = event->pos();
        event->accept();
    }

    update();
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

void ChartContainer::mousePressEvent(QMouseEvent *event)
{

    switch (event->button())
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
        if (m_crosshair->visible())
        {

            requestNewMarker(event->pos());
            // emit markerRequested(event->pos());
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

    // setCursor(Qt::ArrowCursor);
}

void ChartContainer::requestNewMarker(QPointF pt)
{
    if (chartMarkers.length() < 9)
    {

        ChartMarker *marker = new ChartMarker(chart);
        chartMarkers.append(marker);
        marker->placeMarkerbyClick(pt, chartMarkers.length());

        foreach (auto &ser, chart->series())
        {
            if (reinterpret_cast<CustomSeries *>(ser)->isSelected())
            {
                // emit updateCursorData();
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

void ChartContainer::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::MiddleButton:
    {
        middleMousePressed = false;
        middlePressEndPos = event->pos();
        auto dx = middlePressStartPos.x() - middlePressEndPos.x();
        auto dy = middlePressEndPos.y() - middlePressStartPos.y();
        chart->scroll(dx, dy);
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
    // event->accept();
    // setCursor(Qt::ArrowCursor);
}

void ChartContainer::removeMarkers(void)
{
    foreach (auto &marker, chartMarkers)
    {
        marker->remove();
        marker->~ChartMarker();
    }
    chartMarkers.clear();
}

std::tuple<int, int> ChartContainer::getNumSeriesPerAxis(void)
{
    int l = 0, r = 0;
    foreach (auto &ser, chart->series())
    {
        foreach (auto &ax, ser->attachedAxes())
        {
            if (ax->alignment() == Qt::AlignRight)
            {
                r++;
            }
            if (ax->alignment() == Qt::AlignLeft)
            {
                l++;
            }
        }
    }
    return std::tie(l, r);
}

void ChartContainer::resizeEvent(QResizeEvent *event)
{
    foreach (auto &marker, chartMarkers)
    {
        marker->resizeEvent(event);
    }
    QChartView::resizeEvent(event);
}