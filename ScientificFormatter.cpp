
#include "ScientificFormatter.h"
#include <cmath>

QString ScientificFormatter::reformatStr(QString str)
{
    return ScientificFormatter::toScientificSuffix(str.toFloat(),3);
}

QString ScientificFormatter::toScientificSuffix(float value,int digits) {
    // If value is 0, return a zero string (no suffix needed)
    if (value == 0) {
        return "0";
    }

    QString suffix;
    double magnitude = std::log10(std::abs(value));  // Get the magnitude of the number
    
    if (magnitude >= 9) {
        suffix = "G";  // Giga (10^9)
        value /= 1e9;
    } else if (magnitude >= 6) {
        suffix = "M";  // Mega (10^6)
        value /= 1e6;
    } else if (magnitude >= 3) {
        suffix = "k";  // Kilo (10^3)
        value /= 1e3;
    } else if (magnitude >= 0) {
        suffix = "";   // No suffix for values between 1 and 999
    } else if (magnitude >= -3) {
        suffix = "m";  // Milli (10^-3)
        value *= 1e3;
    } else if (magnitude >= -6) {
        suffix = "µ";  // Micro (10^-6)
        value *= 1e6;
    } else if (magnitude >= -9) {
        suffix = "n";  // Nano (10^-9)
        value *= 1e9;
    } else {
        return QString::number(value); // Return the number without suffix for extreme cases
    }

    // Return the value formatted with one decimal and the appropriate suffix
    return QString::number(value, 'f', digits) + suffix;
}