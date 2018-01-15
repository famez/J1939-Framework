
#include <QDebug>

#include <GenericFrame.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>



#include "numericspnitem.h"
#include "statusspnitem.h"

#include "genericframeitem.h"

using namespace J1939;

GenericFrameItem::GenericFrameItem(TreeItem *parentItem, const GenericFrame *frame) : FrameItem(parentItem, frame){

    mGenFrame =(GenericFrame*)(mFrame);

    parseSPNs(mGenFrame);

   //Enable the possibility to edit certain columns
   setColumnEditable(FRAME_NAME_COLUMN, true);

}



void GenericFrameItem::parseSPNs(GenericFrame* frame) {

    std::set<u32> spnNumbers = frame->getSPNNumbers();

    for(auto iter = spnNumbers.begin(); iter != spnNumbers.end(); ++iter) {

        const SPN* spn = frame->getSPN(*iter);
        SPNItem* item = NULL;
        switch (spn->getType()) {
        case SPN::SPN_NUMERIC:
        {
            SPNNumeric* numeric = (SPNNumeric*)spn;
            item = new NumericSPNItem(this, numeric);
        }   break;
        case SPN::SPN_STATUS:
        {
            SPNStatus* status = (SPNStatus*)spn;
            item = new StatusSPNItem(this, status);
        }   break;
        default:
            continue;
            break;
        }
        if(item) {
            appendChild(item);
        }
    }

}




void GenericFrameItem::addSPN(SPN::EType type, u32 number) {

    qDebug() << "[GenericFrameItem::addSPN] Spn item to add." << "Type" << type << " number " << number;
    SPNItem* item = NULL;

    Q_ASSERT(!mGenFrame->hasSPN(number));

    switch (type) {
    case SPN::SPN_NUMERIC:
    {

        SPNNumeric spnNumeric(number);
        SPNNumeric* spn = static_cast<SPNNumeric*>(mGenFrame->registerSPN(spnNumeric));
        item = new NumericSPNItem(this, spn);
    }    break;
    case SPN::SPN_STATUS:
    {
        SPNStatus spnStatus(number);
        SPNStatus* spn = static_cast<SPNStatus*>(mGenFrame->registerSPN(spnStatus));
        item = new StatusSPNItem(this, spn);
    }    break;
    default:
        break;
    }



    if(item) {
        appendChild(item);
    }

}


bool GenericFrameItem::hasSPN(u32 number) const {

    return mGenFrame->hasSPN(number);

}

void GenericFrameItem::deleteSPN(SPNItem* item) {

    u32 number = item->getSPNNumber();

    deleteChild(item);

    mGenFrame->deleteSPN(number);

}


bool GenericFrameItem::setData(int column, const QVariant& value) {


    switch(column) {
    case FRAME_NAME_COLUMN:
    {
        if(value.canConvert<QString>()) {
            std::string name = value.value<QString>().toUtf8().constData();
            if(!name.empty()) {
                mGenFrame->setName(name);
                return true;
            }
        }
    }   break;
    }


    return FrameItem::setData(column, value);
}
