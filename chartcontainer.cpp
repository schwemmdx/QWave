#include "chartcontainer.h"


#include <QColor>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include "theme_colors.h"

ChartContainer::ChartContainer(QWidget *parent)
{
    series = new QLineSeries();
    //series->setPen(QPen(Theme::Red,3));
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
    emit chartSelected(this);
}
void ChartContainer::deselect()
{

    QFont font;
    font.setWeight(QFont::Weight::Normal);
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setTitleFont(font);
}
