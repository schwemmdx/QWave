#include <QString>
#include <QDebug>
//#include <QMouseEvent>

#include "dataimporter.h"
#include "treedock.h"
#include "ui_treedock.h"



TreeDock::TreeDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TreeDock)
{
    ui->setupUi(this);
    //ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);


}

TreeDock::~TreeDock()
{
    delete ui;
}

void TreeDock::loadData(QString file)
{



    QStringList buf;
    buf = file.split("/");
    QString fileName = buf.constLast();

    auto data = DataImporter::fromCSV(file);
    QStandardItem* csvFile =  new QStandardItem(fileName);
    csvFile->setIcon(QIcon(":/icon_data/csv.png"));

    QStandardItemModel* importContent = new QStandardItemModel();
    importContent->setColumnCount(3);

    QList<QStandardItem*> unit;
    QList<QStandardItem*> path;
    path.append(new QStandardItem("Path"));
    path.append(new QStandardItem(file));

    csvFile->appendRow(path);
    foreach(auto sig, data)
    {
        QStandardItem* signal =  new QStandardItem(sig.getName());
        unit.clear();
        unit.append(new QStandardItem("Unit"));
        unit.append(new QStandardItem(sig.getUnit()));
        signal->appendRow(unit);
        csvFile->appendRow(signal);
    }
    // Path Information


    importContent->appendRow(csvFile);

    dataView.setModel(importContent);
    dataView.expandToDepth(0);

}

/*
void TreeDock::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug() << "works!\n";
        //emit customContextMenuRequested(event->pos());
    }
}
*/
