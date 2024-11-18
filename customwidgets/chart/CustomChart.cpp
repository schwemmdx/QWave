#include <QColor>
#include <QApplication>
#include <QtCharts/QLineSeries>


#include "ChartContainer.h"
#include "CustomChart.h"
#include "ThemeColors.h"
#include "CustomSeries.h"
#include "MessageQueue.h"

#include "ScientificFormatter.h"

CustomChart::CustomChart(QObject* parent):QChart(nullptr)
{
    pParent = parent;
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton );
    setAcceptHoverEvents(true);
    legend()->hide();

    //QBrush bgBrush(Monokai::Background);
    //setBackgroundBrush(bgBrush);
    setMargins(QMargins(5,5,5,5));
    setBackgroundRoundness(10);
    setAnimationDuration(500);


    setupLogAxis();
    setupLinAxis();
    setLogXScale(false);
    setLogYLScale(false);

    //setAxisMode(AXIS_LIN);

}
void CustomChart::addDataSeries(QVector<double> x, QVector<double> y, QString xUnit, QString yUnit, int toAxis) {
    // Create a new series
    CustomSeries* series = new CustomSeries(this);

    // Connect signals for status messages and trace selection
    connect(series, &CustomSeries::newStatusMessage, this, [this](QString msg) { emit newStatusMsg(msg); });
    connect(series, &CustomSeries::seriesSelected, this, [this](CustomSeries* pSeries) { emit newTraceSelection(pSeries); });

    // Prepare data points for the series
    QVector<QPointF> dataBuf;
    for (int i = 0; i < x.count(); i++) {
        dataBuf.append(QPointF(x[i], y[i]));
    }
    series->setData(dataBuf);

    // Set color for the new series
    int colorIndex = this->series().size();  // Assuming each series has a unique color
    QPen pen(neonColors[colorIndex % neonColors.size()]);
    series->setPen(pen);

    // Calculate min and max values for x and y data
    double xMin = *std::min_element(x.constBegin(), x.constEnd());
    double xMax = *std::max_element(x.constBegin(), x.constEnd());
    double yMin = *std::min_element(y.constBegin(), y.constEnd());
    double yMax = *std::max_element(y.constBegin(), y.constEnd());

    // Update linear and logarithmic x-axis ranges
    xAxisLin->setRange(xMin - 0.1 * qAbs(xMin), xMax + 0.1 * qAbs(xMax));  // Linear x-axis
    xAxisLog->setRange(qMax(xMin, 1e-3), xMax * 1.1);  // Logarithmic x-axis; avoid zero or negative

    // Update linear and logarithmic y-axis ranges
    yLeftLin->setRange(yMin - 0.1 * qAbs(yMin), yMax + 0.1 * qAbs(yMax));  // Linear y-axis
    yLeftLog->setRange(qMax(yMin, 1e-3), yMax * 1.1);  // Logarithmic y-axis; avoid zero or negative
    qDebug() << "Linear\nmin: "<< xMin - 0.1 * qAbs(xMin) << ", max: " << xMax + 0.1 * qAbs(xMax) << "\n";
    qDebug() << "Log\nmin: "<< qMax(xMin, 1e-3) << ", max: " <<  xMax * 1.1 << "\n";
    
    // Add the new series to the chart
    addSeries(series);

    // Attach the series to the correct y-axis based on `toAxis` parameter
    if (toAxis == 0) {  // Assuming 0 for left axis, 1 for right axis
        series->attachAxis(yLeftLin);
        series->attachAxis(xAxisLin);  // Attach to x-axis as well
    } else if (toAxis == 1) {
        series->attachAxis(yRightLin);
        series->attachAxis(xAxisLin);  // Attach to x-axis as well
    }

    // Update the chart view
    update();
    
}


void CustomChart::hideRYAxis()
{
    yRightLin->hide();
}

void CustomChart::setGridVisibility(int axis,bool visibility)
{

}


bool CustomChart::isSecondYaxisVisible()
{
    return secondYaxisPopulated;
}

