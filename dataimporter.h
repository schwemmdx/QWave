#ifndef DATAIMPORTER_H
#define DATAIMPORTER_H

#include "QVector"
#include "QVector2D"
#include "QString"


#include "xydata.h"
#include "csvdatalayout.h"


class DataImporter
{
public:
    DataImporter();
    static QVector<XYData> fromCSV(QString path, QString colDelimiter = ",", QString rowDelimiter = "\n"); //>! Main function to parse a new data object and represent it as QTreeObject

private:
    CSVDataLayout csvLayout;
    static QString readRawTxt(QString);
    static void toQTable(QString*); // useful ?
    static void toQTree(QString*); // useful ?
    static void toHF5(QString*);  // to much work ?
    static void toJSON(QString*);
    static void csvWizardDlg(CSVDataLayout*);
    static double sciStringtoDouble(const QString*);





    QVector<QVector<QString>> rawTable;
};

#endif // DATAIMPORTER_H
