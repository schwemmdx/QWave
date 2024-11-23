#ifndef ACTIONLISTWIDGET_H
#define ACTIONLISTWIDGET_H

#include <QWidget>
#include <QAction>
#include <QVector>
#include <QGridLayout>

class QVBoxLayout;

class ActionListWidget : public QWidget {
    Q_OBJECT

public:
    explicit ActionListWidget(QWidget *parent = nullptr);

private:
    QVector<QAction*> createExampleActions();
    void setupList(QGridLayout *layout, QVector<QAction*> actions);

    const int widgetWidth = 250;
};

#endif // ACTIONLISTWIDGET_H
