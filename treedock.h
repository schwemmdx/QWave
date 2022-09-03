#ifndef TREEDOCK_H
#define TREEDOCK_H

#include <QDockWidget>
#include <QStandardItemModel>
#include "dataview.h"

namespace Ui {
class TreeDock;
}

class TreeDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit TreeDock(QWidget *parent = nullptr);
    ~TreeDock();
    void expandAll(void);

public slots:
    void loadData(QString);


private:
    QStandardItemModel* importContent;
    Ui::TreeDock *ui;
    DataView dataView{this};

    //void mousePressEvent(QMouseEvent *event);


};

#endif // TREEDOCK_H
