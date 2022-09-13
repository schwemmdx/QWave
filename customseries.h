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
    void setData(QVector<QPointF> data);

signals:
    void seriesSelected(CustomSeries*);
    void newStatusMessage(QString);

public slots:
    void setXLimits(float,float);
    void setYLimitsL(float,float);
    void setYLimitsR(float,float);

private:
    bool selectionState;
    void mouseHover(const QPointF,bool);





};

#endif // CUSTOMSERIES_H
