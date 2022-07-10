#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "dockchart.h"
#include <QtCharts/QtCharts>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    for(int i=0;i<4;i++){
        QWidget*  chartContainer = new DockChart(this);
        chartContainer->setWindowTitle(tr("Datenreihe %1").arg(i+1));
        ui->mainLayout->addWidget(chartContainer);
    }
*/



    //QDockWidget *chartContainer3 = new ChartContainer(this->ui->centralwidget);
    //QDockWidget *chartContainer4 = new ChartContainer(this->ui->centralwidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionTest_triggered()
{
    int i = ui->mainLayout->count();

        QWidget*  chartContainer = new DockChart(this);
        //DockChart test(this);
        pDockedCharts.append(chartContainer);
        chartContainer->setWindowTitle(tr("Datenreihe %1").arg(i+1));
        ui->centralwidget->adjustSize();
        ui->mainLayout->addWidget(chartContainer,1);
        chartContainer->setFocus();
        DockChart doc = static_cast<DockChart>(chartContainer);
        if(chartContainer->hasFocus())
        {

            doc.highlight(true);
        }


}

