#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "chartcontainer.h"
#include <QtCharts/QtCharts>
#include <QStatusBar>

#include "datawidget.h"
#include "theme_colors.h"
#include "optionsdialog.h"
#include "cursordockwidget.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //main chart container
    this->chartContainer = new ChartContainer(this);

    //connect(this,&MainWindow::rubberBandChangeRequest,chartContainer,&ChartContainer::changeRubberBandBehaviour);


    ui->centralwidget->adjustSize();
    ui->mainLayout->addWidget(chartContainer,1);

    setDockOptions(QMainWindow::AllowTabbedDocks);

    pDataDock = new QDockWidget("Data",this);
    
    pDataWidget = new DataWidget(pDataDock);
    pDataDock->setMinimumWidth(300);

    pCursorDock = new CursorDockWidget(this);
    pCursorDock->setMinimumWidth(300);

    addDockWidget(Qt::LeftDockWidgetArea,pDataDock);
    tabifyDockWidget(pDataDock,pCursorDock);

    //addDockWidget(Qt::LeftDockWidgetArea,pCursorDock);


    pDataDock->setWidget(pDataWidget);
    pOptionDlg = new OptionsDialog();
    pOptionDlg->hide();




    connect(this,&MainWindow::loadFromFile,pDataWidget,&DataWidget::loadData);
    connect(pDataWidget,&DataWidget::appendData,chartContainer,
            [this](QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel,int toAxis){
      chartContainer->addDataSeries(xData,yData,xLabel,yLabel,toAxis);});
    connect(chartContainer,&ChartContainer::updateCursorData,pCursorDock,&CursorDockWidget::updateCursorData);
    //connect(chartContainer,&ChartContainer::updateYCursorInfo,pCursorDock,&CursorDockWidget::updateXCursorData);
    //connect(this, &MainWindow::chartThmeChangeRequest,chartContainer,&ChartContainer::themeChange);

    statusBar()->show();
    setTheme();

    pDataDock->hide();
    pCursorDock->hide();

    ui->actiontoggleDataDock->setChecked(false);
    ui->actiontoggleCursorDock->setChecked(false);

    connect(chartContainer,&ChartContainer::newTraceSelection,this,&MainWindow::selectedSeriesChanged);
    //connect(chartContainer,&ChartContainer::newStatusMessage,this,[this](QString msg){statusBar()->showMessage(msg);});

}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::selectedSeriesChanged(CustomSeries* traceClicked)
{

    unselectExcept(traceClicked);
    focusTrace = traceClicked;

}

void MainWindow::unselectExcept(CustomSeries* traceClicked)
{

    if(focusTrace != nullptr)
    {

        for(auto &ser: this->chartContainer->getSeriesInChart())
        {

            if(ser != traceClicked)
            {
                  ser->unselect();

            }

        }
     }
}



void MainWindow::on_actionImportData_triggered()
{
    QString file =  QFileDialog::getOpenFileName();
    if(file.isEmpty())
    {
        return;
    }
    this->pDataDock->show();
    ui->actiontoggleDataDock->setChecked(true);
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




void MainWindow::appendDataToChart(QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel,int toAxis)
{

    this->chartContainer->addDataSeries(xData,yData,xLabel,yLabel,toAxis);
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





void MainWindow::on_actiontoggleCursorDock_toggled(bool arg1)
{
    if(arg1)
    {
        this->pCursorDock->show();
    }
    else
    {
        this->pCursorDock->hide();
    }
}


void MainWindow::on_actiontoggleDataDock_toggled(bool arg1)
{
    if(arg1)
    {
        this->pDataDock->show();
    }
    else
    {
        this->pDataDock->hide();
    }
}

