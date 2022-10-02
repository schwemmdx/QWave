#include "chartmarker.h"
#include "theme_colors.h"
#include <QtGui/QTextDocument>
#include "customseries.h"

ChartMarker::ChartMarker(QChart* chart):
    line(new QLineF()),
    m_line(new QGraphicsLineItem(chart)),
    m_text(new QGraphicsTextItem(chart)),
    m_upperText(new QGraphicsTextItem(chart)),
    m_markerNumber(new QGraphicsTextItem(chart))
{
    yValues.clear();
    pChart = chart;
    m_line->setLine(*line);
    m_line->hide();
    m_markerNumber->hide();
    m_text->hide();
    m_line->setPen(markerPen);
    m_line->setZValue(9999);

    m_markerNumber->setZValue(9999);
    m_markerNumber->document()->setDocumentMargin(5);
    m_markerNumber->setDefaultTextColor(Altium::LightText.lighter());
    m_markerNumber->setHtml(QString("<div style='background-color:"+markerRed.name()+" 'text-align=center;'><b>1</b></div>"));

    m_text->setZValue(9999);
    m_text->document()->setDocumentMargin(5);
    m_text->setDefaultTextColor(Altium::LightText.lighter());
    m_upperText->setZValue(9999);
    m_upperText->document()->setDocumentMargin(5);
    m_upperText->setDefaultTextColor(Altium::LightText.lighter());

}

void ChartMarker::placeMarkerbyClick(QPointF pos)
{
   
    foreach(auto &yVal,yValues)
    {
        delete yVal;
    }
    yValues.clear();

    line->setLine(pos.x(),pChart->plotArea().top(),pos.x(),pChart->plotArea().bottom());
    m_line->setLine(*line);

    m_text->setHtml(QString("<div style='background-color:"+markerRed.name()+";'><b>" + QString::number(pChart->mapToValue(pos).x()) + "</b></div>"));

    CustomSeries* pSerBuf;
    //QString txtBuf="";
    foreach (auto &trace, pChart->series()) {

        pSerBuf = static_cast<CustomSeries*>(trace);
        //txtBuf.append(QString::number(pChart->mapToValue(pos,pSerBuf).y())+ "<p>");
        QGraphicsTextItem* textItemBuf = new QGraphicsTextItem(pChart);
        textItemBuf->setHtml("<div 'text-align=right' line-height:'0%' style='background-color:"+
                             pSerBuf->pen().color().name()+" ;'>"+
                             QString::number(pChart->mapToValue(pos,pSerBuf).y())+
                             "</div>");
        yValues.append(textItemBuf);
    }
    int i = 5;
    foreach(auto &yVal,yValues)
    {
        yVal->setPos(pos.x()-3,pChart->plotArea().top()+i);
        i+=16;

    }

    //m_upperText->setHtml(QString("<div 'text-align=right' line-height:'0%' style='background-color:"+markerRed.name()+" ;'>"+txtBuf+"</div>"));
    //m_upperText->setPos(pos.x()-5 ,pChart->plotArea().top()+5);
    m_markerNumber->setPos(pos.x() - m_markerNumber->boundingRect().width() / 2.0, pChart->plotArea().top() - m_markerNumber->boundingRect().height()/2);


    m_text->setPos(pos.x() - m_text->boundingRect().width() / 2.0, pChart->plotArea().bottom()-m_text->boundingRect().height()/2);
    m_line->show();
    m_upperText->show();
    m_text->show();
    m_markerNumber->show();



}

