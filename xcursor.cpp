#include "xcursor.h"
#include <QtGui/QTextDocument>
#include "customseries.h"

XCursor::XCursor(QChart* chart):
    line(new QLineF()),
    m_line(new QGraphicsLineItem(chart)),
    m_text(new QGraphicsTextItem(chart)),
    m_upperText(new QGraphicsTextItem(chart)),
    m_markerNumber(new QGraphicsTextItem(chart))
{
    pChart= chart;
    m_line->setPen(markerPen);
    m_markerNumber->setZValue(9999);
    m_line->setLine(*line);
    m_line->hide();
    m_markerNumber->hide();
    m_markerNumber->document()->setDocumentMargin(5);
    m_markerNumber->setDefaultTextColor(Altium::BackGround.darker());

    m_markerNumber->setScale(1.5);
}

void XCursor::placeByClick(QPointF pos,int num)
{
    line->setLine(pos.x(),pChart->plotArea().top(),pos.x(),pChart->plotArea().bottom());
    m_line->setLine(*line);
    m_markerNumber->setHtml(QString("<div style='background-color:"+markerRed.name()+" 'text-align=center;'><b>X "+QString::number(num)+"</b></div>"));
    m_line->show();
    m_markerNumber->show();
    m_markerNumber->setPos(pos.x() - 1.5*m_markerNumber->boundingRect().width() / 2.0, pChart->plotArea().top() - m_markerNumber->boundingRect().height()/2);

    foreach(auto &ser,pChart->series())
    {
        if(reinterpret_cast<CustomSeries*>(ser)->isSelected())
        {
            mappedPosition = pChart->mapToPosition(pos,ser).x();
        }
    }

}

void XCursor::remove()
{
    m_line->hide();
    m_markerNumber->hide();
    //delete this;
}

double XCursor::getPos()
{
    return mappedPosition;
}
