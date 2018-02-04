#include <QPainter>
#include <QDebug>
#include <qevent.h>

#include "colouredrangesarea.h"

#define ALPHA_WEIGHT            80


StatusRangeItem::StatusRangeItem(unsigned int start, const QColor& color) : mStart(start),
    mColor(color) {



}

StatusRangeItem::~StatusRangeItem() {

}



StatusRangesArea::StatusRangesArea(unsigned int end, QWidget *parent, int rectHeight) : QWidget(parent), mEnd(end), mRectHeight(rectHeight)
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

//    int visibleWidth = event->rect().width();

    int height = mRectHeight;

    if(mItems.empty())
        return;

    unsigned int start = mItems.begin()->getStart();
    unsigned int lastStart = start;
    unsigned int areaWidth = mEnd - start;

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
            rectWidth = ((unsigned long long)(iter->getStart() - lastStart)) * width / areaWidth;
            rectStart = ((unsigned long long)(lastStart - start)) * width / areaWidth;


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

    rectWidth = ((unsigned long long)(mEnd - lastStart)) * width / areaWidth;
    rectStart = ((unsigned long long)(lastStart - start)) * width / areaWidth;

    QRect rect(rectStart, 0, rectWidth ? rectWidth : 1, height);

    painter.drawRect(rect);

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
    unsigned int start = mItems.begin()->getStart();
    unsigned int areaWidth = mEnd - start;
    int count = 0;

    unsigned int rangePosition = ((unsigned long long)(event->x())) * areaWidth / width + start;

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

const StatusRangeItem* StatusRangesArea::addRange(unsigned int start, const QColor& color) {

    auto iter = mItems.find(start);

    if(iter != mItems.end()) {
        return NULL;
    }

    mItems[start] = StatusRangeItem(start, color);

    return &(mItems[start]);

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
}
