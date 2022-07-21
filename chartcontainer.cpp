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
