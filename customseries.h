#ifndef CUSTOMSERIES_H
#define CUSTOMSERIES_H

#include <QObject>
#include <QWidget>
#include <QLineSeries>

#include <QMainWindow>


#include <random>

class CustomSeries: public QLineSeries
{
    Q_OBJECT
public:
    CustomSeries(QObject* parent=nullptr);
    void unselect(void);
    void select(void);
    bool isSelected(void);

signals:
    void seriesSelected(CustomSeries*);
    void newStatusMessage(QString);

private:
    bool selectionState;
    void mouseHover(const QPointF,bool);




};

#endif // CUSTOMSERIES_H
