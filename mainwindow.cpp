#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "chartcontainer.h"
#include <QtCharts/QtCharts>
#include <QStatusBar>
#include "dataview.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //main chart container
    ChartContainer*  chartContainer = new ChartContainer(this);
    ChartContainer* chart = static_cast<ChartContainer*>(chartContainer);
    connect(chartContainer,&ChartContainer::seriesSelectionChanged,this,&MainWindow::selectedSeriesChanged);
    //connect(chartContainer,&ChartContainer::newStatusMessage,this,&MainWindow::updateStatusBar);
    connect(this,&MainWindow::rubberBandChangeRequest,chartContainer,&ChartContainer::changeRubberBandBehaviour);
    pDockedCharts.append(chartContainer);
    ui->centralwidget->adjustSize();
    ui->mainLayout->addWidget(chartContainer,1);

    pDataDock = new QDockWidget(this);

    pDataView = new DataView();
    pDataDock->setFeatures(QDockWidget::DockWidgetMovable);
    pDataView->setEditTriggers(QAbstractItemView::EditKeyPressed);
    addDockWidget(Qt::LeftDockWidgetArea,pDataDock);

    pDataDock->setWidget(pDataView);

    pDataDock->setMinimumWidth(300);


    connect(this,&MainWindow::loadFromFile,pDataView,&DataView::loadData);
    connect(pDataView,&DataView::appendData,this,&MainWindow::appendDataToChart);
    chart->setTitle(tr("No Data"));
    //statusBar()->showMessage("Ready");

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
    //connect(this,&MainWindow::changeCrosshairVisibility,chartContainer,&ChartContainer::setCrosshairVisibility);
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
        //for(auto &container: pDockedCharts)
        //{
            for(auto &ser: pDockedCharts[0]->tracies)
            {
                if(ser != traceClicked)
                {
                      ser->unselect();
                }

            }
        //}
    }
}



void MainWindow::on_actionImportData_triggered()
{
    QString file =  QFileDialog::getOpenFileName();
    if(file.isEmpty())
    {
        return;
    }
    emit loadFromFile(file);


   // pDockedCharts[0]->addDataSeries(data[1].getPoints());
   // pDockedCharts[0]->setTitle(data[1].getName());


}

void MainWindow::Ondoubleclicktree(int QModelIndex)
{
    qDebug() << QModelIndex;
}
/*
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    
    if( event->key() == Qt::Key_Escape )
    {
        unselectExcept(nullptr);
    }
}
*/

void MainWindow::updateFocusTraceDetails(CustomSeries* trace)
{
   // ui->numPts->setText(QString::number(trace->points().length()));
}

void MainWindow::updateStatusBar(QString msg)
{
    statusBar()->showMessage(msg);
}

void MainWindow::appendDataToChart(QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel)
{
    pDockedCharts[0]->addDataSeries(xData,yData,xLabel,yLabel);
}

void MainWindow::on_actioncreateData_triggered()
{

}


void MainWindow::on_actionzoomVertically_triggered()
{
    auto rb = QChartView::NoRubberBand;
    if(ui->actionzoomVertically->isChecked())
    {
        ui->actionzoomROI->setChecked(false);
        ui->actionZoomHorizontally->setChecked(false);
        rb = QChartView::VerticalRubberBand;
    }
    emit rubberBandChangeRequest(rb);
}


void MainWindow::on_actionzoomROI_triggered()
{
    auto rb = QChartView::NoRubberBand;
    if(ui->actionzoomROI->isChecked())
    {
        rb = QChartView::RectangleRubberBand;
        ui->actionZoomHorizontally->setChecked(false);
        ui->actionzoomVertically->setChecked(false);
    }
    emit rubberBandChangeRequest(rb);
}


void MainWindow::on_actionZoomHorizontally_triggered()
{
    auto rb = QChartView::NoRubberBand;
    if(ui->actionzoomVertically->isChecked())
    {
        ui->actionzoomROI->setChecked(false);
        ui->actionzoomVertically->setChecked(false);
        rb = QChartView::VerticalRubberBand;
    }
    emit rubberBandChangeRequest(rb);
}


void MainWindow::on_actionCrosshair_Mode_triggered()
{
    if(ui->actionCrosshair_Mode->isChecked())
    {
        pDockedCharts[0]->setCrosshairVisibility(true);
        //emit changeCrosshairVisibility(true);
        ui->actionCrosshair_Mode->setIcon(QIcon(":/icons/icons/icons8-location-off-80.png"));
    }
    else
    {
        pDockedCharts[0]->setCrosshairVisibility(false);
        ui->actionCrosshair_Mode->setIcon(QIcon(":/icons/icons/icons8-target-80.png"));
    }
}


void MainWindow::on_actionMeasure_triggered()
{
    if(ui->actionMeasure->isChecked())
    {

    }
    else
    {

    }
}




void MainWindow::on_actiontoggleDataView_triggered()
{
    if(ui->actiontoggleDataView->isChecked())
    {
        this->pDataDock->show();
    }
    else
    {
        this->pDataDock->hide();
    }

}

