#include <J1939Frame.h>



#include "frameitem.h"





using namespace J1939;

#define ACTION_ADD_SPN      "Add SPN"

FrameItem::FrameItem(TreeItem *parentItem, const J1939Frame* frame) : TreeItem(parentItem), mFrame(frame->clone())
{

    //Enable the possibility to edit certain columns

    setColumnEditable(FRAME_SRC_ADDR_COLUMN, true);
    setColumnEditable(FRAME_DST_ADDR_COLUMN, true);
    setColumnEditable(FRAME_PRIORITY_COLUMN, true);


}

FrameItem::~FrameItem() {
    delete mFrame;
}

int FrameItem::columnCount() const {
    return J1939FRAME_FIELDS;
}

QVariant FrameItem::data(int column) const {

    switch(column) {
    case FRAME_NAME_COLUMN:
        return QVariant(QString::fromStdString(mFrame->getName()));
    case FRAME_PGN_COLUMN:
        return QVariant(mFrame->getPGN());
    case FRAME_SRC_ADDR_COLUMN:
        return QVariant(mFrame->getSrcAddr());
    case FRAME_DST_ADDR_COLUMN:
        return QVariant(mFrame->getDstAddr());
    case FRAME_PRIORITY_COLUMN:
        return QVariant(mFrame->getPriority());
    }

    return QVariant();
}



QVariant FrameItem::getHeaderData(int column) const {

    switch(column) {
    case FRAME_NAME_COLUMN:
        return "Name";
    case FRAME_PGN_COLUMN:
        return "PGN";
    case FRAME_SRC_ADDR_COLUMN:
        return "Source";
    case FRAME_DST_ADDR_COLUMN:
        return "Destination";
    case FRAME_PRIORITY_COLUMN:
        return "Priority";
    }

    return QVariant();

}

bool FrameItem::setData(int column, const QVariant& value) {




    switch(column) {

    case FRAME_SRC_ADDR_COLUMN:
    {
        if(value.canConvert<u32>()) {
            u32 source = value.value<u32>();
            if((source & J1939_SRC_ADDR_MASK) == source) {
                mFrame->setSrcAddr(source);
                return true;
            }
        }

    }   break;

    case FRAME_DST_ADDR_COLUMN:
    {
        if(value.canConvert<u32>()) {
            u32 dest = value.value<u32>();
            if((dest & J1939_DST_ADDR_MASK) == dest) {
                mFrame->setDstAddr(dest);
                return true;
            }
        }

    }   break;

    case FRAME_PRIORITY_COLUMN:
    {
        if(value.canConvert<u32>()) {
            u32 priority = value.value<u32>();

            if((priority & J1939_PRIORITY_MASK) == priority) {
                return mFrame->setPriority(priority);
            }
        }

    }   break;

    default:
        break;

    }

    return false;


}
