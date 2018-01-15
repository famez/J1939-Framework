#include <QDebug>

#include "spnitem.h"



#define SPN_NUMBER_COLUMN           0
#define SPN_TYPE_COLUMN             1
#define SPN_NAME_COLUMN             2
#define SPN_OFFSET_COLUMN           3



using namespace J1939;


SPNItem::SPNItem(TreeItem *parentItem, SPN *spn) : TreeItem(parentItem), mSpn(spn)
{
    setColumnEditable(SPN_NAME_COLUMN, true);
    setColumnEditable(SPN_OFFSET_COLUMN, true);
}


SPNItem::~SPNItem() {
}

std::map<J1939::SPN::EType, std::string> SPNItem::mTypeToStr = initializemap();
std::map<J1939::SPN::EType, std::string> SPNItem::initializemap() {

    std::map<J1939::SPN::EType, std::string> retVal;

    retVal[SPN::SPN_NUMERIC] = "Numeric";
    retVal[SPN::SPN_STATUS] = "Status";
    retVal[SPN::SPN_STRING] = "String";

    return retVal;

}


int SPNItem::columnCount() const {
    return SPN_COLUMN_ITEMS;
}

QVariant SPNItem::data(int column) const {

    switch(column) {
    case SPN_NUMBER_COLUMN:
        return QVariant(mSpn->getSpnNumber());
    case SPN_NAME_COLUMN:
        return QVariant(QString::fromStdString(mSpn->getName()));
    case SPN_OFFSET_COLUMN:
        return QVariant(mSpn->getOffset());
    case SPN_TYPE_COLUMN:
        return QVariant(QString::fromStdString(mTypeToStr[mSpn->getType()]));
    }

    return QVariant();
}


bool SPNItem::setData(int column, const QVariant& value) {

    switch(column) {
    case SPN_NAME_COLUMN:
    {
        if(value.canConvert<QString>()) {
            std::string name = value.value<QString>().toUtf8().constData();
            if(!name.empty()) {
                mSpn->setName(name);
                return true;
            }
        }
    }   break;
    case SPN_OFFSET_COLUMN:
    {
        if(value.canConvert<u32>()) {
            u32 offset = value.value<u32>();
            mSpn->setOffset(offset);
            return true;
        }
    }   break;

    }
    return false;

}


QVariant SPNItem::getHeaderData(int column) const {

    switch(column) {
    case SPN_NAME_COLUMN:
        return "Name";
    case SPN_NUMBER_COLUMN:
        return "Number";
    case SPN_OFFSET_COLUMN:
        return "Offset";
    case SPN_TYPE_COLUMN:
        return "Type";
    }

    return QVariant();
}

