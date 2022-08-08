#include "xydata.h"

XYData::XYData()
{

}


void XYData::setDataFromSciStr(std::vector<std::string> dataVec)
{
    QString pointStr;
    QStringList ptParts;

    int exponent;
    double mantissa,result;

    //erase first item because its the units
    dataVec.erase(dataVec.begin());
    foreach(auto &point,dataVec)
    {
        pointStr = QString::fromStdString(point);
        ptParts = pointStr.split("E");
        mantissa = ptParts[0].toDouble();
        exponent = ptParts[1].toInt();
        result = mantissa*pow(10,exponent);
        this->data.append(result);
    }
}

void XYData::setDataFromNumStr(std::vector<std::string> dataVec)
{
    dataVec.erase(dataVec.begin());

    foreach(auto &point,dataVec)
    {
        this->data.append(QString::fromStdString(point).toDouble());
    }
}

void XYData::setName(QString n)
{
    name = n;
}

void XYData::setUnit(QString u)
{
    unit = u;
}

void XYData::clear(void)
{
    data.clear();
}

void XYData::setSourceFileName(QString path)
{
    fileName = path;
}
