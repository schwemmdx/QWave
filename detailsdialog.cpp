#include "detailsdialog.h"
#include "ui_detailsdialog.h"

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
