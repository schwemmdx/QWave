#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

struct AxisGridState{
    bool major;
    bool minor;
};

struct GridState{
    AxisGridState x;
    AxisGridState yleft;
    AxisGridState yright;
};

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();

    QString getTheme();
    GridState getGridState();
    QString getSelectedStyle();

signals:
    void applySettings();

public slots:
    void populateStyles(QStringList styles);

private:
    Ui::OptionsDialog *ui;

private slots:
    void applyClicked();

};

#endif // OPTIONSDIALOG_H
