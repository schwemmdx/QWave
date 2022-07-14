#ifndef CUSTOMSERIES_H
#define CUSTOMSERIES_H

#include <QObject>
#include <QWidget>
#include <QLineSeries>

#include <QMainWindow>
#include "theme_colors.h"


#include <random>

class CustomSeries: public QLineSeries
{
    Q_OBJECT
public:
    CustomSeries(QObject* parent=nullptr);
    void unselect(void);

signals:
    void seriesSelected(CustomSeries*);

private:
    void clicked(void);




};

#endif // CUSTOMSERIES_H
