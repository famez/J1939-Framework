#include <QPainter>

#include "colouredrangesarea.h"



ColouredRangeItem::ColouredRangeItem(unsigned int start, const QColor& color) : mStart(start),
    mColor(color){



}

ColouredRangeItem::~ColouredRangeItem() {

}



ColouredRangesArea::ColouredRangesArea(unsigned int end, QWidget *parent) : QWidget(parent), mEnd(end)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void ColouredRangesArea::paintEvent(QPaintEvent */*event*/) {
    QPainter painter(this);
    int width = size().width();
    int height = size().height();

    if(mItems.empty())
        return;

    unsigned int start = mItems.begin()->getStart();
    unsigned int lastStart = start;
    unsigned int areaWidth = mEnd - start;

    QBrush brush(mItems.begin()->getColor());
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(mItems.begin()->getColor());

    if(mItems.size() > 1) {
        for(auto iter = mItems.begin() + 1; iter != mItems.end(); ++iter) {
            int rectWidth = ((unsigned long long)(iter->getStart() - lastStart)) * width / areaWidth;
            int rectStart = ((unsigned long long)(lastStart - start)) * width / areaWidth;

            QRect rect(rectStart, 0, rectWidth, height);
            painter.drawRect(rect);
            lastStart = iter->getStart();
            painter.setPen(iter->getColor());
            brush.setColor(iter->getColor());
            painter.setBrush(brush);

        }
    }

    QRect rect(lastStart, 0, mEnd - lastStart, height);

    painter.drawRect(rect);

}

QSize ColouredRangesArea::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize ColouredRangesArea::sizeHint() const
{
    return QSize(100, 100);
}


const ColouredRangeItem* ColouredRangesArea::addRange(unsigned int start, const QColor& color) {

    auto iter = mItems.find(start);

    if(iter != mItems.end()) {
        return NULL;
    }

    mItems[start] = ColouredRangeItem(start, color);

    return &(mItems[start]);

}

void ColouredRangesArea::deleteRange(const ColouredRangeItem *item) {

    for(auto iter = mItems.begin(); iter != mItems.end(); ++iter) {
        if(item == &(*iter)) {
            mItems.erase(iter);
            return;
        }
    }
}

void ColouredRangesArea::clear() {
    mItems.clear();
}
