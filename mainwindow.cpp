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

        QWidget*  chartContainer = new ChartContainer(this);
        //DockChart test(this);

        pDockedCharts.append(chartContainer);


        ui->centralwidget->adjustSize();
        ui->mainLayout->addWidget(chartContainer,1);
        chartContainer->setFocus();
        ChartContainer* chart = static_cast<ChartContainer*>(chartContainer);
        chart->setTitle(tr("Datenreihe %1").arg(i+1));
}

void MainWindow::switchSelectedChart(QVector<ChartContainer*>)
{
    //Do shit
}

