#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <qevent.h>

#include <utils.h>

#include "spnstatusedition.h"
#include "statusdescriptor.h"
#include "flowlayout.h"
#include "statusresumeframe.h"
#include "scrollzoomstatusrangearea.h"
#include "keyvaluelabel.h"

using namespace J1939;

SpnStatusEdition::SpnStatusEdition(SPNStatus spn, u64 end) : mSPN(spn), mEnd(end)
{

    FlowLayout* flowLayout = new FlowLayout(5, 5, 5);

    FlowLayout* flowLayout1 = new FlowLayout(5, 5, 5);


    mStatusLabel = new KeyValueLabel("Status", this);
    mStartLabel = new KeyValueLabel("Start", this);
    mEndLabel = new KeyValueLabel("End", this);
    mDurationLabel = new KeyValueLabel("Duration", this);

    flowLayout1->addWidget(mStatusLabel);
    flowLayout1->addWidget(mStartLabel);
    flowLayout1->addWidget(mEndLabel);
    flowLayout1->addWidget(mDurationLabel);



    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;

    QHBoxLayout *hScrolledLayout1 = new QHBoxLayout;
    QHBoxLayout *hScrolledLayout2 = new QHBoxLayout;
    QVBoxLayout *vScrolledLayout = new QVBoxLayout;

    QWidget* auxWidget = new QWidget;
    mScrollArea = new ScrollZoomWidget(auxWidget, this);
    mAuxWidget = new QWidget(auxWidget);

    mStatusArea = new StatusRangesArea(end, auxWidget);

//    resize(size().width(), 60);

    mColors[0] = Qt::GlobalColor::blue;
    mColors[1] = Qt::GlobalColor::green;
    mColors[2] = Qt::GlobalColor::red;
    mColors[3] = Qt::GlobalColor::yellow;

    setLayout(vLayout);

    vScrolledLayout->addLayout(hScrolledLayout1, 1);
    vScrolledLayout->addLayout(hScrolledLayout2, 0);
//    vScrolledLayout->addStretch(1);

    hLayout1->addWidget(mScrollArea, 1, Qt::AlignBottom);

    mScrollArea->setFixedHeight(55);

//    hLayout1->setSizeConstraint(QLayout::SetMaximumSize);

    vLayout->addLayout(flowLayout);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(flowLayout1);


    auxWidget->installEventFilter(this);



    mResume = new StatusResumeFrame(this);



    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0, 0, 0, 0);
    hLayout1->setSpacing(0);
    hLayout1->setContentsMargins(0, 0, 0, 0);
    hLayout2->setSpacing(0);
    hLayout2->setContentsMargins(0, 0, 0, 0);

    vScrolledLayout->setSpacing(0);
    vScrolledLayout->setContentsMargins(0, 0, 0, 0);
    hScrolledLayout1->setSpacing(0);
    hScrolledLayout1->setContentsMargins(0, 0, 0, 0);
    hScrolledLayout2->setSpacing(0);
    hScrolledLayout2->setContentsMargins(0, 0, 0, 0);
    auxWidget->setLayout(vScrolledLayout);
//    vScrolledLayout->addLayout(hScrolledLayout1);
//    vScrolledLayout->addLayout(hScrolledLayout2);


    mStatusArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    mAuxWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
    auxWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
    mScrollArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);


    //It is necessary to make the widget dependant of J1939 library only for this?
    SPNStatus::DescMap descriptions = spn.getValueDescriptionsMap();

    for(auto iter = descriptions.begin(); iter != descriptions.end(); ++iter) {
        StatusDescriptor* desc = new StatusDescriptor(QString::fromUtf8(iter->second.c_str()), mColors[iter->first % 4], this);
        flowLayout->addWidget(desc);
    }

    hScrolledLayout1->addWidget(mAuxWidget);
    hScrolledLayout2->addWidget(mStatusArea);
    vLayout->addWidget(mResume, 1, Qt::AlignBottom);


//    u32 hours, min, sec, millis, micros = mEnd;

//    Utils::fromMicrosToHoursMinSecMillisAndMicros(hours, min, sec, millis, micros);

    Utils::Time time(mEnd);

//    QString time = Utils::formatTime(hours, min, sec, millis, micros);

    //Add label corresponding to the timestamp of the end
    mTimeStampEndLabel = new QLabel(time.toString(), mAuxWidget);


    /*setStyleSheet(QLatin1String("StatusResumeFrame { \n"
    "	border: 2px solid grey;\n"
    "	box-shadow:20px 20px 10px grey;\n"
    "	border-radius: 5px; \n"
    "	background-color:white\n"
    "}"));*/


    //Manage range selection to provide extra information when clicked
    connect(mStatusArea, SIGNAL(rangeSelected(StatusRangeItem)), this, SLOT(onRangePressed(StatusRangeItem)));
    connect(mStatusArea, SIGNAL(rangeDeselected(StatusRangeItem)), this, SLOT(onRangeReleased(StatusRangeItem)));

}

