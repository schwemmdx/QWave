#ifndef SCIENTIFICFORMATTER_H
#define SCIENTIFICFORMATTER_H

#include <QString>

class ScientificFormatter {
public:
    // Static function to convert a float to a string with scientific suffix
    static QString toScientificSuffix(float value,int precision = 3);
    static QString reformatStr(QString str);
};

#endif // SCIENTIFICFORMATTER_H
