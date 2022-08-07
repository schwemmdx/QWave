#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "chartcontainer.h"
#include <QtCharts/QtCharts>
#include <QStatusBar>


#include "dataimporter.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->showMessage("Ready");

}

MainWindow::~MainWindow()
{
    delete ui;
}

//! this function is going to be removed later on
void MainWindow::on_actionTest_triggered()
{
    int i = ui->mainLayout->count();

    ChartContainer*  chartContainer = new ChartContainer(this);
    connect(chartContainer,&ChartContainer::seriesSelectionChanged,this,&MainWindow::selectedSeriesChanged);
    connect(chartContainer,&ChartContainer::newStatusMessage,this,&MainWindow::updateStatusBar);
    pDockedCharts.append(chartContainer);


    ui->centralwidget->adjustSize();
    ui->mainLayout->addWidget(chartContainer,1);

    ChartContainer* chart = static_cast<ChartContainer*>(chartContainer);
    chart->setTitle(tr("Datenreihe %1").arg(i+1));
}


void MainWindow::selectedSeriesChanged(CustomSeries* traceClicked)
{

    unselectExcept(traceClicked);

    focusTrace = traceClicked;
    updateFocusTraceDetails(traceClicked);
}

void MainWindow::unselectExcept(CustomSeries* traceClicked)
{
    if(focusTrace != nullptr)
    {
        for(auto &container: pDockedCharts)
        {
            for(auto &ser: container->tracies)
            {
                if(ser != traceClicked)
                {
                      ser->unselect();
                }

            }
        }
    }
}



void MainWindow::on_actionImportData_triggered()
{
    QString file =  QFileDialog::getOpenFileName();
    //qDebug() << files;
    if(file.isEmpty())
    {
        return;
    }
    DataImporter::fromCSV(file);


}


void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if( event->key() == Qt::Key_Escape )
    {
        unselectExcept(nullptr);
    }
}


void MainWindow::updateFocusTraceDetails(CustomSeries* trace)
{
    ui->numPts->setText(QString::number(trace->points().length()));
}

void MainWindow::updateStatusBar(QString msg)
{
    statusBar()->showMessage(msg);
}

