#ifndef FRAMEDIALOG_H
#define FRAMEDIALOG_H

#include <QDialog>

namespace Ui {
class FrameDialog;
}

class FrameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FrameDialog(QWidget *parent = 0);
    ~FrameDialog();

    int getPGN() const;

private:
    Ui::FrameDialog *ui;
};

#endif // FRAMEDIALOG_H
