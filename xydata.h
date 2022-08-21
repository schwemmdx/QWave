#ifndef XYDATA_H
#define XYDATA_H

#include "QString"
#include "QVector"
#include "QPointF"

class XYData
{
public:
    XYData();


    void setName(QString);
    QString getName();
    void setSourceFileName(QString);
    void setUnit(QString);
    QString getUnit();

    void setAsXData(void);
    bool isXData(void);
    void clear(void);
    int len(void);
    int size(void);

    //would be better to work with pointers and copy the data afterwards ?
    void setData(QVector<QPointF>);
    void setData(QVector<double>,QVector<double>);
    void setDataFromStr(std::vector<std::string>,std::vector<std::string>);

    QVector<QPointF> getPoints(void);


private:
    QString fileName{""};
    QString name{"Signal"};
    QString unit{"N/A"};
    QVector<QPointF> points;

    double parseSciString(std::string);



};

#endif // XYDATA_H
