#include "statusresumeframe.h"
#include "ui_statusresumeframe.h"

StatusResumeFrame::StatusResumeFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusResumeFrame)
{
    ui->setupUi(this);
}

StatusResumeFrame::~StatusResumeFrame()
{
    delete ui;
}


void StatusResumeFrame::setStatus(const QString& status) {
    ui->status_value->setText(status);
}

void StatusResumeFrame::setStart(const QString& start) {
    ui->start_value->setText(start);
}

void StatusResumeFrame::setEnd(const QString& end) {
    ui->end_value->setText(end);
}

void StatusResumeFrame::setDuration(const QString& duration) {
    ui->duration_value->setText(duration);
}
