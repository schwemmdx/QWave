#include "csvdatalayout.h"

CSVDataLayout::CSVDataLayout(int rowLabels,int rowUnits, int rowDataStart, int colCommonX)
{
    this->rowDataStart = rowDataStart;
    this->rowLabels = rowLabels;
    this->rowUnits = rowUnits;
    this->colCommonX = colCommonX;
}
