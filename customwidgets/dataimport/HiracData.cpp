#include "HiracData.h"
#include <QDebug>
#include <QApplication>
#include <QMenu>
#include <QAction>

#include "MessageQueue.h"

HiracData::HiracData(QObject *parent) : QObject(parent) {}

void HiracData::appendData(QTreeView *treeView, const QString &dataPath)
{
    // Ensure the model exists
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(treeView->model());
    if (!model)
    {
        model = new QStandardItemModel(treeView);
        treeView->setModel(model);
    }

    // Emit a signal to indicate data loading has started
    emit dataLoadStarted();

    // Use QtConcurrent to run the loading task in a separate thread
    QFuture<void> future = QtConcurrent::run([this, dataPath, model]()
                                             {
                                                 qDebug() << "Starting to load data asynchronously...";
                                                 this->loadData(dataPath, model); // Call the pure virtual loadData function
                                             });

    // Monitor the task using QFutureWatcher
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    watcher->setFuture(future);

    // Connect the watcher to handle task completion
    connect(watcher, &QFutureWatcher<void>::finished, this, [this, treeView, watcher]()
            {
                qDebug() << "Watcher triggered Data loading finished.";
                // Emit the finished signal
                emit dataLoadFinished();

                watcher->deleteLater(); // Clean up the watcher
            });

    setupContextMenu(treeView);
    treeView->setHeaderHidden(true);
    treeView->setAnimated(true);
}

void HiracData::setupContextMenu(QTreeView *treeView)
{
    // Enable the custom context menu
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    treeView->disconnect(this);
    // Connect the custom context menu request signal
    connect(treeView, &QTreeView::customContextMenuRequested, this, [this, treeView](const QPoint &pos)
            {
        QModelIndex index = treeView->indexAt(pos);
        if (!index.isValid()) return;

        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(treeView->model());
        QStandardItem *item = model ? model->itemFromIndex(index) : nullptr;
        if (!item) return;

        QMenu menu;
        QMenu subMenu;

        // Add context menu actions for items with vector data
        if (item->data(Qt::UserRole).canConvert<QVector<double>>()) {
            menu.addAction("Select as X Vector", [this, treeView, index]() {
                onSetAsXVector(treeView, index);
            });

            menu.addAction("Add to Left Y Axis", [this, treeView, index]() {
                onAddtoLeftYAxis(treeView, index);
            });

            menu.addAction("Add to Right Y Axis", [this, treeView, index]() {
                onAddtoRightAxis(treeView, index);
            });
        } else if (!item->parent()) {
            // Context menu for root nodes
            menu.addAction("Remove Dataset", [this, treeView, index]() {
                removeFromTree(treeView, index);
            });

            // Add export submenu
            subMenu.setTitle("Export to...");
            subMenu.addAction("Comma Separated Value (*.csv)");
            subMenu.addAction("Hierarchical Data Format (*.hdf5)");
            subMenu.addAction("NumPy Byte Array (*.npy)");
            subMenu.addAction("JavaScript Object Notation (*.json)");
            menu.addMenu(&subMenu);
            menu.addSeparator();
            menu.addAction("Collapse all",
            [treeView](){treeView->collapseAll();});
            menu.addAction("Expand all",[treeView](){treeView->expandToDepth(0);});
        }

        // Execute the menu at the global position
        menu.exec(treeView->viewport()->mapToGlobal(pos)); }, Qt::UniqueConnection);

    // Disconnect previous connections to prevent duplicates
    // treeView->disconnect(this);

    // Connect the signal for item click
    connect(treeView, &QTreeView::clicked, this, &HiracData::handleItemClick, Qt::UniqueConnection);
}

