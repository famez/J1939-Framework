#ifndef SPNSTATUSEDITION_H
#define SPNSTATUSEDITION_H

#include <QWidget>
#include <QMap>

#include <Types.h>

#include <SPN/SPNStatus.h>


class QLabel;
class QVBoxLayout;
class StatusResumeFrame;
class ScrollZoomWidget;
class KeyValueLabel;


#include "colouredrangesarea.h"

class SpnStatusEdition : public QWidget
{
    Q_OBJECT
private:
    StatusRangesArea* mStatusArea;
    QMap<u64, QLabel*> mTimeStampLabels;
    QLabel* mTimeStampEndLabel;
    /*
     * We hold the status SPN to obtain the descriptions about the different statuses.
     */
    J1939::SPNStatus mSPN;
    u64 mEnd;

    /*
     * Only used to hold the labels
     */
    QWidget* mAuxWidget;

    /*
     * To apply scroll
     */
    ScrollZoomWidget* mScrollArea;

    /*
     * To show the resume of the status
     */
    StatusResumeFrame* mResume;

    QMap<unsigned int, Qt::GlobalColor> mColors;

    KeyValueLabel* mStatusLabel;
    KeyValueLabel* mStartLabel;
    KeyValueLabel* mEndLabel;
    KeyValueLabel* mDurationLabel;

    void updateLabels();

private slots:
    void onRangePressed(StatusRangeItem);
    void onRangeReleased(StatusRangeItem);

protected:
//    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    bool eventFilter(QObject *target, QEvent *event) Q_DECL_OVERRIDE;

public:
    SpnStatusEdition(J1939::SPNStatus, u64 end);
    void addStatusRange(u64 start, u8 status);

};

#endif // SPNSTATUSEDITION_H
