#include "dataimporter.h"
#include "QFile"

#include "QDebug"
#include "QMessageBox"

#include "third-party/rapidcsv.h"

#include "xydata.h"

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

QVector<XYData> DataImporter::fromCSV(QString path,QString colDelimiter,QString rowDelimiter)
{

    //> FEATURE ADD IN: Import Wizard with live VIEW
    //Currently hard coded layout (see .h file) (Agilent Oscilloscopes)

    CSVDataLayout csvLayout;

    QVector<XYData> csvData;
    rapidcsv::Document csvFile(path.toStdString());
    XYData dataBuf;
    //auto xVec = csvFile.GetColumn<std::string>("x-axis");
    foreach (auto &name, csvFile.GetColumnNames())
    {
        dataBuf.clear();
        dataBuf.setSourceFileName(path);
        dataBuf.setName(QString::fromStdString(name));
        dataBuf.setUnit(QString::fromStdString(csvFile.GetCell<std::string>(name,0)));
        //Chekc wheater the formatting e.g. .1234E-3 or 0.001234
        dataBuf.setDataFromStr(csvFile.GetColumn<std::string>(name));
        csvData.append(dataBuf);
    }

    return csvData;

}

double DataImporter::sciStringtoDouble(const QString*)
{
    return 0.0;
}