void HiracData::onSetAsXVector(QTreeView *treeView, const QModelIndex &index)
{
    if (!treeView || !index.isValid())
    {
        qDebug() << "Error: Invalid treeView or index.";
        return;
    }

    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(treeView->model());
    QStandardItem *item = model ? model->itemFromIndex(index) : nullptr;
    if (!item)
    {
        qDebug() << "No item found for index.";
        return;
    }

    // Find the root node
    QStandardItem *rootNode = item;
    while (rootNode->parent())
    {
        rootNode = rootNode->parent();
    }

    // Reset the previous X vector's visual state
    if (xVectors.contains(rootNode))
    {
        QStandardItem *previousXItem = xVectors[rootNode].item;
        if (previousXItem)
        {
            // reset formatting of item
            previousXItem->setForeground(defaultForegroundColor);
            QFont fnt = previousXItem->font();
            fnt.setItalic(false);
            previousXItem->setFont(fnt);
            previousXItem->setText(previousXItem->text().remove(" (x-vector)"));
        }
    }

    // Set the new X vector
    XVectorInfo info;
    info.data = qvariant_cast<QVector<double>>(index.data(Qt::UserRole));
    info.item = item; // Store the item directly
    xVectors[rootNode] = info;

    // Update UI
    item->setForeground(xVectorColor);
    QFont nFnt = item->font();
    nFnt.setItalic(true);
    item->setFont(nFnt);
    // item->setBackground(QBrush(QColor(173, 216, 230)));
    item->setText(item->text() + " (x-vector)");

    MessageQueue *q = MessageQueue::instance();
    q->addInfo("X Vector for " + rootNode->text() + " set to: " + index.data().toString());

    qDebug() << "X Vector set for root node:" << rootNode->text();
}

void HiracData::handleItemClick(const QModelIndex &index)
{
    if (!index.isValid() || !index.data(Qt::UserRole).canConvert<QVector<double>>())
    {
        qDebug() << "Invalid item or no data.";
        return;
    }

    // Get the model and item
    const QAbstractItemModel *abstractModel = index.model();
    QStandardItemModel *model = const_cast<QStandardItemModel *>(qobject_cast<const QStandardItemModel *>(abstractModel));

    QStandardItem *item = model ? model->itemFromIndex(index) : nullptr;
    if (!item)
    {
        qDebug() << "No item found for index.";
        return;
    }

    // Identify the root node of the clicked item
    QStandardItem *rootNode = item;
    while (rootNode->parent())
    {
        rootNode = rootNode->parent();
    }

    // Debug: Check root node
    // qDebug() << "Root node identified as:" << rootNode->text();

    // Check if the root node has an associated X vector
    if (!xVectors.contains(rootNode))
    {
        MessageQueue *msgQ = MessageQueue::instance();
        msgQ->addWarning("No X vector set for " + rootNode->text() + "\nPlease set an X vector first.");
        return;
    }

    // Retrieve X vector data and label
    QVector<double> xData = xVectors[rootNode].data;
    QStandardItem *xVectorItem = xVectors[rootNode].item;

    QVector<double> yData = qvariant_cast<QVector<double>>(index.data(Qt::UserRole));
    QString yLabel = index.data().toString();

    // Check if the clicked item is the X vector itself
    if (xVectorItem == item)
    {
        // qDebug() << "Clicked item is the X vector. Ignoring.";
        return;
    }

    // Check if the item is already added to the chart
    if (item->data(Qt::UserRole + 1).toBool())
    {
        emit removeSeries(yLabel);
        markItemRemoved(item);
        // qDebug() << "Removed series from chart:" << yLabel;
        return;
    }

    // Add the item to the chart
    markItemAdded(item);
    emit addSeries(yLabel, xData, yData, Qt::AlignLeft);
    // qDebug() << "Added series to chart:" << yLabel;
}

void HiracData::markItemAdded(QStandardItem *item)
{

    item->setData(true, Qt::UserRole + 1);
    item->setForeground(addedSeriesColor);
    auto fnt = item->font();
    fnt.setBold(true);
    // fnt.setPointSize(14);
    item->setFont(fnt);
}

void HiracData::markItemRemoved(QStandardItem *item)
{
    item->setData(false, Qt::UserRole + 1);
    item->setForeground(defaultForegroundColor);
    auto fnt = item->font();
    fnt.setBold(false);
    // fnt.setPointSize(12);
    item->setFont(fnt);
}

double HiracData::calculateMin(const QVector<double> &data)
{
    if (data.isEmpty())
        return std::numeric_limits<double>::quiet_NaN();
    return *std::min_element(data.begin(), data.end());
}

double HiracData::calculateMax(const QVector<double> &data)
{
    if (data.isEmpty())
        return std::numeric_limits<double>::quiet_NaN();
    return *std::max_element(data.begin(), data.end());
}

