#include "datawidget.h"
#include "dataimporter.h"
#include "detailsdialog.h"
#include "chartcontainer.h"


#include <QDockWidget>
#include <QMenu>
#include <QVariant>



DataWidget::DataWidget(QWidget *parent)
    : QTreeView{parent}
{
    contextMenu = new QMenu(this);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu->addAction("Details", this, &DataWidget::actionDetails);
    contextMenu->addAction("Set as X Axis", this, &DataWidget::actionSetItemAsX);
    contextMenu->addAction("Add to Left Y-Axis", this, &DataWidget::actionSetItemAsLeftY);
    contextMenu->addAction("Add to Right Y-Axis", this, &DataWidget::actionSetItemAsRightY);
    contextMenu->addAction("Statistical Analysis", this, &DataWidget::actionStatistics);
    contextMenu->addAction("Compute FFT", this, &DataWidget::actionFFT);
    contextMenu->addAction("Apply Filtering", this, &DataWidget::actionApplyFilter);

    setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(this, &DataWidget::doubleClicked, this, &DataWidget::addToLeftYByDoubleClick);

    connect(this, &DataWidget::customContextMenuRequested,
            this, &DataWidget::onCustomContextMenu);
    setExpandsOnDoubleClick(false);
    setAnimated(true);
    setLayoutDirection(Qt::LeftToRight);

}

DataWidget::~DataWidget()
{

}

void DataWidget::onCustomContextMenu(const QPoint &point)
{
    selectedIndex = this->indexAt(point);
      if (selectedIndex.isValid() ) {
          contextMenu->exec(this->viewport()->mapToGlobal(point));
      }
}
void DataWidget::loadData(QString file)
{

    QStringList buf;
    buf = file.split("/");
    QString fileName = buf.constLast();

    auto data = DataImporter::fromCSV(file);
    QStandardItem* csvFile =  new QStandardItem(fileName);
    //csvFile->setEditable(false);
    csvFile->setIcon(csvIcon);

    importContent = new QStandardItemModel();
    importContent->setColumnCount(2);



    QList<QStandardItem*> unit;
    QList<QStandardItem*> length;
    QList<QStandardItem*> min;
    QList<QStandardItem*> max;
    QList<QStandardItem*> avg;
    QList<QStandardItem*> rms;
    QList<QStandardItem*> median;

    //QList<QStandardItem*> path;
    //path.append(new QStandardItem("Path"));
    //path.append(new QStandardItem(file));

    //csvFile->appendRow(path);

    foreach(auto sig, data)
    {
        QStandardItem* signal =  new QStandardItem();
        signal->setText(sig.getName());
        signal->setIcon(yIcon);

        signal->setData(QVariant::fromValue(sig.getPointVec()),Qt::UserRole+1);

        unit.clear();
        unit.append(new QStandardItem("Unit"));
        unit.append(new QStandardItem(sig.getUnit()));

        length.clear();
        length.append(new QStandardItem("Meas. Count"));
        length.append(new QStandardItem(QString::number(sig.len())));

        max.clear();
        max.append(new QStandardItem("Maximum"));
        max.append(new QStandardItem(QString::number(sig.getMax())));

        min.clear();
        min.append(new QStandardItem("Minimum"));
        min.append(new QStandardItem(QString::number(sig.getMin())));

        avg.clear();
        avg.append(new QStandardItem("Average"));
        avg.append(new QStandardItem(QString::number(sig.getAVG())));

        rms.clear();
        rms.append(new QStandardItem("RMS"));
        rms.append(new QStandardItem(QString::number(sig.getRMS())));

        median.clear();
        median.append(new QStandardItem("Median"));
        median.append(new QStandardItem(QString::number(sig.getMedian())));

        signal->appendRow(unit);
        signal->appendRow(min);
        signal->appendRow(max);
        signal->appendRow(avg);
        signal->appendRow(rms);
        signal->appendRow(median);
        importContent->appendRow(signal);
        signalList.append(signal);

    }
    // Path Information


    //importContent->appendRow(csvFile);

    setModel(importContent);
    setHeaderHidden(true);
    setColumnWidth(0,160);
    setColumnWidth(1,100);
    //expandToDepth();
    this->setIconSize(QSize(16,16));
    selectionModel()->select(model()->index(0,0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
    actionSetItemAsX();
    selectionModel()->select(model()->index(0,0), QItemSelectionModel::Deselect | QItemSelectionModel::Rows);

}
/*
void DataView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(!selected.empty())
    {
        selectedIndex = selected.indexes().constFirst();
    }
}
*/

void DataWidget::addToLeftYByDoubleClick(const QModelIndex &idx)
{
    actionSetItemAsLeftY();
}



void DataWidget::actionSetItemAsX()
{
        auto idx = (selectedIndexes().first());
        auto numChildren = idx.model()->rowCount();
        auto item = static_cast<QStandardItem*>(idx.internalPointer())->child(idx.row());
        QString txt;
        for(int i = 0;i<numChildren;i++)
        {
           static_cast<QStandardItem*>(idx.internalPointer())->child(i)->setIcon(yIcon);
        }
        item->setIcon(xIcon);
        xData =  idx.data(Qt::UserRole+1).value<QVector<double>>();
        xUnit = item->child(0,1)->text();

}

void DataWidget::actionDetails()
{
    DetailsDialog* details = new DetailsDialog(this);
    details->show();

}

bool DataWidget::isIndexValidforAppending(void)
{
    foreach(auto &signal,signalList)
    {
        if(signal->index() == selectedIndexes().first())
        {
            selectedIndex = selectedIndexes().first();
            return true;
        }
    }

    return false;
}

void DataWidget::actionSetItemAsLeftY()
{
    if(isIndexValidforAppending())
    {
        auto idx = selectedIndex;
        auto yData = idx.data(Qt::UserRole+1).value<QVector<double>>();
        emit appendData(xData,yData,xUnit,static_cast<QStandardItem*>(idx.internalPointer())->child(idx.row())->child(0,1)->text(),ChartContainer::AXIS_LEFT);
    }

}

void DataWidget::actionSetItemAsRightY()
{

    if(isIndexValidforAppending())
    {
        auto idx = selectedIndex;
        auto yData = idx.data(Qt::UserRole+1).value<QVector<double>>();
        emit appendData(xData,yData,xUnit,static_cast<QStandardItem*>(idx.internalPointer())->child(idx.row())->child(0,1)->text(),ChartContainer::AXIS_RIGHT);
    }
}

void DataWidget::actionStatistics()
{

}

void DataWidget::actionFFT()
{

}

void DataWidget::actionApplyFilter()
{

}
