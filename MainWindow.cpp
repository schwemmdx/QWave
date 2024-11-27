#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ChartContainer.h"
#include <QtCharts/QtCharts>

#include <QFileInfo>
#include <QThread>
#include <QSettings>
#include <QShortcut>
#include <QStringList>
#include <QDir>

#include <QDebug>

#include "ThemeColors.h"
#include "SettingsTree.h"
#include "MessageQueue.h"

#include "CustomChart.h"
#include "CSVData.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // main chart container
    this->chartContainer = new ChartContainer(this);

    dataView = new QTreeView();
    csvData = new CSVData();
    settings = new SettingsTree(this);
    settings->addSetting("Graph","X Major Grid",false,SettingsTree::Bool,[](const QVariant &newValue) {
        qDebug() << "X Major Grid changed to:" << newValue.toBool();
    });
    
    settings->addSetting("Graph","Y Left Major Grid",false,SettingsTree::Bool,nullptr);
    settings->addSetting("Graph","Y Right Major Grid",false,SettingsTree::Bool,nullptr);
    settings->addSetting("Graph","X Minor Grid",false,SettingsTree::Bool,nullptr);
    settings->addSetting("Graph","Y Left Minor Grid",false,SettingsTree::Bool,nullptr);
    settings->addSetting("Graph","Y Right Minor Grid",false,SettingsTree::Bool,nullptr);
    settings->addSetting("Graph Elements","Trace Width",1.0,SettingsTree::Float
    ,nullptr);
    settings->addSetting("Graph Elements","Font","San Francisco",SettingsTree::Font,nullptr);
    settings->addSetting("Export Parameters","BG Color","white",SettingsTree::Color,nullptr);
 
    
    
    ui->centralwidget->adjustSize();
    ui->mainLayout->addWidget(chartContainer, 1);
    ui->stackedWidget->addWidget(dataView);
    ui->stackedWidget->addWidget(settings);
    ui->stackedWidget->setMinimumWidth(250);
    isStackWidgetVisible = false;
    ui->stackedWidget->hide();
    

    loadStyles();
    ui->actiontoggleCursorDock->setChecked(false);

    connect(chartContainer, &ChartContainer::newTraceSelection, this, &MainWindow::selectedSeriesChanged);
   

    connect(csvData, &CSVData::addSeries, chartContainer, &ChartContainer::addSeriesToChart);
    connect(csvData, &CSVData::removeSeries, chartContainer, &ChartContainer::removeSeriesFromChart);

    toggleStackWidgetShort = new QShortcut(QKeySequence("Ctrl+B"), this);
    connect(toggleStackWidgetShort, &QShortcut::activated, this, &MainWindow::toggleStackWidget);

    QPalette palette;

    // Base Colors
    palette.setColor(QPalette::Window, Monokai::SystemBackground);        // Window background
    palette.setColor(QPalette::WindowText, Monokai::PrimaryLabel);          // Text in windows
    palette.setColor(QPalette::Base, Monokai::SystemBackgroundDark);         // Background for text editing widgets
    palette.setColor(QPalette::AlternateBase, Monokai::SystemBackgroundDark); // Alternate row background
    palette.setColor(QPalette::ToolTipBase, Monokai::SecondaryBackground);  // ToolTip background
    palette.setColor(QPalette::ToolTipText, Monokai::PrimaryLabel);         // ToolTip text
    // Button Colors
    palette.setColor(QPalette::Button, Monokai::SecondaryBackground); // Button background
    palette.setColor(QPalette::ButtonText, Monokai::PrimaryLabel);    // Button text
    palette.setColor(QPalette::BrightText, Monokai::PrimaryLabel);    // Highlighted text (red for emphasis)

    // Highlight and Disabled
    palette.setColor(QPalette::Highlight, Monokai::TertiaryBackground);            // Selected items highlight
    palette.setColor(QPalette::HighlightedText, Monokai::PrimaryLabel);            // Selected items text
    palette.setColor(QPalette::Disabled, QPalette::Text, Monokai::SecondaryLabel); // Disabled text

    // Text and Labels
    palette.setColor(QPalette::Text, Monokai::PrimaryColor);               // Normal text
    palette.setColor(QPalette::PlaceholderText, Monokai::QuaternaryLabel); // Placeholder text

    // Shadows and Fills
    palette.setColor(QPalette::Shadow, QColor(0, 0, 0, 128));     // Shadow
    palette.setColor(QPalette::Light, Monokai::SecondaryFill);    // Light color for gradients
    palette.setColor(QPalette::Midlight, Monokai::PrimaryFill);   // Mid-light for subtle fills
    palette.setColor(QPalette::Dark, Monokai::SystemBackgroundDark);  // Darker background areas
    palette.setColor(QPalette::Mid, Monokai::TertiaryBackground); // Mid-point in gradient backgrounds
    pApplication->setPalette(palette);

    QString tooltipStyle = R"(
    QToolTip {
        background-color: #272822; /* Monokai background */
        color: #F8F8F2; /* Monokai text color */
        border: 0px solid #66D9EF; /* Monokai blue for the border */
        border-radius: 4px; /* Optional rounded corners */
    }
)";
    pApplication->setStyleSheet(tooltipStyle);
    MessageQueue::instance(this)->addWarning("You are running a debug build");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleStackWidget()
{

    // Toggle the visibility flag
    if (isStackWidgetVisible)
    {
        ui->stackedWidget->hide();
    }
    else
    {
        ui->stackedWidget->show();
    }
    isStackWidgetVisible = !isStackWidgetVisible;
}

