#include "customseries.h"
#include "mainwindow.h"
#include <QKeyEvent>

CustomSeries::CustomSeries(QObject* parent)
{
    connect(this, &QXYSeries::clicked, this, &CustomSeries::clicked);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0,3);

    switch(uni(rng))
    {
    case 0:
        //pFun = cos;
        setPen(QPen(Theme::Red,1));
        break;
    case 1:
        //pFun = cos;
        setPen(QPen(Theme::Green,1));
        break;
    case 2:
        //pFun = cos;
        setPen(QPen(Theme::Blue,1));
        break;
   default:
        break;
    }
    for(int i=0;i<50;i++)
    {

        *this<< QPointF(i,rand()%10);
    }


}

void CustomSeries::unselect()
{
    QPen usedPen = pen();
    usedPen.setWidth(1);
    setPen(usedPen);

}

void CustomSeries::clicked(void)
{

    QPen usedPen = pen();
    usedPen.setWidth(3);
    setPen(usedPen);
    emit seriesSelected(this);

}

