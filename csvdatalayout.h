#ifndef CSVDATALAYOUT_H
#define CSVDATALAYOUT_H


class CSVDataLayout
{
public:
    CSVDataLayout(int rowLabels=0,int rowUnits=1, int rowDataStart=2, int colCommonX=-1);

    int rowLabels{-1};
    int rowUnits{-1};
    int rowDataStart{-1};
    int colCommonX{-1};
};

#endif // CSVDATALAYOUT_H
