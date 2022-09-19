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
    series = new CustomSeries(this);
    middleMousePressed = false;
    chart = new QChart();
    //chart->legend()->hide();
    QBrush bgBrush(Altium::BackGround2);
    chart->setBackgroundBrush(bgBrush);

    // chart->addSeries(series);
    // tracies.append(series);

    //chart->createDefaultAxes();
    chart->setMargins(QMargins(5, 5, 5, 5));
    chart->setBackgroundRoundness(5);

    setChart(chart);

    setRenderHint(QPainter::Antialiasing);

    setAlignment(Qt::AlignLeft);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setRubberBand(QChartView::NoRubberBand);

    contextMenu = new QMenu(this);
    contextMenu->addAction("Set Limits", this, &ChartContainer::setLimits);
    contextMenu->addAction("Reset Zoom", this, &ChartContainer::resetZoom);
    contextMenu->addSeparator();
    contextMenu->addAction("Remove Selected", this, &ChartContainer::clearSelectedSeries);
    contextMenu->addAction("Clear Chart", this, &ChartContainer::clearAllSeries);

    chart->setAnimationDuration(250);

    m_crosshair = new ChartCrosshair(chart);

    connect(series, &CustomSeries::seriesSelected, this, &ChartContainer::selectedSeriesChanged);
    connect(series, &CustomSeries::newStatusMessage, this, &ChartContainer::newMsgFromSeries);
    connect(this, &ChartContainer::customContextMenuRequested, this, &ChartContainer::onCustomContextMenu);
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

    series = new CustomSeries(this);

    auto xMin = std::min(x.begin(), x.end());
    auto xMax = std::max(x.begin(), x.end());
    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();
    foreach (auto &p, y)
    {
        if (p < yMin)
        {
            yMin = p;
        }
        if (p > yMax)
        {
            yMax = p;
        }
    }

    qDebug() << "min: " << yMin << "max: " << yMax;
    QPointF ptBuf;
    QVector<QPointF> dataBuf;
    for (uint i = 0; i < x.count(); i++)
    {
        ptBuf.setX(x[i]);
        ptBuf.setY(y[i]);
        dataBuf.append(ptBuf);
    }

    chart->update();

    // chart->setPlotArea(area);

    QValueAxis *axisY = new QValueAxis;
    QValueAxis *axisX = new QValueAxis;

    axisY->setRange(yMin, yMax);
    axisX->setRange(*xMin, *xMax);

    series->setData(dataBuf);
    chart->addSeries(series);
    QPen pen;
    QBrush txtBrush,xBrush;
    xBrush.setColor(Altium::LightText);
    pen.setColor(series->pen().color());
    txtBrush.setColor(pen.color());

    pen.setWidth(2);
    axisY->setLinePen(pen);
    axisY->setTitleText(yUnit);
    axisX->setTitleText(xUnit);
    axisY->setTitleBrush(txtBrush);
    axisY->setLabelsAngle(45);
    axisY->setLabelsBrush(txtBrush);
    axisY->applyNiceNumbers();
    axisX->applyNiceNumbers();
    axisX->setTitleBrush(xBrush);
    axisX->setLabelsBrush(xBrush);
    axisY->setGridLineVisible();
    axisX->setGridLineVisible();
    axisX->setGridLineColor(Altium::BackGround.lighter(300));
    axisY->setGridLineColor(Altium::BackGround.lighter(300));




    // axisY->setLinePen(pen);
    chart->addAxis(axisY, Qt::AlignLeft);
    if (chart->axes(Qt::Orientation::Horizontal).length() == 0)
    {
        chart->addAxis(axisX, Qt::AlignBottom);
    }

    series->attachAxis(axisX);
    series->attachAxis(axisY);
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
    }
}

bool ChartContainer::isSelectedContainer(void)
{
    bool selectionState = false;
    foreach (auto &trace, chart->series())
    {
        if (reinterpret_cast<CustomSeries *>(trace)->isSelected())
        {
            selectionState = true;
            break;
        }
    }
    return selectionState;
}

void ChartContainer::onCustomContextMenu(const QPoint &point)
{
    if (rubberBand() == QChartView::NoRubberBand)
    {
        contextMenu->exec(this->viewport()->mapToGlobal(point));
    }
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
        foreach (auto &ser, chart->series())
        {
            foreach (auto &ax, ser->attachedAxes())
            {
                chart->removeAxis(ax);
            }
        }
        chart->removeAllSeries();
    }
}

void ChartContainer::resetZoom(void)
{
    chart->zoomReset();
}
void ChartContainer::setLimits(void)
{

}

void ChartContainer::changeRubberBandBehaviour(QChartView::RubberBand rb)
{
    setRubberBand(rb);
}

void ChartContainer::mouseMoveEvent(QMouseEvent *event)
{
   m_crosshair->updatePosition(event); 
}

void ChartContainer::setCrosshairVisibility(bool vis)
{
    m_crosshair->setVisibilty(vis);
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
        default:
        {
        }
    }
    event->accept();
    //setCursor(Qt::ArrowCursor);
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
            break;
        }
        default:
        {
        }
    }
    event->accept();
    //setCursor(Qt::ArrowCursor);
}
