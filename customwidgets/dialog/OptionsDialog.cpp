#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    connect(ui->applyBtn,&QPushButton::clicked,this,&OptionsDialog::applyClicked);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

GridState OptionsDialog::getGridState()
{
    GridState grid;
    grid.x.major = ui->xMajor->isChecked();
    grid.x.minor = ui->xMinor->isChecked();
    grid.yleft.major = ui->yMajor->isChecked();
    grid.yleft.minor = ui->yMinor->isChecked();
    grid.yright.major = ui->yMajor_2->isChecked();
    grid.yright.minor = ui->yMinor_2->isChecked();

    return grid;
}

QString OptionsDialog::getTheme()
{
    return ui->themeCBox->currentText();
}

void OptionsDialog::applyClicked()
{
    hide();
    //save all setings to the settings file 
    emit applySettings();
}

QString OptionsDialog::getSelectedStyle()
{
    return ui->styleBox->currentText()+".qss";
}

void OptionsDialog::populateStyles(QStringList styles)
{
    ui->styleBox->clear();
    
    for(QString &s : styles)
    {
        ui->styleBox->addItem(s.remove(".qss"));  
    }
}