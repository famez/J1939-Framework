#ifndef GENERICFRAMEITEM_H
#define GENERICFRAMEITEM_H

#include "frameitem.h"


#include <SPN/SPN.h>



#define GENERICFRAME_TREE_ITEM      "genericframe_tree_item"


namespace J1939 {
class GenericFrame;
}

class GenericFrameItem : public FrameItem
{
private:
    J1939::GenericFrame* mGenFrame;
    void parseSPNs(J1939::GenericFrame*);
public:
    GenericFrameItem(TreeItem *parentItem, const J1939::GenericFrame* frame);

    void addSPN(J1939::SPN::EType type, u32 number);

    bool hasSPN(u32 number) const;

    void deleteSPN(SPNItem *item);

    bool setData(int column, const QVariant& value) final;

    QString getItemType() const final { return GENERICFRAME_TREE_ITEM; }

    const J1939::GenericFrame* getGenericFrame() const { return mGenFrame; }
};

#endif // GENERICFRAMEITEM_H
