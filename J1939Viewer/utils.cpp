#include "utils.h"

#define MICROS_PER_MILLI    1000
#define MILLIS_PER_SEC      1000
#define SEC_PER_MINUTE      60
#define MIN_PER_HOUR        60
/*
Utils::Utils()
{

}

void Utils::fromMicrosToHoursMinSecMillisAndMicros(u32& hours, u32& min, u32& sec, u32& millis, u32& micros) {
    millis = micros / MICROS_PER_MILLI;
    micros = micros % MICROS_PER_MILLI;
    sec = millis / MILLIS_PER_SEC;
    millis = millis % MILLIS_PER_SEC;
    min = sec / SEC_PER_MINUTE;
    sec %= SEC_PER_MINUTE;
    hours = min / MIN_PER_HOUR;
    min %= MIN_PER_HOUR;
}


static QString Utils::formatTime(u32& hours, u32& min, u32& sec, u32& millis, u32& micros) {


    return QString::number(hours) + " h " +
            (min ? QString::number(min) + " min " : QString()) +
            (sec ? QString::number(sec) + " s " : QString()) +
            (millis ? QString::number(millis) + " ms" : QString()) +
            (micros ? QString::number(micros) + " us" : QString());
}
*/
namespace Utils {


Time::Time(u64 microSec)
{
    setTime(microSec);
}


void Time::setTime(u64 microSec) {
    u64 millis = (u64)(microSec / MICROS_PER_MILLI);
    mMicros = (u64)(microSec % MICROS_PER_MILLI);
    u64 sec = (u64)(millis / MILLIS_PER_SEC);
    mMillis = (u64)(millis % MILLIS_PER_SEC);
    mMinutes = (u64)(sec / SEC_PER_MINUTE);
    mSeconds = (u64)(sec % SEC_PER_MINUTE);
    mHours = mMinutes / MIN_PER_HOUR;
    mMinutes %= MIN_PER_HOUR;

}


QString Time::toString() const {

    return (mHours ? QString::number(mHours) + " h " : QString()) +
            (mMinutes ? QString::number(mMinutes) + " min " : QString()) +
            (mSeconds ? QString::number(mSeconds) + " s " : QString()) +
            (mMillis ? QString::number(mMillis) + " ms " : QString()) +
            (mMicros ? QString::number(mMicros) + " us" : QString());

}


}
