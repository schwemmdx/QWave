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
    tools = new ActionListWidget();

    ui->centralwidget->adjustSize();
    ui->mainLayout->addWidget(chartContainer, 1);
    ui->stackedWidget->addWidget(dataView);
    ui->stackedWidget->addWidget(tools);
    ui->stackedWidget->setMinimumWidth(300);
    isStackWidgetVisible = false;
    ui->stackedWidget->hide();
    pOptionDlg = new OptionsDialog(this);
    pOptionDlg->hide();

    loadStyles();
    ui->actiontoggleCursorDock->setChecked(false);

    connect(chartContainer, &ChartContainer::newTraceSelection, this, &MainWindow::selectedSeriesChanged);
    connect(pOptionDlg, &OptionsDialog::applySettings, this, &MainWindow::applyNewOptions);

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
    auto mgr = ThemeManager::instance();
    // if (mgr.setTheme(pOptionDlg->getTheme()))
    //{
    //     pApplication->setPalette(mgr.palette());
    // }

    // load the style selected.

    QFile styleFile(qssFiles[pOptionDlg->getSelectedStyle()]);
    if (styleFile.open(QFile::ReadOnly))
    {
        pApplication->setStyleSheet(styleFile.readAll());
    }
    // save the options to the QSettings File
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

    pOptionDlg->populateStyles(qssFiles.keys());
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

void MainWindow::processNextFile(QStringList files)
{
    if (files.isEmpty())
        return; // All files processed

    QString currentFile = files.takeFirst(); // Get the first file and remove it from the list

    openDlgStartPath = QFileInfo(currentFile).absolutePath();
    csvData->disconnect(this); // Disconnect previous signals

    connect(csvData, &HiracData::dataLoadStarted, this, [currentFile]() {
        QFileInfo f(currentFile);
        MessageQueue *q = MessageQueue::instance();
        q->addInfo("Loading " + f.fileName() + " from " + f.filePath());
    });

    connect(csvData, &HiracData::dataLoadFinished, this, [this, files, currentFile]() {
        QFileInfo f(currentFile);
        MessageQueue *x = MessageQueue::instance();
        x->addInfo("Load of " + f.fileName() + " complete.");

        // Process the next file after the current one is done
        processNextFile(files);
    });

    csvData->appendData(dataView, currentFile); // Start loading the current file
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
    pOptionDlg->show();
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
    const int marginRight = 0;
    const int marginBottom = 20;

    // Get the instance of the MessageQueue
    MessageQueue *msgQ = MessageQueue::instance(this);

    // Calculate the position for the MessageQueue
    QSize msgQSize = msgQ->size(); // Use its current size
    int x = width() - msgQSize.width() - marginRight;
    int y = height() - msgQSize.height() - marginBottom;

    // Set the geometry of the MessageQueue
    msgQ->setGeometry(x, y, msgQSize.width(), msgQSize.height());

    QWidget::resizeEvent(event);
}

void MainWindow::on_action_ToggleYLeft_triggered(bool checked)
{
    MessageQueue *q = MessageQueue::instance();
    q->addMessage("Left y-axis log scaling: " + QString::number(checked), Monokai::PrimaryLabel);
    chartContainer->chart->setLogYLScale(checked);
}

void MainWindow::on_action_toggleXlog_triggered(bool checked)
{
    MessageQueue *q = MessageQueue::instance();
    q->addMessage("X-axis log scaling: " + QString::number(checked), Monokai::PrimaryLabel);
    chartContainer->chart->setLogXScale(checked);
}

void MainWindow::on_actionToggleYRightLog_triggered(bool checked)
{
    MessageQueue *q = MessageQueue::instance();
    q->addMessage("Right y-axis log scaling: " + QString::number(checked), Monokai::PrimaryLabel);
}
