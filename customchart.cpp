#include "customchart.h"
#include <QColor>
#include "customseries.h"
#include "chartcontainer.h"
#include "theme_colors.h"
#include <QApplication>


#include <QtCharts/QLineSeries>

CustomChart::CustomChart(QObject* parent):QChart(nullptr)
{
    pParent = parent;
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton );
    setAcceptHoverEvents(true);
    legend()->hide();

    QBrush bgBrush(Altium::BackGround.darker());
    setBackgroundBrush(bgBrush);
    setMargins(QMargins(1,1,1,1));
    setBackgroundRoundness(5);
    setAnimationDuration(100);

    yAxisLeft = new QValueAxis;
    yAxisRight = new QValueAxis;
    xAxis = new QValueAxis;

//Basic Axis
    addAxis(yAxisLeft,Qt::AlignLeft);
    addAxis(xAxis,Qt::AlignBottom);
    addAxis(yAxisRight,Qt::AlignRight);
    yAxisRight->hide();



}

void CustomChart::addDataSeries(QVector<double> x, QVector<double> y, QString xUnit, QString yUnit,int toAxis)
{
    CustomSeries* series =  new CustomSeries(this);


    connect(series,&CustomSeries::newStatusMessage, this,[this](QString msg){emit newStatusMsg(msg);});
    connect(series,&CustomSeries::seriesSelected, this, [this](CustomSeries* pSeries){emit newTraceSelection(pSeries);});

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

    update();
    xAxis->setRange(*xMin,*xMax);
    xAxis->setMax(*xMax);
    xAxis->setMax(*xMin);

    series->setData(dataBuf);
    addSeries(series);

    QPen pen;
    QBrush txtBrush,xBrush;
    xBrush.setColor(Altium::HighLight2.lighter());
    
    pen.setColor(series->pen().color());
    txtBrush.setColor(pen.color());

    //this->leftYAxis->setLinePen(pen);

    xAxis->setTitleText(xUnit);
    xAxis->applyNiceNumbers();
    xAxis->setTitleBrush(xBrush);
    xAxis->setLabelsBrush(xBrush);
    xAxis->setGridLineVisible(false);

    //xAxis->setGridLineColor(Altium::HighLight2);
    xAxis->setLinePen(QPen(Altium::HighLight2,2));
    yAxisRight->setLinePen(QPen(Altium::HighLight2,2));
    yAxisLeft->setLinePen(QPen(Altium::HighLight2,2));
    //axisY->setGridLineColor(Altium::BackGround.lighter(300));

    QValueAxis* axisToAttachTo = nullptr;
    series->attachAxis(xAxis);

    if(toAxis == 0)
    {
        axisToAttachTo = yAxisLeft;
    }
    else
    {
        yAxisRight->show();
        axisToAttachTo = yAxisRight;

    }
    series->attachAxis(axisToAttachTo);
    axisToAttachTo->setTitleText(yUnit);
    axisToAttachTo->setTitleBrush(xBrush);
    axisToAttachTo->setLabelsAngle(45);
    axisToAttachTo->setLabelsBrush(xBrush);
    axisToAttachTo->applyNiceNumbers();
    axisToAttachTo->setGridLineVisible(false);
    axisToAttachTo->setMax(*yMax);
    axisToAttachTo->setMin(*yMin);



}

void CustomChart::hideRYAxis()
{
    yAxisRight->hide();
}

void CustomChart::setGridVisibility(int axis,bool visibility)
{
    switch(axis)
    {
        case 0:
            xAxis->setGridLineVisible(visibility);
            break;
        case 1:
            yAxisLeft->setGridLineVisible(visibility);
            break;
        case 2:
            yAxisLeft->setGridLineVisible(visibility);
        default:
        break;

    }
}
/*
void CustomChart::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "MousePressEvent from Chart!";

}

void CustomChart::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
     qDebug() << "MouseDoubleClickEvent from Chart!";
}
void CustomChart::mouseMoveEvent(QGraphicsSceneMouseEvent*)
{
    qDebug() << "MouseMoveEvent from Chart!";
}
*/
