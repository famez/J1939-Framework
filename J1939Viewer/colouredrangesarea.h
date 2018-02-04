#ifndef COLOUREDRANGESAREA_H
#define COLOUREDRANGESAREA_H

#include <QWidget>
#include <QMap>

#define DEFAULT_RECT_HEIGHT     10 //px

class StatusRangeItem {
private:

    /*
     * The end is determined by the start of the following range
     */
    unsigned int mStart;
    QColor mColor;

public:
    StatusRangeItem() {}
    StatusRangeItem(unsigned int start, const QColor& color);
    virtual ~StatusRangeItem();


    const QColor& getColor() const { return mColor; }
    void setColor(const QColor& color) { mColor = color; }

    qreal getStart() const { return mStart; }
    void setStart(qreal start) { mStart = start; }

};

//For signals and slots athough maybe it is never used in queued connections
Q_DECLARE_METATYPE(StatusRangeItem)

class StatusRangesArea : public QWidget
{
    Q_OBJECT
public:
    explicit StatusRangesArea(unsigned int end, QWidget *parent = nullptr, int rectHeight = DEFAULT_RECT_HEIGHT);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    const StatusRangeItem* addRange(unsigned int start, const QColor& color);
    void deleteRange(const StatusRangeItem* item);
    void clear();

    void setRectangleHeight(int height);

    void makeOpaqueWhenClicked(bool);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QMap<unsigned int, StatusRangeItem> mItems;
    QMap<unsigned int, StatusRangeItem>::iterator mSelectedItem;

    unsigned int mEnd;     //Necessary to know where to stop to paint for the last range


    /*
     * This attribute mangase the height the rectangles will have
     */
    int mRectHeight;



signals:

    void rangeSelected(StatusRangeItem);
    void rangeDeselected(StatusRangeItem);

public slots:
};

#endif // COLOUREDRANGESAREA_H
