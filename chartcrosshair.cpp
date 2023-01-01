
#include "chartcrosshair.h"
#include <QtCharts/QChart>
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtGui/QTextDocument>
#include <QWidget>

#include "theme_colors.h"



ChartCrosshair::ChartCrosshair(QChart *chart) :
    m_xLine(new QGraphicsLineItem(chart)),
    m_yLine(new QGraphicsLineItem(chart)),
    m_xText(new QGraphicsTextItem(chart)),
    m_yText(new QGraphicsTextItem(chart)),
    m_chart(chart)
{

    //m_chart->setCursor(QCursor(Qt::CursorShape::CrossCursor));
    m_xLine->setPen(QPen(Altium::HighLight2, 2));
    m_yLine->setPen(QPen(Altium::HighLight2, 2));
    m_xText->setZValue(9999);
    m_yText->setZValue(9999);
    m_xText->document()->setDocumentMargin(5);
    m_yText->document()->setDocumentMargin(5);
    m_xText->setDefaultTextColor(Altium::BackGround2.darker());
    m_yText->setDefaultTextColor(Altium::BackGround2.darker());

    xLine = new QLineF();
    yLine = new QLineF();
    m_xLine->setLine(*xLine);
    m_yLine->setLine(*yLine);
    m_xLine->setZValue(9999);
    m_yLine->setZValue(9999);
    visibility = false;

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

    QString xText = QString("%1").arg(m_chart->mapToValue(position).x());
    QString yText = QString("%1").arg(m_chart->mapToValue(position).y());

    //hex color for Altium::Highlight = 6482a0
    m_xText->setHtml(QString("<div style='background-color:"+ Altium::HighLight2.name()+";'><b>") + xText + "</b></div>");
    m_yText->setHtml(QString("<div style='background-color: "+ Altium::HighLight2.name()+";'><b>") + yText + "</b></div>");
    m_xText->setPos(position.x() - m_xText->boundingRect().width() / 2.0, m_chart->plotArea().bottom()-m_xText->boundingRect().height()/2);
    m_yText->setPos(m_chart->plotArea().left()-10, position.y() - m_yText->boundingRect().height() / 2.0);


    if (m_chart->plotArea().contains(position) && 
        visibility)
    {  
        m_chart->setCursor(crosshairCursor);
        m_xLine->show();
        m_xText->show();
        m_yLine->show();
        m_yText->show();
        

    }
        
    else
    {
        m_chart->setCursor(Qt::ArrowCursor);
        m_xLine->hide();
        m_xText->hide();
        m_yLine->hide();
        m_yText->hide();      
    }
    m_chart->update();


    
}

void ChartCrosshair::setVisibilty(bool vis)
{
    visibility = vis;
}

bool ChartCrosshair::visible(void)
{
    return visibility;
}
