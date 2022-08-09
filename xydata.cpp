#include "xydata.h"

XYData::XYData()
{

}


double XYData::parseSciString(std::string point)
{
    double mantissa{0},exponent{1.0};
    QString pointStr = QString::fromStdString(point);
    QStringList ptParts = pointStr.split("E");
    mantissa = ptParts[0].toDouble();
    exponent = ptParts[1].toInt();
    return mantissa*pow(10,exponent);
}

void XYData::setDataFromSciStr(std::vector<std::string> xData,std::vector<std::string> yData)
{
    QString pointStr;
    QStringList ptParts;
    QPointF ptBuf;
    int exponent;
    double mantissa,result;

    //erase first item because its the units
    xData.erase(xData.begin());
    yData.erase(yData.begin());
    for(int i = 0;i<xData.size();i++)
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
