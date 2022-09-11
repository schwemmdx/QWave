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

    void setAsXData(bool);
    bool isXData(void);
    void clear(void);
    int len(void);
    int size(void);
    double getMin(void);
    double getMax(void);
    double getMedian(void);
    double getAVG(void);
    double getRMS(void);

    //would be better to work with pointers and copy the data afterwards ?
    void setData(QVector<double>);
    void setDataFromStr(std::vector<std::string>);
    QVector<double> getPointVec(void);


private:
    QString fileName{""};
    QString name{"Signal"};
    QString unit{"N/A"};
    bool isX{false};

    QVector<double> points{};
    double parseSciString(std::string);

};

#endif // XYDATA_H
