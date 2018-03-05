#ifndef STATUSRESUMEFRAME_H
#define STATUSRESUMEFRAME_H

#include <QWidget>

namespace Ui {
class StatusResumeFrame;
}

class StatusResumeFrame : public QWidget
{
    Q_OBJECT

public:
    explicit StatusResumeFrame(QWidget *parent = 0);
    ~StatusResumeFrame();

    void setStatus(const QString& status);
    void setStart(const QString& start);
    void setEnd(const QString& end);
    void setDuration(const QString& duration);


private:
    Ui::StatusResumeFrame *ui;
};

#endif // STATUSRESUMEFRAME_H
