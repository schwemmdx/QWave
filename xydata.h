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
    void setSourceFileName(QString);
    void setUnit(QString);

    void setAsXData(void);
    bool isXData(void);
    void clear(void);
    int len(void);
    int size(void);

    //would be better to work with pointers and copy the data afterwards ?
    void setData(QVector<QPointF>);
    void setData(QVector<double>,QVector<double>);
    void setDataFromSciStr(std::vector<std::string>,std::vector<std::string>);
    void setDataFromNumStr(std::vector<std::string>,std::vector<std::string>);


private:
    QString fileName{""};
    QString name{"Signal"};
    QString unit{"N/A"};
    QVector<QPointF> points;

    double parseSciString(std::string);



};

#endif // XYDATA_H
