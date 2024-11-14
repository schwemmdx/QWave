#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include "chartcontainer.h"
#include <QtCharts/QtCharts>

#include <QFileInfo>
#include <QThread>

#include "datawidget.h"
#include "theme_colors.h"
#include "optionsdialog.h"
#include "cursordockwidget.h"
#include "MessageQueue.h"

#include "Message.h"
#include "customchart.h"

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

  


    MessageQueue *msgQ = MessageQueue::instance(this);
    QPoint bottomRight = geometry().bottomRight();
    QPoint globalBottomRight = this->mapToGlobal(bottomRight);
  
    connect(this,&MainWindow::loadFromFile,pDataWidget,&DataWidget::loadData);
    connect(pDataWidget,&DataWidget::appendData,chartContainer,
            [this](QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel,int toAxis){
      chartContainer->addDataSeries(xData,yData,xLabel,yLabel,toAxis);});
    connect(chartContainer,&ChartContainer::updateCursorData,pCursorDock,&CursorDockWidget::updateCursorData);
    //connect(chartContainer,&ChartContainer::updateYCursorInfo,pCursorDock,&CursorDockWidget::updateXCursorData);
    //connect(this, &MainWindow::chartThmeChangeRequest,chartContainer,&ChartContainer::themeChange);

    
    setTheme();

    pDataDock->hide();
    pCursorDock->hide();

    ui->actiontoggleDataDock->setChecked(false);
    ui->actiontoggleCursorDock->setChecked(false);

    connect(chartContainer,&ChartContainer::newTraceSelection,this,&MainWindow::selectedSeriesChanged);
  
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
    QString file =  QFileDialog::getOpenFileName(this,QString("Open Data for inspection"),openDlgStartPath,QString("*.csv"));

    if(file.isEmpty())
    {
        return;
    }
    openDlgStartPath = QFileInfo(file).absolutePath();
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
    switch(event->key())
    {
    case Qt::Key_Escape :
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
        break;

    }

    case Qt::Key_P:
    {
        if (event->modifiers().testFlag(Qt::ControlModifier))
        {
            QString fileName = QFileDialog::getSaveFileName(this,"Save Chart...",QDir::homePath());
            if(!fileName.isEmpty()){
                QPixmap pixmap = chartContainer->grab();
                pixmap.save(fileName, "PNG");
                event->accept();
                MessageQueue* mq = MessageQueue::instance(this);
                mq->addMessage("Export "+fileName+"\nsaved!",
                Altium::LightText,4000,QIcon::fromTheme("document-save"));
            }
        }
        break;


    }
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

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    QPoint bottomRight = geometry().bottomRight();
    MessageQueue* msgQ = MessageQueue::instance(this);
    msgQ->setGeometry(bottomRight.x() - 375, bottomRight.y() - 550, 300, 400);
}

void MainWindow::on_action_ToggleYLeft_triggered(bool checked)
{
    MessageQueue* q = MessageQueue::instance();
    q->addMessage("Left y-axis log scaling: "+QString::number(checked),Altium::LightText);
    chartContainer->chart->setLogYLScale(checked);
}


void MainWindow::on_action_toggleXlog_triggered(bool checked)
{
    MessageQueue* q = MessageQueue::instance();
    q->addMessage("X-axis log scaling: "+QString::number(checked),Altium::LightText);
    chartContainer->chart->setLogXScale(checked);
}


void MainWindow::on_actionToggleYRightLog_triggered(bool checked)
{
    MessageQueue* q = MessageQueue::instance();
    q->addMessage("Right y-axis log scaling: "+QString::number(checked),Altium::LightText);
}

