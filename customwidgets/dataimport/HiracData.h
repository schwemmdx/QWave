#ifndef HIRACDATA_H
#define HIRACDATA_H

#include <QObject>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVector>
#include <QHash>
#include <QString>
#include <QColor>
#include <QMenu>
#include <QModelIndex>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include "ThemeColors.h"

// Forward declaration
class ChartContainer;

struct XVectorInfo
{
    QVector<double> data;
    QStandardItem *item;
};

class HiracData : public QObject
{
    Q_OBJECT

public:
    explicit HiracData(QObject *parent = nullptr);
    virtual ~HiracData() {}

    void appendData(QTreeView *treeView, const QString &dataPath);

    // Setup context menu for the tree view
    void setupContextMenu(QTreeView *treeView);

signals:
    // Signals to communicate with ChartContainer
    void addSeries(const QString &label, const QVector<double> &xData, const QVector<double> &yData, Qt::Alignment alignment);
    void removeSeries(const QString &label);
    void dataLoadStarted();
    void dataLoadProgress(int progress);
    void dataLoadFinished();

public slots:
    void onSetAsXVector(QTreeView *treeView, const QModelIndex &index);
    void removeFromTree(QTreeView *treeView, const QModelIndex &index);
    void onAddtoLeftYAxis(QTreeView *treeView, const QModelIndex &index);
    void onAddtoRightAxis(QTreeView *treeView, const QModelIndex &index);
    void handleItemClick(const QModelIndex &index);

protected:
    QHash<QStandardItem *, XVectorInfo> xVectors;
    // Datatype specific implementation of loading the actual data
    virtual void loadData(const QString &dataPath, QStandardItemModel *model) = 0;
    // Utility methods for statistical calculations
    void addStatisticalData(QStandardItem *parent, const QVector<double> &data, const QString &unit);
    double calculateMin(const QVector<double> &data);
    double calculateMax(const QVector<double> &data);
    double calculateMean(const QVector<double> &data);
    double calculateMedian(QVector<double> data);
    double calculateStdDev(const QVector<double> &data);
    int countDataPoints(const QVector<double> &data);

    // Methods to update item states
    void markItemAdded(QStandardItem *item);
    void markItemRemoved(QStandardItem *item);

    // Color definitions

    QColor xVectorColor{QColor(175, 82, 222)};         // Purple for X vector
    QColor addedSeriesColor{QColor(52, 199, 89)};      // Light blue for added series
    QColor defaultForegroundColor{QColor(29, 29, 31)}; // Default text color (Monokai Foreground)

    // State tracking for the X vector
    QVector<double> xVectorData;
    QModelIndex xVectorIndex;

    QColor rootColor{QColor(0, 122, 255)};
    QColor childColor{QColor(29, 29, 31)};
    QColor metaDataColor{QColor(142, 142, 147)};

    QIcon rootIcon, childIcon, metaDataIcon;
};

#endif // HIRACDATA_H
