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
    connect(this, &DataView::doubleClicked, this, &DataView::addToLeftYByDoubleClick);

    connect(this, &DataView::customContextMenuRequested,
            this, &DataView::onCustomContextMenu);

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
    csvFile->setEditable(false);
    csvFile->setIcon(QIcon(":/icon_data/csv.png"));

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
        QStandardItem* signal =  new QStandardItem(sig.getName());

        signal->setData(QVariant::fromValue(sig.getPoints()),Qt::UserRole+1);

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
        csvFile->appendRow(signal);
    }
    // Path Information


    importContent->appendRow(csvFile);

    setModel(importContent);
    setHeaderHidden(true);
    setColumnWidth(0,160);
    setColumnWidth(1,100);
    expandToDepth(0);


}


void DataView::addToLeftYByDoubleClick(const QModelIndex &idx)
{
    if(idx.isValid()& !idx.parent().parent().isValid() )
    {
       emit appendData(idx.data(Qt::UserRole+1).value<QVector<QPointF>>(),APPEND_LEFT);
    }
}


void DataView::actionSetItemAsX()
{

    qDebug() << "Selecting Index" << selectedIndex << "as X Axis";
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
