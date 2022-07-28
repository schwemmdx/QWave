#include "dataimporter.h"
#include "QFile"

#include "QDebug"
#include "QMessageBox"

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
