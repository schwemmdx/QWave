#include "MainWindow.h"
#include "./ui_MainWindow.h"


#include "ChartContainer.h"
#include <QtCharts/QtCharts>

#include <QFileInfo>
#include <QThread>
#include <QSettings>
#include <QShortcut>
#include <QStringList>
#include <QDir>

#include "ThemeColors.h"
#include "OptionsDialog.h"
#include "MessageQueue.h"
#include "Message.h"
#include "CustomChart.h"
#include "CSVData.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //main chart container
    this->chartContainer = new ChartContainer(this);

    dataView = new QTreeView();
    csvData = new CSVData();

    ui->centralwidget->adjustSize();
    ui->mainLayout->addWidget(chartContainer,1);
    ui->stackedWidget->addWidget(dataView);
    ui->stackedWidget->setMinimumWidth(300);
  
    pOptionDlg = new OptionsDialog();
    pOptionDlg->hide();

    loadStyles();
 
    
    ui->actiontoggleCursorDock->setChecked(false);

    connect(chartContainer,&ChartContainer::newTraceSelection,this,&MainWindow::selectedSeriesChanged);
    connect(pOptionDlg,&OptionsDialog::applySettings,this,&MainWindow::applyNewOptions);
    
    connect(csvData,&CSVData::addSeries,chartContainer,&ChartContainer::addSeriesToChart);
    connect(csvData,&CSVData::removeSeries,chartContainer,&ChartContainer::removeSeriesFromChart);


    toggleStackWidgetShort = new QShortcut(QKeySequence("Ctrl+B"), this);
    connect(toggleStackWidgetShort, &QShortcut::activated, this, &MainWindow::toggleStackWidget);


}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::toggleStackWidget() {
   
    // Toggle the visibility flag
    if(isStackWidgetVisible)
    {
        ui->stackedWidget->hide();
    }
    else{
        ui->stackedWidget->show();
    }
    isStackWidgetVisible = !isStackWidgetVisible;
}

void MainWindow::applyNewOptions()
{
    auto mgr = ThemeManager::instance(); 
    if(mgr.setTheme(pOptionDlg->getTheme()))
    {
        pApplication->setPalette(mgr.palette());
    }

    //load the style selected.

    QFile styleFile(qssFiles[pOptionDlg->getSelectedStyle()]);
    if (styleFile.open(QFile::ReadOnly))
    {
        pApplication->setStyleSheet(styleFile.readAll());
    }
    //save the options to the QSettings File
    
}

void MainWindow::loadStyles()
{
    
    QDir styleDir("./themes");
    QStringList filters;
    filters << "*.qss";
    styleDir.setNameFilters(filters);
    QFileInfoList styles = styleDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
    
    for (const QFileInfo &fileInfo : styles) {
        qssFiles[fileInfo.fileName()]= fileInfo.absolutePath();

    }
    
    pOptionDlg->populateStyles(qssFiles.keys());
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
    connect(csvData,&HiracData::dataLoadStarted,this,[file](){
        QFileInfo f(file);
        MessageQueue* q = MessageQueue::instance();
        q->addInfo("Loading "+f.fileName()+ " from "+f.filePath());
    });

    connect(csvData,&HiracData::dataLoadFinished,this,[file](){
        QFileInfo f(file);
        MessageQueue* x = MessageQueue::instance();
        x->addInfo("Load of "+f.fileName()+ " complete.");
    });

    csvData->appendData(dataView,file);
    ui->stackedWidget->setCurrentWidget(dataView);
    



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
                mq->addInfo("Export "+fileName+"\nsaved!");
            }
        }
        break;


    }
    }
}




void MainWindow::appendDataToChart(QVector<double> xData,QVector<double> yData,QString xLabel,QString yLabel,int toAxis)
{
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



void MainWindow::on_actionOptions_triggered()
{
    pOptionDlg->show();
}





void MainWindow::on_actiontoggleCursorDock_toggled(bool arg1)
{
    if(arg1)
    {
        //this->pCursorDock->show();
    }
    else
    {
        //this->pCursorDock->hide();
    }
}


void MainWindow::on_actiontoggleDataDock_toggled(bool arg1)
{
    if(arg1)
    {
       
    }
    else
    {
        
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // Margins for the MessageQueue widget
    const int marginRight = 10;
    const int marginBottom = 20;

    // Get the instance of the MessageQueue
    MessageQueue *msgQ = MessageQueue::instance(this);

    // Calculate the position for the MessageQueue
    QSize msgQSize = msgQ->size(); // Use its current size
    int x = width() - msgQSize.width() - marginRight;
    int y = height() - msgQSize.height() - marginBottom;

    // Set the geometry of the MessageQueue
    msgQ->setGeometry(x, y, msgQSize.width(), msgQSize.height());
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

