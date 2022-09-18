#ifndef DATAVIEW_H
#define DATAVIEW_H

#include "detailsdialog.h"

#include <QDockWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMouseEvent>
#include <QModelIndex>
#include <QIcon>


#define APPEND_LEFT 0
#define APPEND_RIGHT 1

class DataView : public QTreeView
{
    Q_OBJECT

public:
    explicit DataView(QDockWidget *parent = nullptr);
    ~DataView();

signals:
    void xAxisChanged(QVector<double>*);
    void appendData(QVector<double>,QVector<double>,QString,QString);

public slots:
    void loadData(QString file);

private:

    QVector<double> xData;
    QString xUnit;

    //void selectionChanged(const QItemSelection &, const QItemSelection &);
    DetailsDialog* details;
    QModelIndex selectedIndex;
    QStandardItemModel importData;
    QMenu* contextMenu;
    QAction* setAsXData;
    QAction* setAsYLData;
    QAction* setAsYRData;
    QAction* remove;
    QAction* inspect;


    const QIcon xIcon{":/icons/icons/x-coord.png"};
    const QIcon yIcon{":/icons/icons/y-coord.png"};
    const QIcon csvIcon{":/icons/icons/icons8-csv-80.png"};

    //void mouseDoubleClickEvent(QMouseEvent * event);

    void onCustomContextMenu(const QPoint &);
    void actionSetItemAsX();
    void actionFFT();
    void actionStatistics();
    void actionApplyFilter();
    void actionDetails();
    void actionSetItemAsLeftY();
    void addToLeftYByDoubleClick(const QModelIndex &);
    void actionSetItemAsRightY();

};

#endif // DATAVIEW_H
