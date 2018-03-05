#ifndef UTILS_H
#define UTILS_H

//J1939 includes
#include <Types.h>

//Qt includes
#include <QString>
/*
class Utils
{
public:
    Utils();
    static void fromMicrosToHoursMinSecMillisAndMicros(u32& hours, u32& min, u32& sec, u32& millis, u32& micros);
    static QString formatTime(u32& hours, u32& min, u32& sec, u32& millis, u32& micros);

};
*/
namespace Utils {



class Time
{
private:
    u32 mHours;
    u32 mMinutes;
    u32 mSeconds;
    u32 mMillis;
    u32 mMicros;

public:
    Time(u64 microSec);

    void setTime(u64 microSec);

    u32 getHours() const { return mHours; }
    u32 getMinutes() const { return mMinutes; }
    u32 getSeconds() const { return mSeconds; }
    u32 getMillis() const { return mMillis; }
    u32 getMicros() const { return mMicros; }

    QString toString() const;
};

}
#endif // UTILS_H
