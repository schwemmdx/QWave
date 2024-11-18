#include "DetailsDialog.h"
#include "ui_DetailsDialog.h"

#include <QButtonGroup>

DetailsDialog::DetailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailsDialog)
{
    ui->setupUi(this);
    
}

DetailsDialog::~DetailsDialog()
{
    delete ui;
}
