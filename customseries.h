#ifndef CUSTOMSERIES_H
#define CUSTOMSERIES_H

#include <QObject>
#include <QWidget>
#include <QLineSeries>
#include <QMouseEvent>
#include <QMainWindow>




class CustomSeries:public QLineSeries
{
    Q_OBJECT

public:
    CustomSeries(QObject* parent=nullptr);


    bool isSelected(void);
    void setData(QVector<QPointF> data,QString label="");
    int findClosestPointIndex(const QPointF &chartPosition);
    float xmin;
    float ymin;
    float xmax;
    float ymax;

    

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
private slots:
    void onHover(const QPointF &point,bool state);
    void onPointPress(const QPointF &point);
    void setLimits(QVector<QPointF> pData );
    

private:
    bool selectionState;
    bool selectable{true};
    QVector<QPointF> orginalData; const
    QVector<QPointF> modData;


protected:









};

#endif // CUSTOMSERIES_H