void MainWindow::applyNewOptions()
{
   
}

void MainWindow::loadStyles()
{

    QDir styleDir("./themes");
    QStringList filters;
    filters << "*.qss";
    styleDir.setNameFilters(filters);
    QFileInfoList styles = styleDir.entryInfoList(QDir::Files | QDir::NoSymLinks);

    for (const QFileInfo &fileInfo : styles)
    {
        qssFiles[fileInfo.fileName()] = fileInfo.absolutePath();
    }

   
}

void MainWindow::selectedSeriesChanged(CustomSeries *traceClicked)
{

    unselectExcept(traceClicked);
    focusTrace = traceClicked;
}

void MainWindow::unselectExcept(CustomSeries *traceClicked)
{

    if (focusTrace != nullptr)
    {

        for (auto &ser : this->chartContainer->getSeriesInChart())
        {

            if (ser != traceClicked)
            {
                ser->unselect();
            }
        }
    }
}


void MainWindow::on_actionImportData_triggered()
{
    MessageQueue::instance(this)->addInfo("Start Processing...");
    
    QStringList files = QFileDialog::getOpenFileNames(
        this, QString("Open Data for inspection"), openDlgStartPath, QString("*.csv"));

    if (files.isEmpty())
    {
        return;
    }

    ui->stackedWidget->show();
    ui->stackedWidget->setCurrentWidget(dataView);

    // Process files one by one
    
    processNextFile(files);
}

