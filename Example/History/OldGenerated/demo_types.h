#ifndef _DEMO_APP_T_
#define _DEMO_APP_T_

#include "stdint.h"

#ifndef _DEMO_APP_H_

#define T_PRESSURE4_P const uint8_t*
#define T_RTC6_P const uint8_t*
#define T_GSM2_P const uint8_t*

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t weekdays;
    uint8_t monthday;
    uint8_t month;
    uint8_t year;

} rtc6_time_t;

#endif
#endif