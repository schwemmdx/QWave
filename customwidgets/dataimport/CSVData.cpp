#include "CSVData.h"
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QFileInfo>
#include <QStringList>
#include <QTextStream>
#include "ThemeColors.h"
#include "MessageQueue.h"

CSVData::CSVData(QObject *parent) : HiracData(parent) {
    //rootIcon = QIcon(":/icons/materials/csv.png");
    
}
void CSVData::loadData(const QString &dataPath, QStandardItemModel *model) {
    qDebug() << "Loading CSV data from:" << dataPath;

    QFile file(dataPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << dataPath;
        return;
    }

    QTextStream in(&file);
    QStringList headers;
    QVector<QVector<double>> columnsData; // Store as QVector<double>

    int rowIndex = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (rowIndex == 0) {
            // Assume first row is headers
            headers = fields;
            columnsData.resize(headers.size());
        } else {
            for (int i = 0; i < fields.size(); ++i) {
                bool ok;
                double value = fields[i].toDouble(&ok);
                if (ok) {
                    columnsData[i].append(value); // Valid numeric value
                } else {
                    columnsData[i].append(std::numeric_limits<double>::quiet_NaN()); // Fallback to NaN
                }
            }
        }
        rowIndex++;
    }

    // Create the root item representing the file
    QFileInfo info(dataPath);
    QStandardItem *rootItem = new QStandardItem(info.fileName());
    rootItem->setToolTip(dataPath);
    rootItem->setForeground(rootColor);

    rootItem->setEditable(false);
    model->appendRow(rootItem);

    // Create child items for each column
    for (int i = 0; i < headers.size(); ++i) {
        QStandardItem *childItem = new QStandardItem(headers[i]);
        childItem->setForeground(childColor);
        childItem->setEditable(false);

        // Attach the data to the child item using Qt::UserRole
        QVariant dataVariant = QVariant::fromValue(columnsData[i]);
        childItem->setData(dataVariant, Qt::UserRole);

        // Add metadata (Min, Max, Mean, etc.)
        addStatisticalData(childItem, columnsData[i]);

        // Append the child to the root item
        rootItem->appendRow(childItem);

        // Debug output to verify that the data is assigned
        qDebug() << "Loaded column:" << headers[i] << "with data:" << columnsData[i];
    }

    qDebug() << "CSV data loaded successfully.";
}
