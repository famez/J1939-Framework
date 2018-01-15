#ifndef NUMERICSPNITEM_H
#define NUMERICSPNITEM_H

#include "spnitem.h"

namespace J1939 {
class SPNNumeric;
}


class NumericSPNItem : public SPNItem
{

private:
    J1939::SPNNumeric* mSpnNumeric;
public:
    NumericSPNItem(TreeItem *parentItem, J1939::SPNNumeric*);


    int columnCount() const final;
    QVariant data(int column) const final;
    QVariant getHeaderData(int column) const final;
    bool setData(int column, const QVariant& value) final;


};

#endif // NUMERICSPNITEM_H
