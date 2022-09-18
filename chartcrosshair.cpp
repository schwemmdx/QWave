
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

    m_chart->setCursor(QCursor(Qt::CursorShape::CrossCursor));
    m_xLine->setPen(QPen(Theme::Orange, 1));
    m_yLine->setPen(QPen(Theme::Orange, 1));
    m_xText->setZValue(11);
    m_yText->setZValue(11);
    m_xText->document()->setDocumentMargin(5);
    m_yText->document()->setDocumentMargin(5);
    m_xText->setDefaultTextColor(Theme::Orange);
    m_yText->setDefaultTextColor(Theme::Orange);

    xLine = new QLineF();
    yLine = new QLineF();
    m_xLine->setLine(*xLine);
    m_yLine->setLine(*yLine);

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

    QString xText = QString("%1").arg(m_chart->mapToValue(position).x()),
            yText = QString("%1").arg(m_chart->mapToValue(position).y());
    m_xText->setHtml(QString("<div style='background-color: #ff0000;'>") + xText + "</div>");
    m_yText->setHtml(QString("<div style='background-color: #ff0000;'>") + yText + "</div>");
    m_xText->setPos(position.x() - m_xText->boundingRect().width() / 2.0, m_chart->plotArea().bottom());
    m_yText->setPos(m_chart->plotArea().right(), position.y() - m_yText->boundingRect().height() / 2.0);


    if (m_chart->plotArea().contains(position) && 
        event->modifiers().testFlag(Qt::ControlModifier))
    {  

        m_xLine->show();
        m_xText->show();
        m_yLine->show();
        m_yText->show();
        

    }
        
    else
    {
        m_xLine->hide();
        m_xText->hide();
        m_yLine->hide();
        m_yText->hide();      
    }
    m_chart->update();
    event->accept();
    
}
