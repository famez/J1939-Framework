#include <QDebug>

#include <SPN/SPNStatus.h>
#include "statusspnitem.h"
#include "spnstatuseditor.h"



#define STATUS_SPN_COLUMN_ITEMS    3




using namespace J1939;

StatusSPNItem::StatusSPNItem(TreeItem *parentItem, SPNStatus* spn) : SPNItem(parentItem, spn) , mSpnStatus(spn){

    setColumnEditable(SPN_STATUS_ENUMERATOR_OFFSET, true);
    setColumnEditable(SPN_STATUS_BIT_OFFSET, true);
    setColumnEditable(SPN_STATUS_BIT_SIZE, true);

    setColumnPersistentEdition(SPN_STATUS_ENUMERATOR_OFFSET, true);

    setDelegateType(SPN_STATUS_ENUMERATOR_OFFSET, DEL_TYPE_COMBOBOX);

}


int StatusSPNItem::columnCount() const {
    return STATUS_SPN_COLUMN_ITEMS + SPNItem::columnCount();
}


QVariant StatusSPNItem::data(int column) const {

    switch(column) {
    case SPN_STATUS_ENUMERATOR_OFFSET:
    {
        StatusMap statuses;
        std::map<u8, std::string> descriptions = mSpnStatus->getValueDescriptionsMap();

        for(auto iter = descriptions.begin(); iter != descriptions.end(); ++iter) {
            statuses[iter->first] = QString::fromStdString(iter->second);
        }

        return QVariant::fromValue(statuses);
    }

    case SPN_STATUS_BIT_OFFSET:
        return QVariant(mSpnStatus->getBitOffset());
    case SPN_STATUS_BIT_SIZE:
        return QVariant(mSpnStatus->getBitSize());
    default:
        break;
    }

    return SPNItem::data(column);

}

QVariant StatusSPNItem::getHeaderData(int column) const {

    switch(column) {
    case SPN_STATUS_ENUMERATOR_OFFSET:
        return QVariant(HEADER_ENUM_STR);
    case SPN_STATUS_BIT_OFFSET:
        return QVariant(HEADER_BIT_OFFSET_STR);
    case SPN_STATUS_BIT_SIZE:
        return QVariant(HEADER_BIT_SIZE_STR);
    default:
        break;
    }

    return SPNItem::getHeaderData(column);
}


bool StatusSPNItem::setData(int column, const QVariant& value) {

    switch(column) {
    case SPN_STATUS_ENUMERATOR_OFFSET:
    {
        if(value.canConvert<StatusMap>()) {

            StatusMap statuses = qvariant_cast<StatusMap>(value);
            mSpnStatus->clearValueDescriptions();

            for(auto iter = statuses.begin(); iter != statuses.end(); ++iter) {
                std::string desc = iter.value().toUtf8().constData();
                mSpnStatus->setValueDescription(iter.key(), desc);
            }

        }

    }   break;
    case SPN_STATUS_BIT_OFFSET:
    {
        if(value.canConvert<u32>()) {
            u32 offset = value.value<u32>();
            if((offset & 0xFF) == offset) {
                mSpnStatus->setBitOffset((u8)offset);
                return true;
            }
        }
    }   break;
    case SPN_STATUS_BIT_SIZE:
    {
        if(value.canConvert<u32>()) {
            u32 size = value.value<u32>();
            if((size & 0xFF) == size) {
                mSpnStatus->setBitSize((u8)size);
                return true;
            }
        }
    }   break;


    }
    return SPNItem::setData(column, value);

}

TreeViewCustomEditor* StatusSPNItem::getCustomEditor(int column) const {

    switch (column) {
    case SPN_STATUS_ENUMERATOR_OFFSET:
        return new SpnStatusEditor;
        break;
    default:
        break;
    }

    return NULL;
}
