#include <QPainter>
#include <QDebug>
#include <qevent.h>

#include "colouredrangesarea.h"

#define ALPHA_WEIGHT            80


StatusRangeItem::StatusRangeItem(unsigned int start, u8 status, const QColor& color) : mStart(start), mStatus(status),
    mColor(color) {



}

StatusRangeItem::~StatusRangeItem() {

}



StatusRangesArea::StatusRangesArea(u64 end, QWidget *parent, int rectHeight) : QWidget(parent), mEnd(end), mRectHeight(rectHeight)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    mSelectedItem = mItems.end();



}

void StatusRangesArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QColor transColor;          //To paint the content of the rectangles with an alpha

    int rectStart, rectWidth;

    int width = this->width();

    Q_UNUSED(event);
//    int visibleWidth = event->rect().width();

    int height = mRectHeight;

    if(mItems.empty())
        return;

    u64 start = mItems.begin()->getStart();
    u64 lastStart = start;
    u64 areaWidth = mEnd - start;

    transColor = mItems.begin()->getColor();

    //If the item to paint is the current selected one, we draw it opaque
    if(mItems.begin() != mSelectedItem) {
        transColor.setAlpha(ALPHA_WEIGHT);
    }


    QBrush brush(transColor);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(mItems.begin()->getColor());

    if(mItems.size() > 1) {
        for(auto iter = mItems.begin() + 1; iter != mItems.end(); ++iter) {
            rectWidth = ((u64)(iter->getStart() - lastStart)) * width / areaWidth;
            rectStart = ((u64)(lastStart - start)) * width / areaWidth;


            QRect rect(rectStart, 0, rectWidth ? rectWidth : 1, height);

            painter.drawRect(rect);


            lastStart = iter->getStart();
            painter.setPen(iter->getColor());
            transColor = iter->getColor();

            //If the item to paint is the current selected one, we draw it opaque
            if(iter != mSelectedItem) {
                transColor.setAlpha(ALPHA_WEIGHT);
            }

            brush.setColor(transColor);
            painter.setBrush(brush);

        }
    }

    rectWidth = ((u64)(mEnd - lastStart)) * width / areaWidth;
    rectStart = ((u64)(lastStart - start)) * width / areaWidth;

    QRect rect(rectStart, 0, rectWidth ? rectWidth : 1, height);

    painter.drawRect(rect);

}

void StatusRangesArea::keyPressEvent(QKeyEvent *event) {

    bool update = false;
    if(mSelectedItem == mItems.end()) {
        event->ignore();
        return;
    }

    if(event->matches(QKeySequence::MoveToPreviousChar) && !isFirstRange(*mSelectedItem)) {

        --mSelectedItem;
        update = true;
    }

    if(event->matches(QKeySequence::MoveToNextChar) && !isLastRange(*mSelectedItem)) {
        ++mSelectedItem;
        update = true;
    }

    if(update) {
        emit rangeSelected(*mSelectedItem);
        this->update();
        event->accept();
    }

}

void StatusRangesArea::mousePressEvent(QMouseEvent *event) {


    if(event->button() != Qt::MouseButton::LeftButton || event->x() < 0) {
        return;
    }

    if(mItems.empty()) {
        mSelectedItem = mItems.end();
        return;
    }

    //TODO check height


    int width = size().width();
    u64 start = mItems.begin()->getStart();
    u64 areaWidth = mEnd - start;
    int count = 0;

    u64  rangePosition = ((u64)(event->x())) * areaWidth / width + start;

    auto iter = mItems.begin();

    for(iter = mItems.begin(); iter != mItems.end(); ++iter) {

        if(rangePosition < iter->getStart()) {
            qDebug() << "[ColouredRangesArea::mousePressEvent] Item " << count << "pressed";

            break;
        }

        count++;
    }

    if(mSelectedItem != mItems.end()) {
        emit rangeDeselected(*mSelectedItem);
    }


    mSelectedItem = iter - 1;

    emit rangeSelected(*mSelectedItem);
    update();

}

QSize StatusRangesArea::minimumSizeHint() const
{
    return QSize(50, mRectHeight);
}

QSize StatusRangesArea::sizeHint() const
{
    return QSize(100, mRectHeight);
}

bool StatusRangesArea::addStatusRange(u64 start, u8 status, const QColor& color) {

    auto iter = mItems.find(start);

    if(iter != mItems.end()) {
        return false;
    }

    mItems[start] = StatusRangeItem(start, status, color);

    return true;

}

void StatusRangesArea::deleteRange(const StatusRangeItem *item) {

    for(auto iter = mItems.begin(); iter != mItems.end(); ++iter) {
        if(item == &(*iter)) {
            mItems.erase(iter);
            return;
        }
    }
}

void StatusRangesArea::clear() {
    mItems.clear();
    mSelectedItem = mItems.end();
}

bool StatusRangesArea::isValidRange(StatusRangeItem item) {

    auto iter = mItems.find(item.getStart());

    return (iter != mItems.end() && iter.value() == item);

}

bool StatusRangesArea::isFirstRange(StatusRangeItem item) {

    return (mItems.find(item.getStart()) == mItems.begin());
}

bool StatusRangesArea::isLastRange(StatusRangeItem item) {

    auto iter = mItems.find(item.getStart());

    return ((iter != mItems.end()) && (iter + 1 == mItems.end()));

}

StatusRangeItem StatusRangesArea::getFollowingRange(StatusRangeItem item) {

    StatusRangeItem retVal = item;

    if(isValidRange(item) && !isLastRange(item)) {

        retVal = *(mItems.find(item.getStart()) + 1);

    }

    return retVal;

}


StatusRangeItem StatusRangesArea::getPreviousRange(StatusRangeItem item) {

    StatusRangeItem retVal = item;

    if(isValidRange(item) && !isFirstRange(item)) {

        retVal = *(mItems.find(item.getStart()) - 1);

    }

    return retVal;


}
