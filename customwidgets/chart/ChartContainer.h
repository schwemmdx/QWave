#ifndef CHARTCONTAINER_H
#define CHARTCONTAINER_H


#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QMenu>
#include <QValueAxis>


#include "CustomSeries.h"
#include "ChartCrosshair.h"
#include "ChartMarker.h"
#include "CustomChart.h"

class ChartContainer : public QChartView
{
    Q_OBJECT
public:
    explicit ChartContainer(QWidget *parent = nullptr);
    ~ChartContainer();

    //void deselect(void);
    QList<CustomSeries*> getSeriesInChart(void);
    std::tuple<int,int> getNumSeriesPerAxis(void);
    CustomChart* chart;



signals:

    void newStatusMessage(QString);
    void zoomApplied(int);
    void scrollApplied(int);
    void markerRequested(QPointF);
    void newTraceSelection(CustomSeries*);
    void updateCursorData(QList<void*>);

public slots:

    void addSeriesToChart(const QString &label, const QVector<double> &xData, const QVector<double> &yData, Qt::Alignment alignment);
    void removeSeriesFromChart(const QString &label);
    //void addDataSeries(QVector<double> ,QVector<double> ,QString ,QString ,int);
    void setCrosshairVisibility(bool);
    //void themeChange(int);
    bool isCrosshairVisible(void );



 
private:
    QWidget* pParent;
    QMenu* contextMenu;

    qreal zoomFactor{1.0};
    qreal scrollFactor{1.0};
    qreal stepModifier {1.0};
    ChartCrosshair* m_crosshair;
    QList<ChartMarker*> chartMarkers;
    QValueAxis* leftYAxis;
    QValueAxis* rightYAxis;
    QValueAxis* xAxis;
    QMap<QString,CustomSeries*> chartContent;
    void requestNewMarker(QPointF );
    bool middleMousePressed;
    QPoint middlePressStartPos,middlePressEndPos,fistVal;


    //QList<CursorInfo*> cursorInfos;


private slots:

    void onCustomContextMenu(const QPoint &);
    //Context Menu Entries

    void clearSelectedSeries(void);
    void clearAllSeries(void);
    void resetZoom(void);
    void setLimits(void);
    void removeMarkers(void);

 protected:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent*)override;
    void wheelEvent(QWheelEvent*) override;

   // bool eventFilter(QObject* obj, QEvent* event);
/*
    protected: 
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
*/
};

#endif // CHARTCONTAINER_H
