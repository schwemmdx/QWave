#include "chartcontainer.h"


#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include "theme_colors.h"


#include <random>

ChartContainer::ChartContainer(QWidget *parent)
{
    series = new QLineSeries();

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0,3);

    switch(uni(rng))
    {
    case 0:
        //pFun = cos;
        series->setPen(QPen(Theme::Red,1));
        break;
    case 1:
        //pFun = cos;
        series->setPen(QPen(Theme::Green,1));
        break;
    case 2:
        //pFun = cos;
        series->setPen(QPen(Theme::Blue,1));
        break;
   default:
        break;
    }
    for(int i=0;i<50;i++)
    {

        *series<< QPointF(i,rand()%10);
    }

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setMargins(QMargins(5,5,5,5));
    chart->setBackgroundRoundness(10);
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

void ChartContainer::mousePressEvent(QMouseEvent* event)
{
    //current chart selected.
    setFocus();

    QFont font;
    font.setWeight(QFont::Weight::Bold);
    chart->setBackgroundBrush(QBrush(Theme::ForeGround.lighter()));
    chart->setTitleFont(font);
    QLineSeries* line = (QLineSeries*)chart->series()[0];
    QPen p = line->pen();
    p.setWidth(3);
    line->setPen(p);

    emit chartSelected(this);
}
void ChartContainer::deselect()
{

    QFont font;
    font.setWeight(QFont::Weight::Normal);
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setTitleFont(font);
}
