/*
 * This class lets StatusRangesArea to be zoomed and scrolled to be able to see in a clearer way the
 * change from a status to another
 */


#ifndef SCROLLZOOMSTATUSRANGEAREA_H
#define SCROLLZOOMSTATUSRANGEAREA_H

#include <QAbstractScrollArea>


class ScrollZoomWidget : public QAbstractScrollArea
{
    Q_OBJECT
private:
    int mCurrentZoom;

    /*
     * Relative shifting applied to the viewport over the scrolling area
     */
    double mRelX;
    void adjustScrollBar(int oldScrollAreaWidth, int newScrollAreaWidth, int newViewportWidth);

public slots:
    void moveViewport(int hOffset);

protected:
    bool viewportEvent(QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
//    void scrollContentsBy(int dx, int dy) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

public:
    ScrollZoomWidget(QWidget* scrolledWidget, QWidget *parent = nullptr);


public slots:
    void setZoom(unsigned int zoom);

};

#endif // SCROLLZOOMSTATUSRANGEAREA_H
