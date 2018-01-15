#ifndef SPNDIALOG_H
#define SPNDIALOG_H

#include <QDialog>

namespace Ui {
class SpnDialog;
}

class SpnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpnDialog(QWidget *parent = 0);
    ~SpnDialog();

    int getSPNNumber();
    QString getSPNType();

private:
    Ui::SpnDialog *ui;

};

#endif // SPNDIALOG_H
