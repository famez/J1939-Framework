#include "spndialog.h"
#include "ui_spndialog.h"

SpnDialog::SpnDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpnDialog)
{
    ui->setupUi(this);
}

SpnDialog::~SpnDialog()
{
    delete ui;
}


int SpnDialog::getSPNNumber() {
    return ui->spinBox->value();
}
QString SpnDialog::getSPNType() {
    return ui->comboBox->currentText();
}
