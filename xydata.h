#ifndef XYDATA_H
#define XYDATA_H

#include <QList>
#include <QSharedPointer>


class XYData
{
public:
    XYData(QVector<qreal> x,QVector<qreal> y);
    XYData(QSharedDataPointer<QVector<qreal>>,QVector<qreal> y);

    int getLength(void);
    bool isXYLenValid(void);
    size_t getSize(void);

    void setXRadix(int radix);
    void setYRadix(int radix);

    void setX(QVector<qreal> x);
    void setY(QVector<qreal> y);

private:
    QVector<qreal> x{};
    QVector<qreal> y{};

};

#endif // XYDATA_H
