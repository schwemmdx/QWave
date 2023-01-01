#ifndef CHARTCONTAINER_H
#define CHARTCONTAINER_H


#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QMenu>
#include <QValueAxis>


#include "customseries.h"
#include "chartcrosshair.h"
#include "chartmarker.h"


class ChartContainer : public QChartView
{
    Q_OBJECT

public:
    explicit ChartContainer(QWidget *parent = nullptr);
    ~ChartContainer();
    void deselect(void);
    QList<CustomSeries*> tracies;


signals:
    //void chartSelected(ChartContainer*);
    void seriesSelectionChanged(CustomSeries*);
    void newStatusMessage(QString);
    void zoomApplied(int);
    void scrollApplied(int);
    void markerRequested(QPointF);

public slots:
    void selectedSeriesChanged(CustomSeries*);

    void setTitle(QString);
    void addDataSeries(QVector<double> ,QVector<double> ,QString ,QString );
    void setCrosshairVisibility(bool);
    //void themeChange(int);
    bool isCrosshairVisible(void );


 
private:
    QWidget* pParent;
    QMenu* contextMenu;

    QChart *chart;
    qreal zoomFactor{1.0};
    qreal scrollFactor{1.0};
    qreal stepModifier {1.0};
    ChartCrosshair* m_crosshair;
    QList<ChartMarker*> chartMarkers;
    QValueAxis* leftYAxis;
    QValueAxis* rightYAxis;
    QValueAxis* xAxis;
    void requestNewMarker(QPointF );
    bool middleMousePressed;
    QPoint middlePressStartPos,middlePressEndPos,fistVal;



private slots:

    void newMsgFromSeries(QString);

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
