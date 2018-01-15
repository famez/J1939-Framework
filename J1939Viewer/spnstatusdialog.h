#ifndef SPNSTATUSDIALOG_H
#define SPNSTATUSDIALOG_H

#include <QDialog>
#include <Types.h>

namespace Ui {
class SpnStatusDialog;
}

class SpnStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpnStatusDialog(QWidget *parent = 0);
    ~SpnStatusDialog();


    void setNumber(u8 number);
    u8 getNumber() const;

    void setDescription(const QString& desc);
    QString getDescription() const;

private:
    Ui::SpnStatusDialog *ui;
};

#endif // SPNSTATUSDIALOG_H