void SpnStatusEdition::addStatusRange(u64 start, u8 status) {

    if(!mStatusArea->addStatusRange(start, status, mColors[status % 4])) {
        return;         //If already added, do nothing
    }

//    u32 hours, min, sec, millis, micros = item.getStart();

//    Utils::fromMicrosToHoursMinSecMillisAndMicros(hours, min, sec, millis, micros);

    Utils::Time time(start);

//    QString time = Utils::formatTime(hours, min, sec, millis, micros);

    QLabel* label = new QLabel(time.toString(), mAuxWidget);

    mTimeStampLabels[start] = label;

}


bool SpnStatusEdition::eventFilter(QObject *target, QEvent *event) {

    Q_UNUSED(target);

    QWidget* auxWidget = static_cast<QWidget*>(target);

    if(event->type() != QEvent::Resize) {
        return false;
    }

    QResizeEvent* resEvent = static_cast<QResizeEvent*>(event);

    if(resEvent->size().width() != resEvent->oldSize().width()) {
        //Here, we adjust the position of all the labels
        updateLabels();

    }
    qDebug() << "[SpnStatusEdition::eventFilter] auxWidget sizeHint:" << auxWidget->sizeHint() << "size" << auxWidget->size() << "minimum size hint" << auxWidget->minimumSizeHint() <<
                "minimum size" << auxWidget->minimumSize();

    qDebug() << "[SpnStatusEdition::eventFilter] vScrolledLayout sizeHint: " << auxWidget->layout()->minimumSize();

    return false;
}

/*void SpnStatusEdition::resizeEvent(QResizeEvent *event) {

    Q_UNUSED(event);

    //Here, we adjust the position of all the labels
    updateLabels();

}*/


void SpnStatusEdition::updateLabels() {

    if(mTimeStampLabels.empty()) {
        return;
    }

    u64 start = mTimeStampLabels.begin().key();
    u64 range = mEnd - start;

    int currentLabelPos = 0;
    int lastLabelRightPos = mTimeStampLabels.begin().value()->width();

    for(auto label = mTimeStampLabels.begin() + 1; label != mTimeStampLabels.end(); ++label) {
        currentLabelPos = (u64)(label.key() - start) * mStatusArea->width() / range - label.value()->width() / 2;

        label.value()->setVisible(currentLabelPos > lastLabelRightPos);

        if(currentLabelPos > lastLabelRightPos) {
            label.value()->move(currentLabelPos, 0);
            lastLabelRightPos = currentLabelPos + label.value()->width();
        }

    }

    currentLabelPos = mStatusArea->width() - mTimeStampEndLabel->width();

    mTimeStampEndLabel->setVisible(currentLabelPos > lastLabelRightPos);

    mTimeStampEndLabel->move(currentLabelPos, 0);

}


void SpnStatusEdition::onRangePressed(StatusRangeItem item) {

    qDebug() << "[SpnStatusEdition::onRangePressed] Range pressed";

    if(!mStatusArea->isValidRange(item)) {
        return;
    }


    u64 end;

    QString status;

    //It is necessary to make the widget dependant of J1939 library only for this?
    SPNStatus::DescMap descriptions = mSPN.getValueDescriptionsMap();

    auto iter = descriptions.find(item.getStatus());

    if(iter != descriptions.end()) {
        status = QString::fromStdString(iter->second);
    }

    mStatusLabel->setValue(status);

    Utils::Time time(item.getStart());

//    u32 hours, min, sec, millis, micros = item.getStart();

//    Utils::fromMicrosToHoursMinSecMillisAndMicros(hours, min, sec, millis, micros);

//    time = Utils::formatTime(hours, min, sec, millis, micros);



    mStartLabel->setValue(time.toString());

    if(mStatusArea->isLastRange(item)) {
        end = mEnd;
    } else {
        end = mStatusArea->getFollowingRange(item).getStart();
    }



//    millis = end;

//    Utils::fromMicrosToHoursMinSecMillisAndMicros(hours, min, sec, millis, micros);

//    time = Utils::formatTime(hours, min, sec, millis, micros);

    time.setTime(end);

    mEndLabel->setValue(time.toString());

//    millis = end - item.getStart();

//    Utils::fromMicrosToHoursMinSecMillisAndMicros(hours, min, sec, millis, micros);

//    time = Utils::formatTime(hours, min, sec, millis, micros);

    time.setTime(end - item.getStart());

    mDurationLabel->setValue(time.toString());


}

void SpnStatusEdition::onRangeReleased(StatusRangeItem item) {
    qDebug() << "[SpnStatusEdition::onRangeReleased] Range pressed";
}
