#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "trcinspector.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void open();
    void save();
    void openTRCInspector();

private:
    Ui::MainWindow *ui;
    QString mCurrentPath;
    QAction* mSaveAction;
    TRCInspector mInspector;

};

#endif // MAINWINDOW_H
