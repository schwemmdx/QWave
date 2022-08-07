#include "dataimporter.h"
#include "QFile"

#include "QDebug"
#include "QMessageBox"

#include "third-party/rapidcsv.h"

DataImporter::DataImporter()
{

}

QString DataImporter::readRawTxt(QString path)
{
   QString content;
   QFile file(path);

   if(file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       content = file.readAll();
       file.close();
   }
   else
   {
       QMessageBox msgBox;
       msgBox.setText("Die Datei "+path + " konnte nicht geöffnet werden!");
       msgBox.exec();
   }
   return content;
}

void DataImporter::toQTree(QString*)
{

}

void DataImporter::fromCSV(QString path,QString colDelimiter,QString rowDelimiter)
{

    //> FEATURE ADD IN: Import Wizard with live VIEW
    //Currently hard coded layout (see .h file) (Agilent Oscilloscopes)

    CSVDataLayout csvLayout;

    QMap<QString,std::vector<std::string>> csvData;
    QMap<QString,QString> csvUnits;

    rapidcsv::Document csvFile(path.toStdString());

    foreach (auto &name, csvFile.GetColumnNames())
    {
        auto dataBuf(csvFile.GetColumn<std::string>(name));
        auto unit(csvFile.GetCell<std::string>(name,0));
        dataBuf.erase(dataBuf.begin());
        csvUnits.insert(QString::fromStdString(name),QString::fromStdString(unit));
        csvData.insert(QString::fromStdString(name),dataBuf);
    }












}

double DataImporter::sciStringtoDouble(const QString*)
{

    return 0.0;
}
