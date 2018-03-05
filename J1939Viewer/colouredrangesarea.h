#ifndef COLOUREDRANGESAREA_H
#define COLOUREDRANGESAREA_H

//J1939 includes
#include <Types.h>

//QT includes
#include <QWidget>
#include <QMap>

#define DEFAULT_RECT_HEIGHT     10 //px

class StatusRangeItem {
private:

    /*
     * The end is determined by the start of the following range
     */
    u64 mStart;
    u8 mStatus;
    QColor mColor;

public:
    StatusRangeItem() {}
    StatusRangeItem(unsigned int start, u8 status, const QColor& color);
    virtual ~StatusRangeItem();


    const QColor& getColor() const { return mColor; }
    void setColor(const QColor& color) { mColor = color; }

    u64 getStart() const { return mStart; }
    void setStart(u64 start) { mStart = start; }

    u8 getStatus() const { return mStatus; }
    void setStatus(u8 status) { mStatus = status; }

    bool operator ==(const StatusRangeItem& other) {
        return ((mColor == other.mColor) && (mStatus == other.mStatus) && (mStart == other.mStart));
    }


};

//For signals and slots athough maybe it is never used in queued connections
Q_DECLARE_METATYPE(StatusRangeItem)

class StatusRangesArea : public QWidget
{
    Q_OBJECT
public:
    explicit StatusRangesArea(u64 end, QWidget *parent = nullptr, int rectHeight = DEFAULT_RECT_HEIGHT);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    /*
     * Returns true if added, false otherwise
     */
    bool addStatusRange(u64 start, u8 status, const QColor& color);
    void deleteRange(const StatusRangeItem* item);


    /*
     * Checks if the range is valid
     */
    bool isValidRange(StatusRangeItem);

    /*
     * It tells us if this is the first range or otherwise there are more ranges from the beggining
     */
    bool isFirstRange(StatusRangeItem);

    /*
     * It tells us if there are more ranges until the end or otherwise this is the last range
     */
    bool isLastRange(StatusRangeItem);

    /*
     * Returns the following range concerning the passed StatusRangeItem as argument
     *
     */
    StatusRangeItem getFollowingRange(StatusRangeItem);

    /*
     * Returns the previous range concerning the passed StatusRangeItem as argument
     *
     */
    StatusRangeItem getPreviousRange(StatusRangeItem);

    void clear();

    void setRectangleHeight(int height);

    void makeOpaqueWhenClicked(bool);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


private:
    QMap<u64, StatusRangeItem> mItems;
    QMap<u64, StatusRangeItem>::iterator mSelectedItem;

    u64 mEnd;     //Necessary to know where to stop to paint for the last range


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
