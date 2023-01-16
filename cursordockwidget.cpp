#include "cursordockwidget.h"
#include "ui_cursordockwidget.h"

CursorDockWidget::CursorDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CursorDockWidget)
{
    ui->setupUi(this);
/*
    connect(ui->placeXBtn,&QCommandLinkButton::clicked,this,[this](){emit placeXCursors();});
    connect(ui->placeYBtn,&QCommandLinkButton::clicked,this,[this](){emit placeYCursors();});
    connect(ui->clearCursorsBtn,&QCommandLinkButton::clicked,this,[this](){emit clearCursors();});
*/
}

CursorDockWidget::~CursorDockWidget()
{
    delete ui;
}


void CursorDockWidget::updateCursorData(QList<void*> dummy)
{
    /*
    ui->x1->setText(QString::number(p1,'g',4));
    ui->x1->setText(QString::number(p2,'g',4));
    ui->deltaX->setText(QString::number(p2-p1,'g',4));
    ui->freqX->setText(QString::number(abs(1/p2-p1),'g',4));
    */
}



void CursorDockWidget::cursorsCleared(void)
{
    ui->deltaX->setText("-");
    ui->deltaY->setText("-");
    ui->x1->setText("-");
    ui->x2->setText("-");
    ui->y1->setText("-");
    ui->y2->setText("-");
    ui->freqX->setText("-");

}
