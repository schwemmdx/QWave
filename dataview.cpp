#include "dataview.h"
#include "dataimporter.h"
#include "detailsdialog.h"

#include <QDockWidget>
#include <QMenu>
#include <QVariant>

DataView::DataView(QDockWidget *parent)
    : QTreeView{parent}
{
    contextMenu = new QMenu(this);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu->addAction("Details", this, &DataView::actionDetails);
    contextMenu->addAction("Set as X Axis", this, &DataView::actionSetItemAsX);
    contextMenu->addAction("Add to Left Y-Axis", this, &DataView::actionSetItemAsLeftY);
    contextMenu->addAction("Add to Right Y-Axis", this, &DataView::actionSetItemAsRightY);
    contextMenu->addAction("Statistical Analysis", this, &DataView::actionStatistics);
    contextMenu->addAction("Compute FFT", this, &DataView::actionFFT);
    contextMenu->addAction("Apply Filtering", this, &DataView::actionApplyFilter);

    setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(this, &DataView::doubleClicked, this, &DataView::addToLeftYByDoubleClick);

    connect(this, &DataView::customContextMenuRequested,
            this, &DataView::onCustomContextMenu);
    setExpandsOnDoubleClick(false);

}

DataView::~DataView()
{

}

void DataView::onCustomContextMenu(const QPoint &point)
{
    selectedIndex = this->indexAt(point);
      if (selectedIndex.isValid() ) {
          contextMenu->exec(this->viewport()->mapToGlobal(point));
      }
}
void DataView::loadData(QString file)
{

    QStringList buf;
    buf = file.split("/");
    QString fileName = buf.constLast();

    auto data = DataImporter::fromCSV(file);
    QStandardItem* csvFile =  new QStandardItem(fileName);
    //csvFile->setEditable(false);
    csvFile->setIcon(QIcon(":./icons//icon_data/csv.png"));

    QStandardItemModel* importContent = new QStandardItemModel();
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
        signal->setIcon(QIcon(":/icons/icon_data/radio-waves.png"));

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
    }
    // Path Information


    //importContent->appendRow(csvFile);

    setModel(importContent);
    setHeaderHidden(true);
    setColumnWidth(0,160);
    setColumnWidth(1,100);
    //expandToDepth();


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

void DataView::addToLeftYByDoubleClick(const QModelIndex &idx)
{
    if(idx.isValid()& !idx.parent().parent().isValid() )
    {

       emit appendData(idx.data(Qt::UserRole+1).value<QVector<QPointF>>(),APPEND_LEFT);
    }
}


void DataView::actionSetItemAsX()
{

        qDebug() << selectedIndexes().first();
        auto idx = (selectedIndexes().first());
        auto numChildren = idx.model()->rowCount();
        auto item = static_cast<QStandardItem*>(idx.internalPointer())->child(idx.row());
        QString txt;
        for(int i = 0;i<numChildren;i++)
        {
           //txt =  static_cast<QStandardItem*>(idx.internalPointer())->child(i)->text();
           //txt = txt.remove(" @ X");
           static_cast<QStandardItem*>(idx.internalPointer())->child(i)->setIcon(QIcon(":/icons/icon_data/radio-waves.png"));
           //static_cast<QStandardItem*>(idx.internalPointer())->child(i)->setText(txt);
        }
        //QString currTxt = item->text();
        //item->setText(currTxt+ " @ X");

        item->setIcon(QIcon(":/icons/icon_data/1 (107).png"));
        xData =  idx.data(Qt::UserRole+1).value<QVector<double>>();
        qDebug() << xData;
}

void DataView::actionDetails()
{
    DetailsDialog* details = new DetailsDialog(this);
    details->show();

}

void DataView::actionSetItemAsLeftY()
{

}

void DataView::actionSetItemAsRightY()
{

}

void DataView::actionStatistics()
{

}

void DataView::actionFFT()
{

}

void DataView::actionApplyFilter()
{

}
