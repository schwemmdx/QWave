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
    static QString readRawTxt(QString);
    static void toQTable(QString*);
    static void toQTree(QString*);
    static void toHF5(QString*);
    static void toJSON(QString*);

    QVector<QVector<QString>> rawTable;
};

#endif // DATAIMPORTER_H
