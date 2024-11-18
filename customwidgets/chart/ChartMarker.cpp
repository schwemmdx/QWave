#include <QtGui/QTextDocument>

#include "ThemeColors.h"
#include "ScientificFormatter.h"
#include "CustomSeries.h"
#include "ChartMarker.h"


ChartMarker::ChartMarker(QChart* chart):
    line(new QLineF()),
    m_line(new QGraphicsLineItem(chart)),
    m_text(new QGraphicsTextItem(chart)),
    m_upperText(new QGraphicsTextItem(chart)),
    m_markerNumber(new QGraphicsTextItem(chart))
    //bRect(line->x1()-5,pChart->boundingRect().bottom(),10,pChart->boundingRect().height())
    
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
    m_markerNumber->setDefaultTextColor(Altium::BackGround.darker());


    m_text->setZValue(9999);

    m_text->document()->setDocumentMargin(5);
    m_text->setDefaultTextColor(Altium::BackGround.darker());
    m_upperText->setZValue(9999);
    m_upperText->document()->setDocumentMargin(5);
    m_upperText->setDefaultTextColor(Altium::BackGround.darker());

   

    m_text->setScale(1.1);
    m_upperText->setScale(1.1);
    m_markerNumber->setScale(1.5);
    //setAcceptHoverEvents(true);
    //setCursor(Qt::SizeHorCursor);

}

void ChartMarker::placeMarkerbyClick(QPointF pos,int numExisting)
{

    foreach(auto &yVal,yValues)
    {
        delete yVal;
    }
    yValues.clear();

    line->setLine(pos.x(),pChart->plotArea().top(),pos.x(),pChart->plotArea().bottom());
    m_line->setLine(*line);

    m_text->setHtml(QString("<div style='background-color:"+markerRed.name()+";'><b>" + ScientificFormatter::toScientificSuffix(pChart->mapToValue(pos).x(),4) + "</b></div>"));
     m_markerNumber->setHtml(QString("<div style='background-color:"+markerRed.name()+" 'text-align=center;'><b>"+QString::number(numExisting)+"</b></div>"));
    CustomSeries* pSerBuf;
    //QString txtBuf="";
    foreach (auto &trace, pChart->series()) {

        pSerBuf = static_cast<CustomSeries*>(trace);
        //txtBuf.append(QString::number(pChart->mapToValue(pos,pSerBuf).y())+ "<p>");
        QGraphicsTextItem* textItemBuf = new QGraphicsTextItem(pChart);
        textItemBuf->setDefaultTextColor(Altium::BackGround.darker());
        int intersectIndex = pSerBuf->findClosestPointIndex(pChart->mapToValue(pos,pSerBuf));
     
        textItemBuf->setHtml("<div 'text-align=right' line-height:'0%' style='background-color:"+
                             pSerBuf->pen().color().name()+";'>"+
                             ScientificFormatter::toScientificSuffix(pSerBuf->at(intersectIndex).y(),4)+
                             "</div>");
        //qDebug() << "Clicked : " << pos << " Idx: "<< intersectIndex << " mapped y-val: "<< pSerBuf->at(intersectIndex) << "\n";              
                            
        yValues.append(textItemBuf);
        if(pSerBuf->isSelected())
        {
            intersectPoints.append(pChart->mapToValue(pos,pSerBuf));
        }

    }
    int i = 20;
    foreach(auto &yVal,yValues)
    {
        yVal->setPos(pos.x()-3,pChart->plotArea().top()+i);
        i+=16;

    }

    //m_upperText->setHtml(QString("<div 'text-align=right' line-height:'0%' style='background-color:"+markerRed.name()+" ;'>"+txtBuf+"</div>"));
    //m_upperText->setPos(pos.x()-5 ,pChart->plotArea().top()+5);
    m_markerNumber->setPos(pos.x() - 1.5*m_markerNumber->boundingRect().width() / 2.0, pChart->plotArea().top() - m_markerNumber->boundingRect().height()/2);


    m_text->setPos(pos.x() - m_text->boundingRect().width() / 2.0, pChart->plotArea().bottom()-m_text->boundingRect().height()/2);
    m_line->show();
    m_upperText->show();
    m_text->show();
    m_markerNumber->show();
}

bool ChartMarker::isUnderMouse(QMouseEvent* e)
{
    return e->pos().x() == line->x1();
}


QRectF ChartMarker::boundingRect() const
{
    return QRectF(line->x1()-10,pChart->boundingRect().bottom(),10,pChart->boundingRect().height());
}

void ChartMarker::remove(void)
{
    m_line->hide();
    m_markerNumber->hide();
    m_upperText->hide();
    m_text->hide();
    foreach(auto &item,yValues)
    {
        item->hide();
    }


    //this->~ChartMarker();

}


