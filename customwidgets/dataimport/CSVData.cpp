#include "CSVData.h"
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileInfo>
#include <QStringList>
#include <QTextStream>
#include "ThemeColors.h"
#include "MessageQueue.h"

CSVData::CSVData(QObject *parent) : HiracData(parent)
{
    // rootIcon = QIcon(":/icons/materials/csv.png");
}
/*
void CSVData::loadData(const QString &dataPath, QStandardItemModel *model) {
    emit dataLoadStarted(); // Notify that loading has started

    QFile file(dataPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        MessageQueue::instance()->addError("Failed to open file: " + file.fileName());
        return;
    }

    QTextStream in(&file);
    QStringList headers;
    QVector<QVector<double>> columnsData;

    int rowIndex = 0;
    int totalLines = 0;

    while (!in.atEnd()) {
        in.readLine();
        totalLines++;
    }
    file.seek(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (rowIndex == 0) {
            headers = fields;
            columnsData.resize(headers.size());
        } else {
            for (int i = 0; i < fields.size(); ++i) {
                bool ok;
                double value = fields[i].toDouble(&ok);
                columnsData[i].append(ok ? value : std::numeric_limits<double>::quiet_NaN());
            }
        }

        rowIndex++;
        int progress = static_cast<int>((static_cast<double>(rowIndex) / totalLines) * 100);
        emit dataLoadProgress(progress); // Emit progress signal
    }

    // Finalize processing
    emit dataLoadProgress(100);
    emit dataLoadFinished(); // Notify that loading is complete
}
*/

void CSVData::loadData(const QString &dataPath, QStandardItemModel *model) {
    emit dataLoadProgress(1);

    QFile file(dataPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit dataLoadError("Failed to open file: " + dataPath);
        return;
    }

    QTextStream in(&file);
    QStringList headers;
    QStringList units;
    QVector<QVector<double>> columnsData;

    int rowIndex = 0;
    int totalLines = 0;

    // Count lines for progress calculation
    while (!in.atEnd()) {
        in.readLine();
        ++totalLines;
    }
    file.seek(0); // Reset stream to the beginning of the file

    columnsData.reserve(1000); // Pre-allocate space to minimize reallocation
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.isEmpty()) continue; // Skip empty rows

        if (rowIndex == 0) {
            // First row: Column headers
            headers = fields;
            columnsData.resize(headers.size());
        } else if (rowIndex == 1) {
            // Second row: Column units
            units = fields;
            if (units.size() != headers.size()) {
                emit dataLoadError("Mismatch between header and unit sizes in " + dataPath);
                return;
            }
        } else {
            // Data rows start from the third row
            for (int i = 0; i < fields.size(); ++i) {
                bool ok;
                double value = fields[i].toDouble(&ok);
                if (ok && !std::isnan(value) && !std::isinf(value)) {
                    columnsData[i].append(value); // Valid numeric value
                } else {
                    columnsData[i].append(std::numeric_limits<double>::quiet_NaN()); // Fallback to NaN
                }
            }
        }

        ++rowIndex;

        // Emit progress update
        if (rowIndex % (totalLines / 10) == 0) {
            int progress = static_cast<int>((static_cast<double>(rowIndex) / totalLines) * 100);
            emit dataLoadProgress(progress);
        }
    }

    emit dataLoadProgress(90);

    // Create the root item representing the file
    QFileInfo info(dataPath);
    QStandardItem *rootItem = new QStandardItem(info.fileName());
    rootItem->setIcon(rootIcon);
    rootItem->setToolTip(dataPath);
    rootItem->setForeground(rootColor);
    rootItem->setEditable(false);
    model->appendRow(rootItem);

    // Create child items for each column
    for (int i = 0; i < headers.size(); ++i) {
        QStandardItem *childItem = new QStandardItem(headers[i]);
        childItem->setIcon(childIcon);
        childItem->setForeground(childColor);
        childItem->setEditable(false);

        // Attach the data to the child item using Qt::UserRole
        QVariant dataVariant = QVariant::fromValue(columnsData[i]);
        childItem->setData(dataVariant, Qt::UserRole);

        // Add metadata (Min, Max, Mean, etc.)
        if (!columnsData[i].isEmpty()) {
            try {
                addStatisticalData(childItem, columnsData[i], units[i]);
            } catch (...) {
                emit dataLoadError("Error calculating statistics for column: " + headers[i]);
            }
        }

        rootItem->appendRow(childItem);
    }

    emit dataLoadProgress(100);
    emit dataLoadFinished();
}


