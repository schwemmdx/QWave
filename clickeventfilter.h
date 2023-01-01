#ifndef CLICKEVENTFILTER_H
#define CLICKEVENTFILTER_H

#include <QObject>
#include <QEvent>

class ClickEventFilter:public QObject
{
    Q_OBJECT
public:
    ClickEventFilter();

protected:
   // bool eventFilter(QObject*,QEvent*);
};

#endif // CLICKEVENTFILTER_H
