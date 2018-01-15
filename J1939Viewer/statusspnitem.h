#ifndef STATUSSPNITEM_H
#define STATUSSPNITEM_H

#include "spnitem.h"

/*Column numbers*/
#define SPN_STATUS_ENUMERATOR_OFFSET            (SPN_COLUMN_ITEMS) + 0
#define SPN_STATUS_BIT_OFFSET                   (SPN_COLUMN_ITEMS) + 1
#define SPN_STATUS_BIT_SIZE                     (SPN_COLUMN_ITEMS) + 2


#define HEADER_ENUM_STR                     "List of values"
#define HEADER_BIT_OFFSET_STR               "Bit Offset"
#define HEADER_BIT_SIZE_STR                 "Bit Size"

namespace J1939 {
class SPNStatus;
}



class StatusSPNItem : public SPNItem {

private:
    J1939::SPNStatus* mSpnStatus;
public:

    StatusSPNItem(TreeItem *parentItem, J1939::SPNStatus*);

    StatusSPNItem() = delete;


    int columnCount() const final;
    QVariant data(int column) const final;
    QVariant getHeaderData(int column) const final;
    bool setData(int column, const QVariant& value) final;

    TreeViewCustomEditor* getCustomEditor(int column) const final;


};

#endif // STATUSSPNITEM_H
