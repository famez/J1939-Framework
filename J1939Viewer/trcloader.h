/*
 * This class is a thread that is in charge of loading a file in background so that does not block ui thread.
 */


#ifndef TRCLOADER_H
#define TRCLOADER_H

#include <QThread>

#include <Types.h>

namespace J1939 {
    class J1939Frame;
}

class TRCLoader : public QThread
{
    Q_OBJECT
private:
    QString mFileName;
    int mProgress;
    QList< QPair< u64, J1939::J1939Frame* > >* mFramesList;
protected:
    void run() final;
public:
    TRCLoader(const QString &file, QList< QPair< u64, J1939::J1939Frame* > >* frameList);

signals:

    void progressUpdated(int progress);
};

#endif // TRCLOADER_H
