#ifndef CURSORDOCKWIDGET_H
#define CURSORDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class CursorDockWidget;
}

class CursorDockWidget : public QDockWidget
{
    Q_OBJECT
signals:
    void placeXCursors(void);
    void placeYCursors(void);
    void clearCursors(void);

public:
    explicit CursorDockWidget(QWidget *parent = nullptr);
    ~CursorDockWidget();

public slots:
    void updateCursorData(QList<void*>);
    void cursorsCleared(void);

private:
    Ui::CursorDockWidget *ui;
};

#endif // CURSORDOCKWIDGET_H
