#include "dockchart.h"
#include "ui_dockchart.h"

#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include "theme_colors.h"


DockChart::DockChart(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockChart)
{
    ui->setupUi(this);
    //actual "line" on chart
    series = new QLineSeries();
    series->setPen(QPen(Theme::Red,3));
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
    //chart object
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setMargins(QMargins(10,10,10,10));

    //chart->setTitle("Simple line chart example");
    chartView = new QChartView(chart);
    chartView->setContentsMargins(QMargins(0,0,0,0));
    chartView->setLineWidth(3);
    chartView->setRenderHint(QPainter::Antialiasing);

    //sick chart styling
    chart->setBackgroundBrush(QBrush(Theme::BackGround2.lighter()));
    chart->setBackgroundRoundness(20);
    setWidget(chartView);
}

DockChart::~DockChart()
{
    delete ui;
}

void DockChart::highlight(bool state)
{
    if(state)
    {
        chartView->setBackgroundBrush(QBrush(Theme::BackGround2.lighter().lighter()));
    }
    else
    {
        chart->setBackgroundBrush(QBrush(Theme::BackGround2.lighter()));
    }
}

void DockChart::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Mouse Press Detected";
}
