
#include <QtCharts/QChart>
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtGui/QTextDocument>
#include <QWidget>

#include "ThemeColors.h"
#include "ChartCrosshair.h"
#include "ChartContainer.h"
#include "ScientificFormatter.h"

ChartCrosshair::ChartCrosshair(CustomChart *chart) : m_xLine(new QGraphicsLineItem(chart)),
                                                     m_yLine(new QGraphicsLineItem(chart)),
                                                     m_xText(new QGraphicsTextItem(chart)),

                                                     m_yLeftText(new QGraphicsTextItem(chart)),
                                                     m_yRightText(new QGraphicsTextItem(chart)),
                                                     m_chart(chart)
{

    // m_chart->setCursor(QCursor(Qt::CursorShape::CrossCursor));
    m_xLine->setPen(QPen(grayCol, 1.5));
    m_yLine->setPen(QPen(grayCol, 1.5));
    m_xText->setZValue(9999);
    m_yLeftText->setZValue(9999);
    m_yRightText->setZValue(9999);

    m_xText->document()->setDocumentMargin(5);
    m_yLeftText->document()->setDocumentMargin(5);
    m_yRightText->document()->setDocumentMargin(5);
    m_xText->setDefaultTextColor(Monokai::PrimaryLabel);
    m_yLeftText->setDefaultTextColor(Monokai::PrimaryLabel);
    m_yRightText->setDefaultTextColor(Monokai::PrimaryLabel);

    xLine = new QLineF();
    yLine = new QLineF();
    m_xLine->setLine(*xLine);
    m_yLine->setLine(*yLine);

    m_xLine->setZValue(9999);
    m_yLine->setZValue(9999);
    visibility = false;

    m_yRightText->hide();
}

void ChartCrosshair::updatePosition(QMouseEvent *event)
{
    auto position(event->pos());

    QLineF xLine(position.x(), m_chart->plotArea().top(),
                 position.x(), m_chart->plotArea().bottom());
    QLineF yLine(m_chart->plotArea().left(), position.y(),
                 m_chart->plotArea().right(), position.y());

    m_xLine->setLine(xLine);
    m_yLine->setLine(yLine);
    QPointF chartPos = m_chart->mapToValue(position);
    QString xText = ScientificFormatter::toScientificSuffix(chartPos.x(), 2);
    QString yLeftText = ScientificFormatter::toScientificSuffix(chartPos.y(), 2);
    QString yRightText = ScientificFormatter::toScientificSuffix(chartPos.y(), 2);

    // hex color for Altium::Highlight = 6482a0
    m_xText->setHtml(QString("<div style='background-color: %1; color: %2;'>%3</div>")
                         .arg(grayCol.name())
                         .arg(textCol.name())
                         .arg("&nbsp;" + xText + "&nbsp;"));

    m_yLeftText->setHtml(QString("<div style='background-color: %1; color: %2;'>%3</div>")
                             .arg(grayCol.name())
                             .arg(textCol.name())
                             .arg("&nbsp;" + yLeftText + "&nbsp;"));

    m_yRightText->setHtml(QString("<div style='background-color: %1; color: %2;'>%3</div>")
                              .arg(grayCol.name())
                              .arg(textCol.name())
                              .arg("&nbsp;" + yRightText + "&nbsp;"));

    m_xText->setPos(position.x() - m_xText->boundingRect().width() /
                                       2.0,
                    m_chart->plotArea().bottom());

    m_yLeftText->setPos(m_chart->plotArea().left() - m_yLeftText->boundingRect().width() + 10, position.y() -
                                                                                                   m_yLeftText->boundingRect().height() / 2.0);

    m_yRightText->setPos(m_chart->plotArea().right() - (yRightText.length() * 10 + 5), position.y() -
                                                                                           m_yRightText->boundingRect().height() / 2.0);

    if (m_chart->plotArea().contains(position) &&
        visibility)
    {

        m_chart->setCursor(crosshairCursor);
        m_xLine->show();
        m_xText->show();
        m_yLine->show();
        m_yLeftText->show();
        if (m_chart->isSecondYaxisVisible())
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
    if (!vis)
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
