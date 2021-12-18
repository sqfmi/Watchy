#include <DS3232RTC.h>
#include "AbstractRTC.h"

class DS3232 : public AbstractRTC {
public:
    DS3232RTC rtc_ds; // TODO: We should not have public member variables
    ~DS3232() {}
    void config(String datetime);
    void clearAlarm();
    void read(tmElements_t &tm);
    void set(tmElements_t tm);
    uint8_t temperature();
    int rtcType();
};