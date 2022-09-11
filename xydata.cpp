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

void XYData::setDataFromStr(std::vector<std::string> data)
{
    double val;
    int exponent;
    double mantissa,result;

    //erase first item because its the units
    data.erase(data.begin());

    double x,y;

    for(int i = 0;i<data.size();i++)
    {
       if(QString::fromStdString(data[i]).contains('E'))
       {
           val = XYData::parseSciString(data[i]);
       }
       else
       {
           val = QString::fromStdString(data[i]).toDouble();
       }
       points.append(val);
    }


}

QVector<double> XYData::getPointVec(void)
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

 int XYData::len()
 {
     return points.length();
 }

 double XYData::getMin()
 {
     return *std::min(points.cbegin(),points.cend());
 }

 double XYData::getMax()
 {
     return *std::max(points.cbegin(),points.cend());
 }

 double XYData::getAVG()
 {
     double sum =0;
     for(auto &pt: points)
     {
         sum+=pt;
     }
     sum/=points.length();
     return sum;
 }

 double XYData::getMedian()
 {
     return -1;
 }

 double XYData::getRMS()
 {
    double sum=0;
    foreach (auto &elem, points) {
        sum+=std::pow(elem,2);
    }

    sum/=points.length();
    return std::sqrt(sum);

 }