// Helper function to apply consistent styles to axes
void CustomChart::applyAxisStyles(QAbstractAxis* axis, const QString& labelFormat, const QBrush& labelBrush, bool hideGridLines = true) {
    if (auto valueAxis = qobject_cast<QValueAxis*>(axis)) {
        valueAxis->setLabelFormat(labelFormat);
        valueAxis->setLinePen(QPen(Qt::black, 2));
        valueAxis->setLabelsBrush(labelBrush);
        valueAxis->setGridLineVisible(!hideGridLines);  // Show/hide gridlines

    } else if (auto logAxis = qobject_cast<QLogValueAxis*>(axis)) {
        logAxis->setLabelFormat(labelFormat);
        logAxis->setLinePen(QPen(Qt::black, 2));
        logAxis->setLabelsBrush(labelBrush);
        logAxis->setGridLineVisible(!hideGridLines);
        logAxis->setMinorGridLineVisible(!hideGridLines);
    }
}

void CustomChart::setupLinAxis() {
    // Create linear axes
    yLeftLin = new QValueAxis;
    yRightLin = new QValueAxis;
    xAxisLin = new QValueAxis;

    // Set label format, brush, and general styling
    QBrush labelBrush(Qt::black);
    applyAxisStyles(yLeftLin, "%2.0e", labelBrush);
    applyAxisStyles(yRightLin, "%1.1e", labelBrush);
    applyAxisStyles(xAxisLin, "%1.1e", labelBrush);

    // Additional settings specific to linear axes
    yRightLin->hide();  // Auto-hide right axis on setup
    xAxisLin->setTickCount(6);
    xAxisLin->setTitleBrush(labelBrush);
    xAxisLin->setLabelsAngle(0);
    xAxisLin->setLinePen(QPen(Qt::black, 1.5));
    yLeftLin->setLinePen(QPen(Qt::black, 1.5));
    yRightLin->setLinePen(QPen(Qt::black, 1.5));
    xAxisLin->setGridLineVisible(false);  // Hides grid lines on x-axis
    xAxisLin->setRange(-10,10);
    yLeftLin->setRange(-1,1);

}

void CustomChart::setupLogAxis() {
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
    yRightLog->hide();  // Auto-hide right axis on setup
    xAxisLog->setMinorTickCount(5);  // Set minor tick count
    xAxisLog->setBase(10);           // Set logarithmic base for x-axis
    yLeftLog->setBase(10);           // Set logarithmic base for y-axis
    xAxisLog->setMinorGridLineVisible(false);  // Hide minor grid lines
    xAxisLog->setRange(1e-3,1e3);
    xAxisLog->setLinePen(QPen(Qt::black, 1.5));
    yLeftLog->setLinePen(QPen(Qt::black, 1.5));
    yRightLog->setLinePen(QPen(Qt::black, 1.5));
}

void CustomChart::replaceAxis(QAbstractAxis* oldAx, QAbstractAxis* newAx, enum Qt::AlignmentFlag align) {
    // Remove the old axis from the chart and replace it with the new one

    //if its a horizontal axis, its a x axis
    auto axesOfChart = axes();
   for(auto &ax: axesOfChart)
   {
        if(ax == oldAx)
        {
            removeAxis(oldAx);
        }

    }
    
    addAxis(newAx,align);

    for(auto serie: series()){
        if(serie->attachedAxes().contains(oldAx))
            serie->detachAxis(oldAx);
        serie->attachAxis(newAx);
    }
   
}

void CustomChart::setLogYLScale(bool useLog) {
  if (useLog) {
        // Switch to linear left y-axis
        replaceAxis(yLeftLin,yLeftLog,Qt::AlignLeft);
        
        
    } else {
        // Switch to log left y-axis
       replaceAxis(yLeftLog,yLeftLin,Qt::AlignLeft);
    }
    
}

void CustomChart::setLogYRScale(bool arg)
{/*
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
   if(useLog){
    replaceAxis(xAxisLin,xAxisLog,Qt::AlignBottom);
   }
   else{
    replaceAxis(xAxisLog,xAxisLin,Qt::AlignBottom);
    //xAxisLin->applyNiceNumbers();
   }
}

  