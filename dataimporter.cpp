#include "dataimporter.h"
#include "QFile"

#include "QDebug"
#include "QMessageBox"

DataImporter::DataImporter()
{

}

void DataImporter::readCSV(QString path)
{
   QString content;
    QFile file(path);
   if(file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       content = file.readAll();
   }
   else
   {
       QMessageBox msgBox;
       msgBox.setText("Die Datei "+path + " konnte nicht geöffnet werden!");
       msgBox.exec();
   }


}
