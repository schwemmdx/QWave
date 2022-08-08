#ifndef XYDATA_H
#define XYDATA_H

#include "QString"
#include "QVector"

class XYData
{
public:
    XYData();

    QVector<double>* getY(void);
    QVector<double>* getX(void);
    void setName(QString);
    void setSourceFileName(QString);
    void setUnit(QString);

    void setAsXData(void);
    bool isXData(void);
    void clear(void);
    int len(void);
    int size(void);

    //would be better to work with pointers and copy the data afterwards ?
    void setData(QVector<double>);
    void setDataFromSciStr(std::vector<std::string>);
    void setDataFromNumStr(std::vector<std::string>);


private:
    QString fileName{""};
    QString name{"Signal"};
    QString unit{"N/A"};
    XYData* pXData{nullptr};
    QVector<double> data;



};

#endif // XYDATA_H
