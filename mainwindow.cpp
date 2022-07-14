#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "chartcontainer.h"
#include <QtCharts/QtCharts>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionTest_triggered()
{
    int i = ui->mainLayout->count();

        ChartContainer*  chartContainer = new ChartContainer(this);
        connect(chartContainer,&ChartContainer::seriesSelectionChanged,this,&MainWindow::selectedSeriesChanged);

        pDockedCharts.append(chartContainer);


        ui->centralwidget->adjustSize();
        ui->mainLayout->addWidget(chartContainer,1);
        chartContainer->setFocus();
        ChartContainer* chart = static_cast<ChartContainer*>(chartContainer);
        chart->setTitle(tr("Datenreihe %1").arg(i+1));
}

//void MainWindow::switchSelectedChart(QVector<ChartContainer*>)

    //Do shit

void MainWindow::selectedSeriesChanged(CustomSeries* traceClicked)
{
    QPen defaultPen;
    for(auto &container: pDockedCharts)
    {
        for(auto &ser: container->tracies)
        {
            if(ser != traceClicked)
            {
                defaultPen = ser->pen();
                defaultPen.setWidth(1);
                ser->setPen(defaultPen);
            }
        }
    }
    defaultPen = traceClicked->pen();
    defaultPen.setWidth(defaultPen.width()+1);
    traceClicked->setPen(defaultPen);


    updateFocusTraceDetails(traceClicked);


}

void MainWindow::on_actionImportData_triggered()
{
    QStringList files =  QFileDialog::getOpenFileNames();
    //qDebug() << files;
}


void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if( event->key() == Qt::Key_Escape )
    {
        qDebug() << "ESC Pressed!";

    }
}


void MainWindow::updateFocusTraceDetails(CustomSeries* trace)
{
    ui->numPts->setText(QString::number(trace->points().length()));
}

