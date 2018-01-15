#include "spnstatusdialog.h"
#include "ui_spnstatusdialog.h"

SpnStatusDialog::SpnStatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpnStatusDialog)
{
    ui->setupUi(this);
}

SpnStatusDialog::~SpnStatusDialog()
{
    delete ui;
}

void SpnStatusDialog::setNumber(u8 number) {

    ui->numberSpinBox->setValue(number);

}

u8 SpnStatusDialog::getNumber() const {

    return ui->numberSpinBox->value();
}

void SpnStatusDialog::setDescription(const QString &desc) {

    ui->descLineEdit->setText(desc);

}

QString SpnStatusDialog::getDescription() const {

    return ui->descLineEdit->text();
}
