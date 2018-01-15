#ifndef TRCINSPECTOR_H
#define TRCINSPECTOR_H

#include <QWidget>
#include <Types.h>
#include <QModelIndex>

namespace Ui {
class TRCInspector;
}


namespace J1939 {
class J1939Frame;
}

class TRCLoader;
class NumericSPNItem;

class TRCInspector : public QWidget
{
    Q_OBJECT

public:
    explicit TRCInspector(QWidget *parent = 0);
    ~TRCInspector();

    void loadFile();

public slots:
    void loaderFinished();
    void updateProgress(int progress);
    void itemClicked(QModelIndex);

private:
    Ui::TRCInspector *ui;
    TRCLoader* mLoader;

    QList< QPair< u32, J1939::J1939Frame* > > mFrameList;
    void startBackgroundLoader(const QString& file);
    void showNumericSPNViewer(NumericSPNItem*);
};

#endif // TRCINSPECTOR_H
