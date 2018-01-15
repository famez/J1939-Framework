#ifndef COLOUREDRANGESAREA_H
#define COLOUREDRANGESAREA_H

#include <QWidget>
#include <QMap>

class ColouredRangeItem {
private:

    /*
     * The end is determined by the start of the following range
     */
    unsigned int mStart;
    QColor mColor;
public:
    ColouredRangeItem() {}
    ColouredRangeItem(unsigned int start, const QColor& color);
    virtual ~ColouredRangeItem();


    const QColor& getColor() const { return mColor; }
    void setColor(const QColor& color) { mColor = color; }

    qreal getStart() const { return mStart; }
    void setStart(qreal start) { mStart = start; }

};

class ColouredRangesArea : public QWidget
{
    Q_OBJECT
public:
    explicit ColouredRangesArea(unsigned int end, QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    const ColouredRangeItem* addRange(unsigned int start, const QColor& color);
    void deleteRange(const ColouredRangeItem* item);
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QMap<unsigned int, ColouredRangeItem> mItems;
    unsigned int mEnd;     //Necessary to know where to stop to paint for the last range


signals:

public slots:
};

#endif // COLOUREDRANGESAREA_H
