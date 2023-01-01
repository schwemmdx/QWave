#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "chartcontainer.h"
#include <QtCharts/QtCharts>
#include <QStatusBar>

#include "dataview.h"
#include "theme_colors.h"
#include "optionsdialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //main chart container
    this->chartContainer = new ChartContainer(this);
    connect(chartContainer,&ChartContainer::seriesSelectionChanged,this,&MainWindow::selectedSeriesChanged);
    //connect(chartContainer,&ChartContainer::newStatusMessage,this,&MainWindow::updateStatusBar);
    //connect(this,&MainWindow::rubberBandChangeRequest,chartContainer,&ChartContainer::changeRubberBandBehaviour);


    ui->centralwidget->adjustSize();
    ui->mainLayout->addWidget(chartContainer,1);


    pDataDock = new QDockWidget(this);
    pDataView = new DataView();

    pDataDock->setFeatures(QDockWidget::DockWidgetMovable);
    pDataView->setEditTriggers(QAbstractItemView::EditKeyPressed);
    addDockWidget(Qt::LeftDockWidgetArea,pDataDock);

    pOptionDlg = new OptionsDialog();
    pOptionDlg->hide();

    pDataDock->setWidget(pDataView);

    pDataDock->setMinimumWidth(300);


    connect(this,&MainWindow::loadFromFile,pDataView,&DataView::loadData);
    connect(pDataView,&DataView::appendData,this,&MainWindow::appendDataToChart);

    //connect(this, &MainWindow::chartThmeChangeRequest,chartContainer,&ChartContainer::themeChange);

    chartContainer->setTitle(tr(""));
    //statusBar()->showMessage("Ready");
    setTheme();
    ui->actiontoggleDataView->setChecked(true);



}

MainWindow::~MainWindow()
{
    delete ui;
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
            for(auto &ser: this->chartContainer->tracies)
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


   // chartView->addDataSeries(data[1].getPoints());
   // chartView->setTitle(data[1].getName());


}

void MainWindow::Ondoubleclicktree(int QModelIndex)
{

}


void MainWindow::keyPressEvent(QKeyEvent* event)
{
    
    if( event->key() == Qt::Key_Escape )
    {
        if(!chartContainer->isCrosshairVisible())
        {
            unselectExcept(nullptr);
        }
        else
        {
            chartContainer->setCrosshairVisibility(false);
            this->ui->actionCrosshair_Mode->setChecked(false);
        }
        event->accept();

    }
}


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
    this->chartContainer->addDataSeries(xData,yData,xLabel,yLabel);
}

void MainWindow::on_actioncreateData_triggered()
{

}


void MainWindow::on_actionCrosshair_Mode_triggered()
{
    if(ui->actionCrosshair_Mode->isChecked())
    {
        chartContainer->setCrosshairVisibility(true);
        //emit changeCrosshairVisibility(true);
        ui->actionCrosshair_Mode->setIcon(QIcon(":/icons/icons/icons8-location-off-80.png"));
    }
    else
    {
        chartContainer->setCrosshairVisibility(false);
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

void MainWindow::set_pMain(QApplication* pApp)
{
    pApplication = pApp;
}

void MainWindow::setTheme(void)
{
    QPalette darkpalette;
    darkpalette.setColor(QPalette::Window, Altium::BackGround2);
    darkpalette.setColor(QPalette::Text,Altium::LightText);
    darkpalette.setColor(QPalette::WindowText, Altium::LightText);
    darkpalette.setColor(QPalette::Base,Altium::BackGround2);
    darkpalette.setColor(QPalette::AlternateBase, Altium::BackGround2);
    darkpalette.setColor(QPalette::ToolTipBase, Altium::BackGround.lighter());
    darkpalette.setColor(QPalette::ToolTipText, Altium::LightText),
    darkpalette.setColor(QPalette::Text, Altium::LightText);
    darkpalette.setColor(QPalette::Button, Altium::BackGround.lighter());
    darkpalette.setColor(QPalette::ButtonText, Altium::LightText);
    darkpalette.setColor(QPalette::BrightText, Altium::LightText.lighter());
    darkpalette.setColor(QPalette::Highlight, Altium::Highlight);
    darkpalette.setColor(QPalette::HighlightedText,Altium::LightText);

    pApplication->setPalette(darkpalette);
}

void MainWindow::on_actionOptions_triggered()
{
    pOptionDlg->show();

}




