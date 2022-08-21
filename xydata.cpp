#include "xydata.h"
#include "QDebug"

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

void XYData::setDataFromStr(std::vector<std::string> xData,std::vector<std::string> yData)
{
    QPointF ptBuf;
    int exponent;
    double mantissa,result;

    //erase first item because its the units
    xData.erase(xData.begin());
    yData.erase(yData.begin());
    double x,y;

    for(int i = 0;i<yData.size();i++)
    {
       if(QString::fromStdString(xData[i]).contains('E'))
       {
           ptBuf.setX(XYData::parseSciString(xData[i]));
       }
       else
       {
           ptBuf.setX(QString::fromStdString(xData[i]).toDouble());
       }
       if(QString::fromStdString(yData[i]).contains('E'))
       {
           ptBuf.setY(XYData::parseSciString(yData[i]));
       }
       else
       {
           ptBuf.setY(QString::fromStdString(yData[i]).toDouble());
       }
       points.append(ptBuf);
    }


}

QVector<QPointF> XYData::getPoints(void)
{
    return this->points;
}

void XYData::setName(QString n)
{
    this->name = n;
}

void XYData::setUnit(QString u)
{
    this->unit = u;
}

void XYData::clear(void)
{
    this->points.clear();
}

void XYData::setSourceFileName(QString path)
{
    fileName = path;
}

 QString XYData::getUnit()
{
    return unit;
}
 QString XYData::getName()
{
    return name;
}
