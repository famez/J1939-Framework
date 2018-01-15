#include "framedialog.h"
#include "ui_framedialog.h"

FrameDialog::FrameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrameDialog)
{
    ui->setupUi(this);
}

FrameDialog::~FrameDialog()
{
    delete ui;
}

int FrameDialog::getPGN() const {

    return ui->spinBox->value();

}
