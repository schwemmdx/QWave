#include <QColor>
#include <QApplication>
#include <QtCharts/QLineSeries>

#include "ChartContainer.h"
#include "CustomChart.h"
#include "ThemeColors.h"
#include "CustomSeries.h"
#include "MessageQueue.h"

#include "ScientificFormatter.h"

#include <QGraphicsDropShadowEffect>

CustomChart::CustomChart(QObject *parent) : QChart(nullptr)
{

    pParent = parent;
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setAcceptHoverEvents(true);
    legend()->hide();

    // QBrush bgBrush(Monokai::Background);
    // setBackgroundBrush(bgBrush);
    setMargins(QMargins(5, 5, 5, 5));
    setBackgroundRoundness(10);
    setAnimationDuration(500);

    setupLogAxis();
    setupLinAxis();
    setLogXScale(false);
    setLogYLScale(false);

    // setAxisMode(AXIS_LIN);
}

CustomSeries *CustomChart::addDataSeries(QVector<double> x, QVector<double> y, QString xUnit, QString yUnit, Qt::Alignment align)
{
    // Create a new series
    CustomSeries *series = new CustomSeries(this);

    // Connect signals for status messages and trace selection
    connect(series, &CustomSeries::newStatusMessage, this, [this](QString msg)
            { emit newStatusMsg(msg); });
    connect(series, &CustomSeries::seriesSelected, this, [this](CustomSeries *pSeries)
            { emit newTraceSelection(pSeries); });

    // Prepare data points for the series
    QVector<QPointF> dataBuf;
    for (int i = 0; i < x.count(); i++)
    {
        dataBuf.append(QPointF(x[i], y[i]));
    }
    series->setData(dataBuf);

    // Set color for the new series
    int colorIndex = this->series().size(); // Assuming each series has a unique color
    QPen pen(neonColors[colorIndex % neonColors.size()]);
    series->setPen(pen);
    // Define a margin factor
    const double marginFactor = 0.1; // 10% margin

    // Calculate min and max values for x and y data
    double xMin = *std::min_element(x.constBegin(), x.constEnd());
    double xMax = *std::max_element(x.constBegin(), x.constEnd());
    double yMin = *std::min_element(y.constBegin(), y.constEnd());
    double yMax = *std::max_element(y.constBegin(), y.constEnd());

    // Calculate ranges and margins
    double xRange = xMax - xMin;
    double yRange = yMax - yMin;

    // Apply uniform margins based on the larger range
    double xMargin = marginFactor * xRange;
    double yMargin = marginFactor * yRange;

    // Calculate new min and max values for centered display
    double newXMin = (xMin + xMax) / 2 - (xRange / 2) - xMargin;
    double newXMax = (xMin + xMax) / 2 + (xRange / 2) + xMargin;

    double newYMin = (yMin + yMax) / 2 - (yRange / 2) - yMargin;
    double newYMax = (yMin + yMax) / 2 + (yRange / 2) + yMargin;
    MessageQueue *q = MessageQueue::instance();

    xAxisLin->setRange(newXMin, newXMax);
    yLeftLin->setRange(newYMin, newYMax);

    // Update logarithmic axes
    xAxisLog->setRange(qMax(newXMin, 1e-3), qMax(newXMax, 1e-3)); // Avoid zero or negative values
    yLeftLog->setRange(qMax(newYMin, 1e-3), qMax(newYMax, 1e-3)); // Avoid zero or negative values

    // Add the new series to the chart
    addSeries(series);

    // Attach the series to the correct y-axis based on `toAxis` parameter
    if (Qt::AlignLeft == align)
    { // Assuming 0 for left axis, 1 for right axis
        series->attachAxis(yLeftLin);
        series->attachAxis(xAxisLin); // Attach to x-axis as well
    }
    else if (Qt::AlignRight == align)
    {
        series->attachAxis(yRightLin);
        series->attachAxis(xAxisLin); // Attach to x-axis as well
    }

    // Update the chart view
    // update();
    return series;
}

void CustomChart::hideRYAxis()
{
    yRightLin->hide();
}

void CustomChart::setGridVisibility(int axis, bool visibility)
{
}

bool CustomChart::isSecondYaxisVisible()
{
    return secondYaxisPopulated;
}

