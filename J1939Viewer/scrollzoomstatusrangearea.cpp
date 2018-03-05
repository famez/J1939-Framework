#include <qevent.h>
#include <QScrollBar>
#include <QDebug>

#include "scrollzoomstatusrangearea.h"

ScrollZoomWidget::ScrollZoomWidget(QWidget* scrolledWidget, QWidget *parent) : QAbstractScrollArea(parent), mCurrentZoom(1), mRelX(0)
{

    setViewport(scrolledWidget);
    viewport()->setFixedWidth(width());

    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(moveViewport(int)), Qt::QueuedConnection);

}


void ScrollZoomWidget::adjustScrollBar(int oldScrollAreaWidth, int newScrollAreaWidth, int newViewportWidth) {

    int oldHValue = horizontalScrollBar()->value();

    horizontalScrollBar()->setPageStep(newViewportWidth > newScrollAreaWidth ? newScrollAreaWidth : newViewportWidth);
    horizontalScrollBar()->setRange(0, newViewportWidth > newScrollAreaWidth ? newViewportWidth - newScrollAreaWidth : newScrollAreaWidth - newViewportWidth);

    int newHValue = oldHValue * newScrollAreaWidth / oldScrollAreaWidth;

//    horizontalScrollBar()->setValue(newHValue);
    horizontalScrollBar()->setValue((int)(mRelX * (double)(newViewportWidth)));

}


void ScrollZoomWidget::setZoom(unsigned int zoom) {

    mCurrentZoom = zoom;
    viewport()->setFixedWidth(width() * zoom);
}

bool ScrollZoomWidget::viewportEvent(QEvent *event) {

    switch (event->type()) {        //Delegate to viewport Paint, MouseButton events.
    case QEvent::Paint:
    case QEvent::MouseButtonPress:
        return false;
        break;
    default:
        break;
    }

    bool retVal = QAbstractScrollArea::viewportEvent(event);

    if(event->type() == QEvent::Resize) {
        return false;       //Delegate also the event to the child if it has to do some work
    }

    return retVal;
}


//Resize event from the scrolled view
void ScrollZoomWidget::resizeEvent(QResizeEvent* event){

    int scrolledWidgetWidth = event->size().width();
    int scrollingAreaWidth = width();

    //Scroll bar is adapted to the new width of the scrolled view
    adjustScrollBar(scrollingAreaWidth, scrollingAreaWidth, scrolledWidgetWidth);      //Scroll area size has not changed
    event->accept();
}


void ScrollZoomWidget::wheelEvent(QWheelEvent *event) {

    int newZoom = mCurrentZoom;

    if(event->delta() > 0) {
        ++newZoom;
    } else if(mCurrentZoom > 1) {
        --newZoom;
    } else {
        return;
    }

    setZoom(newZoom);

    event->accept();

}



//void ScrollZoomWidget::scrollContentsBy(int dx, int dy) {

//    Q_UNUSED(dx);
//    Q_UNUSED(dy);

//    int hvalue = horizontalScrollBar()->value();

//    moveViewport(hvalue);

//}

bool ScrollZoomWidget::event(QEvent* event) {

    switch (event->type()) {
    case QEvent::Resize:        //Resize event from the Scrolling View
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);

        int newScrolledWidgetWidth = viewport()->width();
        int newScrollingAreaWidth = resizeEvent->size().width();
        int oldScrollingAreaWidth = resizeEvent->oldSize().width();


        //Adjust the width of the viewport when resizing the scroll area
        if(newScrolledWidgetWidth != newScrollingAreaWidth * mCurrentZoom) {
            viewport()->setFixedWidth(width() * mCurrentZoom);      //Resize event of viewport will adjust the scroll bar
        } else {
            adjustScrollBar(oldScrollingAreaWidth, newScrolledWidgetWidth, newScrollingAreaWidth);
        }

    }   break;
    default:
        break;
    }

    return QAbstractScrollArea::event(event);
}


void ScrollZoomWidget::moveViewport(int hOffset) {

    QPoint topLeft = viewport()->rect().topLeft();
    viewport()->move(topLeft.x() - hOffset, topLeft.y());

    qDebug() << "[ScrollZoomWidget::moveViewport] Width: " << viewport()->width() << " hOffset: " << hOffset << " x: " << viewport()->x();

    mRelX = (double)(hOffset) / (double)(viewport()->width());
}