void MainWindow::processNextFile(QStringList files) {
    if (files.isEmpty()) {
        return; // All files processed
    }

    QString currentFile = files.takeFirst(); // Get the first file and remove 
    if (currentFile.isEmpty() || !QFileInfo::exists(currentFile)) {
        MessageQueue::instance(this)->addError("Invalid file: " + currentFile);
        processNextFile(files);
        return;
    }
    //it from the list
    openDlgStartPath = QFileInfo(currentFile).absolutePath();

    // Create a unique progress message for this file
    QFileInfo fileInfo(currentFile);


    Message *fileMessage = MessageQueue::instance(this)->addProgressMessage("Starting to load " + fileInfo.fileName() + "...");
    // Notify the start of file loading
    connect(csvData, &HiracData::dataLoadStarted, this, [fileMessage, currentFile]() {
        fileMessage->updateText("Loading started: " + QFileInfo(currentFile).fileName());
    });

    // Update progress dynamically
    connect(csvData, &HiracData::dataLoadProgress, this, [fileMessage](int progress) {
        if (fileMessage) {
            progress = std::clamp(progress, 0, 100); // Ensure progress is within valid range
            fileMessage->updateProgress(progress);
            fileMessage->updateText("Loading progress: " + QString::number(progress) + "%");
        }
    });

    // Notify when the file load is complete
    connect(csvData, &HiracData::dataLoadFinished, this, [this, files, currentFile, fileMessage]() {
        disconnect(csvData, nullptr, this, nullptr);
        if (fileMessage) {
            fileMessage->updateText("Finished loading: " + QFileInfo(currentFile).fileName());
            fileMessage->updateProgress(100); // Ensure progress bar is full
            processNextFile(files);
        }

        // Process the next file in the list

    });

    // Handle errors during file loading
    qDebug() << "Loading " << currentFile ;

    csvData->appendData(dataView,currentFile);
    //MessageQueue::instance(this)->addInfo("Loaded "+currentFile);

}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
    {
        if (!chartContainer->isCrosshairVisible())
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
            QString fileName = QFileDialog::getSaveFileName(this, "Save Chart...", QDir::homePath());
            if (!fileName.isEmpty())
            {
                QPixmap pixmap = chartContainer->grab();
                pixmap.save(fileName, "PNG");
                event->accept();
                MessageQueue *mq = MessageQueue::instance(this);
                mq->addInfo("Export " + fileName + "\nsaved!");
            }
        }
        break;
    }
    }
}


void MainWindow::on_actionCrosshair_Mode_triggered()
{
    if (ui->actionCrosshair_Mode->isChecked())
    {

        chartContainer->setCrosshairVisibility(true);
        // emit changeCrosshairVisibility(true);
        // ui->actionCrosshair_Mode->setIcon(QIcon(":/icons/icons/icons8-location-off-80.png"));
    }
    else
    {

        chartContainer->setCrosshairVisibility(false);
        // ui->actionCrosshair_Mode->setIcon(QIcon(":/icons/icons/icons8-target-80.png"));
    }
}

void MainWindow::set_pMain(QApplication *pApp)
{
    pApplication = pApp;
}

void MainWindow::on_actionOptions_triggered()
{
    ui->stackedWidget->setCurrentWidget(settings);
    ui->stackedWidget->show();
}

void MainWindow::on_actiontoggleCursorDock_toggled(bool arg1)
{
    if (arg1)
    {
        // this->pCursorDock->show();
    }
    else
    {
        // this->pCursorDock->hide();
    }
}

void MainWindow::on_actiontoggleDataDock_toggled(bool arg1)
{
    if (arg1)
    {
    }
    else
    {
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // Margins for the MessageQueue widget
    const int marginRight = 5;  // Add a small margin from the right edge
    const int marginBottom = 25; // Distance from the bottom toolbar

    // Get the instance of the MessageQueue
    MessageQueue *msgQ = MessageQueue::instance(this);

    // Calculate the height dynamically if the MessageQueue adjusts its size based on messages
    QSize msgQSize = msgQ->size(); // Use sizeHint for more accurate size
    int width = msgQSize.width();      // Use the MessageQueue's preferred width
    int height = msgQSize.height();    // Use the MessageQueue's preferred height

    // Calculate the position for the MessageQueue
    int x = this->width() - width - marginRight;
    int y = this->height() - height - marginBottom;

    // Set the geometry of the MessageQueue
    msgQ->setGeometry(x, y, width, height);

    // Call the base class implementation
    QMainWindow::resizeEvent(event);
}


void MainWindow::on_action_ToggleYLeft_triggered(bool checked)
{
    MessageQueue *q = MessageQueue::instance(this);
    q->addInfo("Left y-axis log scaling: " + QString::number(checked));
    chartContainer->chart->setLogYLScale(checked);
}

void MainWindow::on_action_toggleXlog_triggered(bool checked)
{
    MessageQueue *q = MessageQueue::instance(this);
    q->addInfo("X-axis log scaling: " + QString::number(checked));
    chartContainer->chart->setLogXScale(checked);
}

void MainWindow::on_actionToggleYRightLog_triggered(bool checked)
{
    MessageQueue *q = MessageQueue::instance(this);
    q->addInfo("Right y-axis log scaling: " + QString::number(checked));
}
