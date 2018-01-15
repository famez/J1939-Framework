#ifndef FRAMEITEM_H
#define FRAMEITEM_H


#include "treeitem.h"
#include "spnitem.h"

#define FRAME_TREE_ITEM         "frame_tree_item"



#define J1939FRAME_FIELDS         5        //At the moment only PGN Column and Name Column

#define FRAME_PGN_COLUMN          0
#define FRAME_NAME_COLUMN         1
#define FRAME_SRC_ADDR_COLUMN     2
#define FRAME_DST_ADDR_COLUMN     3
#define FRAME_PRIORITY_COLUMN     4

namespace J1939 {
class J1939Frame;
}


class FrameItem : public TreeItem
{
protected:
    J1939::J1939Frame* mFrame;

public:
    FrameItem(TreeItem *parentItem, const J1939::J1939Frame*);
    virtual ~FrameItem();
    int columnCount() const;
    QVariant data(int column) const;
    virtual bool setData(int column, const QVariant& value);

    const J1939::J1939Frame* getFrame() const { return mFrame; }


    QVariant getHeaderData(int column) const;

    virtual QString getItemType() const { return FRAME_TREE_ITEM; }


};

#endif // FRAMEITEM_H
