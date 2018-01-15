#ifndef SPNITEM_H
#define SPNITEM_H

#include <map>
#include <SPN/SPN.h>
#include "treeitem.h"

#define SPN_COLUMN_ITEMS            4



#define SPN_TREE_ITEM           "spn_tree_item"

class SPNItem : public TreeItem
{
private:
    static std::map<J1939::SPN::EType, std::string> mTypeToStr;
    static std::map<J1939::SPN::EType, std::string> initializemap();
    J1939::SPN* mSpn;

public:
    SPNItem(TreeItem *parentItem, J1939::SPN*);
    virtual ~SPNItem();
    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual QVariant getHeaderData(int column) const;
    virtual bool setData(int column, const QVariant& value);
    QString getItemType() const final { return SPN_TREE_ITEM; }


    u32 getSPNNumber() const { return mSpn->getSpnNumber(); }

    const J1939::SPN* getSPN() const { return mSpn; }

};

#endif // SPNITEM_H