double HiracData::calculateMean(const QVector<double> &data)
{
    if (data.isEmpty())
        return std::numeric_limits<double>::quiet_NaN();
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double HiracData::calculateMedian(QVector<double> data)
{
    if (data.isEmpty())
        return std::numeric_limits<double>::quiet_NaN();
    std::sort(data.begin(), data.end());
    int n = data.size();
    return (n % 2 == 0) ? (data[n / 2 - 1] + data[n / 2]) / 2.0 : data[n / 2];
}

double HiracData::calculateStdDev(const QVector<double> &data)
{
    if (data.size() < 2)
        return std::numeric_limits<double>::quiet_NaN();
    double mean = calculateMean(data);
    double variance = std::accumulate(data.begin(), data.end(), 0.0,
                                      [mean](double accum, double value)
                                      {
                                          return accum + (value - mean) * (value - mean);
                                      }) /
                      (data.size() - 1);
    return std::sqrt(variance);
}

int HiracData::countDataPoints(const QVector<double> &data)
{
    return data.size();
}

void HiracData::addStatisticalData(QStandardItem *parent, const QVector<double> &data, const QString &unit)
{
    auto createItem = [&](const QString &label, double value) -> QStandardItem *
    {
        QStandardItem *item = new QStandardItem(QString("%1: %2").arg(label).arg(value, 0, 'f', 2)); // 2 decimal precision
        item->setForeground(metaDataColor);
        item->setEditable(false);
        return item;
    };

    auto createUnitItem = [&](const QString &unitText) -> QStandardItem *
    {
        QStandardItem *item = new QStandardItem(QString("Unit: %1").arg(unitText));
        item->setForeground(metaDataColor);
        item->setEditable(false);
        return item;
    };

    // Calculate statistics
    if (!data.isEmpty())
    {
        double min = *std::min_element(data.constBegin(), data.constEnd());
        double max = *std::max_element(data.constBegin(), data.constEnd());
        double mean = std::accumulate(data.constBegin(), data.constEnd(), 0.0) / data.size();

        // Add Min, Max, and Mean nodes
        parent->appendRow(createItem("Min", min));
        parent->appendRow(createItem("Max", max));
        parent->appendRow(createItem("Mean", mean));
    }
    else
    {
        // If data is empty, indicate no statistics available
        QStandardItem *emptyItem = new QStandardItem("No data available for statistics");
        emptyItem->setForeground(metaDataColor);
        emptyItem->setEditable(false);
        parent->appendRow(emptyItem);
    }

    // Add Unit node
    parent->appendRow(createUnitItem(unit.isEmpty() ? "No unit" : unit));
}

void HiracData::onAddtoLeftYAxis(QTreeView *treeView, const QModelIndex &index)
{
    if (!index.isValid() || !index.data(Qt::UserRole).canConvert<QVector<double>>())
    {
        qDebug() << "Invalid index or data.";
        return;
    }

    QVector<double> yData = qvariant_cast<QVector<double>>(index.data(Qt::UserRole));
    QString yLabel = index.data().toString();

    if (!xVectorIndex.isValid())
    {
        qDebug() << "No X vector set. Cannot add series to left Y-axis.";
        return;
    }

    emit addSeries(yLabel, xVectorData, yData, Qt::AlignLeft);

    qDebug() << "Added series to left Y-axis:" << yLabel;
}

void HiracData::onAddtoRightAxis(QTreeView *treeView, const QModelIndex &index)
{
    if (!index.isValid() || !index.data(Qt::UserRole).canConvert<QVector<double>>())
    {
        qDebug() << "Invalid index or data.";
        return;
    }

    QVector<double> yData = qvariant_cast<QVector<double>>(index.data(Qt::UserRole));
    QString yLabel = index.data().toString();

    if (!xVectorIndex.isValid())
    {
        qDebug() << "No X vector set. Cannot add series to right Y-axis.";
        return;
    }

    emit addSeries(yLabel, xVectorData, yData, Qt::AlignRight);

    qDebug() << "Added series to right Y-axis:" << yLabel;
}

void HiracData::removeFromTree(QTreeView *treeView, const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    // Ensure the model is a QStandardItemModel
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(treeView->model());
    if (!model)
    {
        return;
    }

    // Get the item from the model
    QStandardItem *item = model->itemFromIndex(index);
    if (!item)
    {
        return;
    }

    // Ensure the item is a root node
    if (item->parent())
    {
        return;
    }

    // Remove all associated child series
    for (int i = 0; i < item->rowCount(); ++i)
    {
        QStandardItem *child = item->child(i);
        if (child)
        {
            QString label = child->text();
            emit removeSeries(label); // Ensure this signal doesn't cause re-entrant calls
        }
    }

    // Remove all child rows explicitly
    item->removeRows(0, item->rowCount());
    // Remove the root row
    model->invisibleRootItem()->removeRow(item->row());
}
