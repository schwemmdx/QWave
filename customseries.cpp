#include "customseries.h"
#include "mainwindow.h"

#include "theme_colors.h"

#include <QToolTip>
#include <QCursor>
#include <QKeyEvent>
#include <QThread>
#include <QStatusBar>


CustomSeries::CustomSeries(QObject* parent)
{
    connect(this, &QXYSeries::clicked, this, &CustomSeries::select);
    connect(this, &QXYSeries::hovered,this,&CustomSeries::mouseHover);

    selectionState = false;
    setPointsVisible();
}

void CustomSeries::setData(QVector<QPointF> data)
{
    this->clear();
    *this << data;
}
void CustomSeries::unselect()
{
    selectionState = false;
    QPen usedPen = pen();
    usedPen.setWidth(1);
    setPen(usedPen);
    setPointsVisible(false);
    setPointLabelsVisible(false);

}

void CustomSeries::select(void)
{
    selectionState = true;
    QPen usedPen = pen();
    usedPen.setWidth(3);
    setPen(usedPen);
    setPointsVisible(true);
    emit seriesSelected(this);

}

void CustomSeries::mouseHover(const QPointF point,bool state)
{
    if(isSelected()){
        QPalette pal;
        pal.setColor(QPalette::ToolTipBase, Theme::BackGround2);
        pal.setColor(QPalette::ToolTipText, Theme::ForeGround);
        QPoint pos = QCursor::pos();

        QToolTip* info;
        QToolTip::setPalette(pal);
        info->showText(pos,
                       "X: "
                       +QString::number(point.x())
                       +"\nY: "
                       +QString::number(point.y()),nullptr,QRect(),5000);
        emit newStatusMessage("X: " +QString::number(point.x())+" , Y: "+QString::number(point.y()));
    }





}

bool CustomSeries::isSelected(void)
{
    return selectionState;
}

