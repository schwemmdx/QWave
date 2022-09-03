#ifndef DATAVIEW_H
#define DATAVIEW_H

#include "detailsdialog.h"

#include <QDockWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMouseEvent>

class DataView : public QTreeView
{

public:
    explicit DataView(QDockWidget *parent = nullptr);

signals:
    void xAxisChanged(QModelIndex*);

public slots:
    void loadData(QString file);

private:
    DetailsDialog* details;
    QModelIndex selectedIndex;
    QStandardItemModel importData;
    QMenu* contextMenu;
    QAction* setAsXData;
    QAction* setAsYLData;
    QAction* setAsYRData;
    QAction* remove;
    QAction* inspect;

    void onCustomContextMenu(const QPoint &);
    void actionSetItemAsX();
    void actionFFT();
    void actionStatistics();
    void actionApplyFilter();
    void actionDetails();
    void actionSetItemAsLeftY();
    void actionSetItemAsRightY();

};

#endif // DATAVIEW_H
