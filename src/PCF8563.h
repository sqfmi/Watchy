#include <Rtc_Pcf8563.h>
#include "AbstractRTC.h"

class PCF8563 : public AbstractRTC {
public:
    Rtc_Pcf8563 rtc_pcf; // TODO: We should not have public member variables
    ~PCF8563() {}
    void config(String datetime);
    void clearAlarm();
    void read(tmElements_t &tm);
    void set(tmElements_t tm);
    int rtcType();
private:
    int getDayOfWeek(int d, int m, int y);
};
