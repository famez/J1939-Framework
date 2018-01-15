#include <SPN/SPNNumeric.h>

#include "numericspnitem.h"



#define NUMERIC_SPN_COLUMN_ITEMS        4


/*Column numbers*/
#define SPN_NUMERIC_FORMAT_OFFSET       (SPN_COLUMN_ITEMS) + 0
#define SPN_NUMERIC_FORMAT_GAIN         (SPN_COLUMN_ITEMS) + 1
#define SPN_NUMERIC_BYTE_SIZE           (SPN_COLUMN_ITEMS) + 2
#define SPN_NUMERIC_UNITS               (SPN_COLUMN_ITEMS) + 3


#define HEADER_FORMAT_OFFSET_STR        "Format Offset"
#define HEADER_FORMAT_GAIN_STR          "Format Gain"
#define HEADER_BYTE_SIZE                "Byte Size"
#define HEADER_UNITS                    "Units"


using namespace J1939;

NumericSPNItem::NumericSPNItem(TreeItem *parentItem, SPNNumeric* spn) : SPNItem(parentItem, spn), mSpnNumeric(spn) {

    setColumnEditable(SPN_NUMERIC_FORMAT_OFFSET, true);
    setColumnEditable(SPN_NUMERIC_FORMAT_GAIN, true);
    setColumnEditable(SPN_NUMERIC_BYTE_SIZE, true);
    setColumnEditable(SPN_NUMERIC_UNITS, true);

}


int NumericSPNItem::columnCount() const {
    return NUMERIC_SPN_COLUMN_ITEMS + SPNItem::columnCount();
}


QVariant NumericSPNItem::data(int column) const {

    switch(column) {
    case SPN_NUMERIC_FORMAT_OFFSET:
        return QVariant(mSpnNumeric->getFormatOffset());
    case SPN_NUMERIC_FORMAT_GAIN:
        return QVariant(mSpnNumeric->getFormatGain());
    case SPN_NUMERIC_BYTE_SIZE:
        return QVariant(mSpnNumeric->getByteSize());
    case SPN_NUMERIC_UNITS:
        return QVariant(QString::fromStdString(mSpnNumeric->getUnits()));
    default:
        break;
    }

    return SPNItem::data(column);

}

QVariant NumericSPNItem::getHeaderData(int column) const {

    switch(column) {
    case SPN_NUMERIC_FORMAT_OFFSET:
        return QVariant(HEADER_FORMAT_OFFSET_STR);
    case SPN_NUMERIC_FORMAT_GAIN:
        return QVariant(HEADER_FORMAT_GAIN_STR);
    case SPN_NUMERIC_BYTE_SIZE:
        return QVariant(HEADER_BYTE_SIZE);
    case SPN_NUMERIC_UNITS:
        return QVariant(HEADER_UNITS);
    default:
        break;
    }

    return SPNItem::getHeaderData(column);
}



bool NumericSPNItem::setData(int column, const QVariant& value) {

    switch(column) {
    case SPN_NUMERIC_FORMAT_OFFSET:
    {
        if(value.canConvert<s32>()) {
            s32 offset = value.value<s32>();
            mSpnNumeric->setFormatOffset(offset);
            return true;
        }
    }   break;
    case SPN_NUMERIC_FORMAT_GAIN:
    {
        if(value.canConvert<double>()) {
            double gain = value.value<double>();
            mSpnNumeric->setFormatGain(gain);
            return true;
        }
    }   break;
    case SPN_NUMERIC_BYTE_SIZE:
    {
        if(value.canConvert<u32>()) {
            u32 byteSize = value.value<u32>();

            if((byteSize & 0xFF) == byteSize) {
                mSpnNumeric->setByteSize((u8)byteSize);
                return true;
            }

        }
    }   break;

    case SPN_NUMERIC_UNITS:
    {
        if(value.canConvert<QString>()) {
            std::string units = value.value<QString>().toUtf8().constData();
            if(!units.empty()) {
                mSpnNumeric->setUnits(units);
                return true;
            }
        }
    }   break;


    }
    return SPNItem::setData(column, value);


}