// Helper function to apply consistent styles to axes
void CustomChart::applyAxisStyles(QAbstractAxis *axis, const QString &labelFormat, const QBrush &labelBrush, bool hideGridLines = true)
{
    if (auto valueAxis = qobject_cast<QValueAxis *>(axis))
    {
        valueAxis->setLabelFormat(labelFormat);
        valueAxis->setLinePen(QPen(Qt::black, 2));
        valueAxis->setLabelsBrush(labelBrush);
        valueAxis->setGridLineVisible(!hideGridLines); // Show/hide gridlines
    }
    else if (auto logAxis = qobject_cast<QLogValueAxis *>(axis))
    {
        logAxis->setLabelFormat(labelFormat);
        logAxis->setLinePen(QPen(Qt::black, 2));
        logAxis->setLabelsBrush(labelBrush);
        logAxis->setGridLineVisible(!hideGridLines);
        logAxis->setMinorGridLineVisible(!hideGridLines);
    }
}
void CustomChart::setupLinAxis()
{
    // Create linear axes
    yLeftLin = new QValueAxis;
    yRightLin = new QValueAxis;
    xAxisLin = new QValueAxis;

    // Define label brush for light theme
    QBrush labelBrush(Qt::black);

    // Set axis line pen (dark for light theme)
    QPen axisPen(Qt::black);
    axisPen.setWidthF(1.5);

    // Set grid pen (soft gray for light theme)
    QPen gridPen(QColor(200, 200, 200, 100)); // Light gray with transparency
    gridPen.setStyle(Qt::DotLine);            // Dotted for a sleek look

    // Set minor grid pen
    QPen minorGridPen(QColor(220, 220, 220, 80)); // Lighter gray
    minorGridPen.setStyle(Qt::DashLine);

    // Apply styles to yLeftLin
    yLeftLin->setLabelFormat("%2.0e"); // Exponential format
    yLeftLin->setLabelsBrush(labelBrush);
    yLeftLin->setLinePen(axisPen);
    yLeftLin->setGridLinePen(gridPen);
    yLeftLin->setMinorGridLinePen(minorGridPen);
    yLeftLin->setRange(-1, 1); // Default range

    // Apply styles to yRightLin
    yRightLin->setLabelFormat("%1.1e"); // Exponential format
    yRightLin->setLabelsBrush(labelBrush);
    yRightLin->setLinePen(axisPen);
    yRightLin->setGridLinePen(gridPen);
    yRightLin->setMinorGridLinePen(minorGridPen);
    yRightLin->hide(); // Auto-hide right axis

    // Apply styles to xAxisLin
    xAxisLin->setLabelFormat("%1.1e"); // Exponential format
    xAxisLin->setLabelsBrush(labelBrush);
    xAxisLin->setLinePen(axisPen);
    xAxisLin->setGridLinePen(gridPen);
    xAxisLin->setMinorGridLinePen(minorGridPen);
    xAxisLin->setTickCount(6); // 6 ticks for better spacing
    xAxisLin->setTitleBrush(labelBrush);
    xAxisLin->setLabelsAngle(0);         // Horizontal labels for readability
    xAxisLin->setGridLineVisible(false); // Hide grid lines on x-axis
    xAxisLin->setRange(-10, 10);         // Default range

    // Optional: Add axis titles
    QFont titleFont("Arial", 12, QFont::Bold); // Bold font for axis titles
    xAxisLin->setTitleFont(titleFont);
    yLeftLin->setTitleFont(titleFont);
    yRightLin->setTitleFont(titleFont);

    xAxisLin->setTitleText("X Axis"); // Example title
    yLeftLin->setTitleText("Y Axis"); // Example title

    // Enable smooth transitions
    setAnimationOptions(QChart::SeriesAnimations);

    // Set chart drop shadow for depth
    setBackgroundRoundness(5); // Rounded corners for light theme
}

void CustomChart::setupLogAxis()
{
    // Create logarithmic axes
    yLeftLog = new QLogValueAxis;
    yRightLog = new QLogValueAxis;
    xAxisLog = new QLogValueAxis;

    // Set label format, base, brush, and general styling
    QBrush labelBrush(Altium::HighLight2.lighter());
    applyAxisStyles(yLeftLog, "%1.0e", labelBrush);
    applyAxisStyles(yRightLog, "%1.0e", labelBrush);
    applyAxisStyles(xAxisLog, "%1.0e", labelBrush);

    // Additional settings specific to logarithmic axes
    yRightLog->hide();                        // Auto-hide right axis on setup
    xAxisLog->setMinorTickCount(5);           // Set minor tick count
    xAxisLog->setBase(10);                    // Set logarithmic base for x-axis
    yLeftLog->setBase(10);                    // Set logarithmic base for y-axis
    xAxisLog->setMinorGridLineVisible(false); // Hide minor grid lines
    xAxisLog->setRange(1e-3, 1e3);
    xAxisLog->setLinePen(QPen(Qt::black, 1.5));
    yLeftLog->setLinePen(QPen(Qt::black, 1.5));
    yRightLog->setLinePen(QPen(Qt::black, 1.5));
}

void CustomChart::replaceAxis(QAbstractAxis *oldAx, QAbstractAxis *newAx, enum Qt::AlignmentFlag align)
{
    // Remove the old axis from the chart and replace it with the new one

    // if its a horizontal axis, its a x axis
    auto axesOfChart = axes();
    for (auto &ax : axesOfChart)
    {
        if (ax == oldAx)
        {
            removeAxis(oldAx);
        }
    }

    addAxis(newAx, align);

    for (auto serie : series())
    {
        if (serie->attachedAxes().contains(oldAx))
            serie->detachAxis(oldAx);
        serie->attachAxis(newAx);
    }
}

void CustomChart::setLogYLScale(bool useLog)
{
    if (useLog)
    {
        // Switch to linear left y-axis
        replaceAxis(yLeftLin, yLeftLog, Qt::AlignLeft);
    }
    else
    {
        // Switch to log left y-axis
        replaceAxis(yLeftLog, yLeftLin, Qt::AlignLeft);
    }
}

void CustomChart::setLogYRScale(bool arg)
{ /*
  if (useLogRightY) {
         // Switch to linear right y-axis
         removeAxis(logRightYAxis);
         addAxis(linearRightYAxis, Qt::AlignRight);

         // Update all series to use the linear right y-axis
         for (auto *series : series()) {
             series->detachAxis(logRightYAxis);
             series->attachAxis(linearRightYAxis);
         }

         useLogRightY = false;
     } else {
         // Switch to log right y-axis
         removeAxis(linearRightYAxis);
         addAxis(logRightYAxis, Qt::AlignRight);

         // Update all series to use the log right y-axis
         for (auto *series : series()) {
             series->detachAxis(linearRightYAxis);
             series->attachAxis(logRightYAxis);
         }

         useLogRightY = true;
     }*/
}

void CustomChart::setLogXScale(bool useLog)
{
    useLogX = useLog;
    if (useLog)
    {
        replaceAxis(xAxisLin, xAxisLog, Qt::AlignBottom);
    }
    else
    {
        replaceAxis(xAxisLog, xAxisLin, Qt::AlignBottom);
        // xAxisLin->applyNiceNumbers();
    }
}
