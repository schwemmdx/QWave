#ifndef CSVDATA_H
#define CSVDATA_H

#include "HiracData.h"

class CSVData : public HiracData {
public:
    explicit CSVData(QObject *parent = nullptr);
    ~CSVData() override {}

protected:
    void loadData(const QString &dataPath, QStandardItemModel *model) override;
};

#endif // CSVDATA_H
