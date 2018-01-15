/*
 * Class to load SPNs in a QChart in another thread other than the ui thread in order to not block the GUI.
 */

#ifndef SPNLOADER_H
#define SPNLOADER_H

#include <QThread>

#include <J1939Frame.h>

#include "spnchart.h"


class SPNLoader : public QThread
{
     Q_OBJECT
private:
    typedef QList< QPair< u32, J1939::J1939Frame* > >  FrameList;
    SPNViewer* mChart;
    const FrameList* mFrameList;

signals:
    void pointsLoaded(QList<QPointF> points);

protected:
    void run() final;
public:
    SPNLoader(SPNViewer* viewer, const FrameList* frameList);
    virtual ~SPNLoader();

};

#endif // SPNLOADER_H
