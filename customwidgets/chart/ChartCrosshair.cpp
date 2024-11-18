
#include <QtCharts/QChart>
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtGui/QTextDocument>
#include <QWidget>

#include "ThemeColors.h"
#include "ChartCrosshair.h"
#include "ChartContainer.h"
#include "ScientificFormatter.h"

ChartCrosshair::ChartCrosshair(CustomChart *chart) :
    m_xLine(new QGraphicsLineItem(chart)),
    m_yLine(new QGraphicsLineItem(chart)),
    m_xText(new QGraphicsTextItem(chart)),
    m_yLeftText(new QGraphicsTextItem(chart)),
    m_yRightText(new QGraphicsTextItem(chart)),
    m_chart(chart)
{

    //m_chart->setCursor(QCursor(Qt::CursorShape::CrossCursor));
    m_xLine->setPen(QPen(Altium::LightText, 0.75));
    m_yLine->setPen(QPen(Altium::LightText, 0.75));
    m_xText->setZValue(9999);
    m_yLeftText->setZValue(9999);
    m_yRightText->setZValue(9999);

    m_xText->document()->setDocumentMargin(5);
    m_yLeftText->document()->setDocumentMargin(5);
    m_yRightText->document()->setDocumentMargin(5);
    m_xText->setDefaultTextColor(Altium::BackGround2.darker());
    m_yLeftText->setDefaultTextColor(Altium::BackGround2.darker());
    m_yRightText->setDefaultTextColor(Altium::BackGround2.darker());

    xLine = new QLineF();
    yLine = new QLineF();
    m_xLine->setLine(*xLine);
    m_yLine->setLine(*yLine);

    m_xLine->setZValue(9999);
    m_yLine->setZValue(9999);
    visibility = false;
    
    m_yRightText->hide();

}

void ChartCrosshair::updatePosition(QMouseEvent* event)
{
    auto position(event->pos());
    

    QLineF xLine(position.x(), m_chart->plotArea().top(),
                position.x(), m_chart->plotArea().bottom());
    QLineF yLine(m_chart->plotArea().left(), position.y(),
                m_chart->plotArea().right(), position.y());

    m_xLine->setLine(xLine);

    m_yLine->setLine(yLine);

    QString xText = ScientificFormatter::toScientificSuffix(m_chart->mapToValue(position).x());
    QString yLeftText = ScientificFormatter::toScientificSuffix(m_chart->mapToValue(position).y());
    QString yRightText = ScientificFormatter::toScientificSuffix(m_chart->mapToValue(position).y());

    
    //hex color for Altium::Highlight = 6482a0
    m_xText->setHtml(QString("<div style='background-color:"
    + Altium::HighLight2.name()+";'><b>") + xText + "</b></div>");

    m_yLeftText->setHtml(QString("<div style='background-color: "
    + Altium::HighLight2.name()+";'><b>") + yLeftText + "</b></div>");

    m_yRightText->setHtml(QString("<div style='background-color: "
    + Altium::HighLight2.name()+";'><b>") + yLeftText + "</b></div>");


    m_xText->setPos(position.x() - m_xText->boundingRect().width() / 
    2.0, m_chart->plotArea().bottom()-m_xText->boundingRect().height()/2);

    m_yLeftText->setPos(m_chart->plotArea().left()-10, position.y() - 
    m_yLeftText->boundingRect().height() / 2.0);

    m_yRightText->setPos(m_chart->plotArea().right()-(yRightText.length()*10+10), position.y() - 
    m_yRightText->boundingRect().height() / 2.0);

    if (m_chart->plotArea().contains(position) && 
        visibility)
    {  
        
        m_chart->setCursor(crosshairCursor);
        m_xLine->show();
        m_xText->show();
        m_yLine->show();
        m_yLeftText->show();
        if(m_chart->isSecondYaxisVisible())
        {
            m_yRightText->show();
        }
        else
        {
              m_yRightText->hide();
        }
        

    }
        
    else
    {
        
        m_chart->setCursor(Qt::ArrowCursor);
        m_xLine->hide();
        m_xText->hide();
        m_yLine->hide();
        m_yLeftText->hide();  
        m_yRightText->hide();    
    }
    m_chart->update();


    
}


void ChartCrosshair::setVisibilty(bool vis)
{
    visibility = vis;
    if(!vis)
    {
        m_chart->setCursor(Qt::ArrowCursor);
        m_xLine->hide();
        m_xText->hide();
        m_yLine->hide();
        m_yLeftText->hide();
        m_yRightText->hide();
    }
    else
    {
        m_chart->setCursor(crosshairCursor);
        m_xLine->show();
        m_xText->show();
        m_yLine->show();
        m_yLeftText->show();
        m_yRightText->show();
    }

}

bool ChartCrosshair::visible(void)
{
    return visibility;
}
