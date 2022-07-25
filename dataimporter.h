#ifndef DATAIMPORTER_H
#define DATAIMPORTER_H

#include "QVector"
#include "QVector2D"
#include "QString"

class DataImporter
{
public:
    DataImporter();
private:
    static void readCSV(QString);
    QVector<QVector<QString>> rawTable;
};

#endif // DATAIMPORTER_H
