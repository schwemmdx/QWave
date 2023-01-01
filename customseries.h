#ifndef CUSTOMSERIES_H
#define CUSTOMSERIES_H

#include <QObject>
#include <QWidget>
#include <QLineSeries>
#include <QMouseEvent>
#include <QMainWindow>


#include <random>

class CustomSeries:public QLineSeries
{
    Q_OBJECT

public:
    CustomSeries(QObject* parent=nullptr);


    bool isSelected(void);
    void setData(QVector<QPointF> data);

signals:
    void seriesSelected(CustomSeries*);
    void newStatusMessage(QString);


public slots:
    void selected(const QPointF &point);
    void unselect(void);
    void setXLimits(float,float);
    void setYLimitsL(float,float);
    void setYLimitsR(float,float);

    //void mousePressEvent(QMouseEvent* );



private:
    bool selectionState;
    bool selectable{true};


protected:
     //void mouseHover(const QPointF,bool) override;







};

#endif // CUSTOMSERIES_H
