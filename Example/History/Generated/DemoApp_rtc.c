#include "Pressure4_demo_app_resources.h"
#include "DemoApp_initTime.h"
#include "DemoApp_rtc.h"
#include "demo_types.h"
#include "demo_config.h"

extern uint8_t _setHourNow;
extern uint8_t _setMin;
extern uint8_t _setSec;
extern uint8_t _setDay;
extern uint8_t _setMonth;
extern uint8_t _setYear;

static rtc6_time_t utc_time;
static rtc6_time_t alarm_time;
static rtc6_time_t local_time;
static char demoText[10] = {0};
static int8_t time_zone = 2;

static char logTime[256] = {0};
static char logDate[256] = {0};

sbit LED_G at GPIOB_ODR.B3;

void demoInit_RTC()
{
    mikrobus_gpioInit( _MIKROBUS2, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_i2cInit( _MIKROBUS2, &_RTC6_I2C_CFG[0] );
    mikrobus_logInit( _MIKROBUS3  , 9600 );
    rtc6_i2cDriverInit( (T_RTC6_P)&_MIKROBUS2_GPIO, (T_RTC6_P)&_MIKROBUS2_I2C, 0x6F );
    rtc6_init( time_zone );
    
    /* Set UTC time */
    utc_time.seconds  = _setSec;
    utc_time.minutes  = _setMin;
    utc_time.hours    = _setHourNow;
    utc_time.monthday = _setDay;
    utc_time.month    = _setMonth;
    utc_time.year     = _setYear;

    rtc6_setGMT_time( &utc_time );
    rtc6_repeatAlarm(_RTC6_ALARM_0, 20);
    
    /* Alarm led*/
    GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_3);
    LED_G = 0;
    
#ifdef __DEMO_LOG_USBUART__
    mikrobus_logWrite(" --- RTC 6 is Initialized --- ", _LOG_LINE  );
#endif
}

void rtc_readTime(char *demoTime, char *demoDate)
{
    rtc6_getGMT_time( &utc_time );
    rtc_logTime();
    
    strcpy(demoTime, logTime);
    strcpy(demoDate, logDate);
}

uint8_t rtc_checkAlarm()
{
    if(rtc6_isActiveAlarm() != 0)
    {
#ifdef __DEMO_LOG_USBUART__
        mikrobus_logWrite(" ----- Active alarm ----- ", _LOG_LINE);
#endif
        LED_G = 1;
        Delay_ms( 300 );
        rtc6_disableAlarm( _RTC6_ALARM_0 );
        rtc6_repeatAlarm(_RTC6_ALARM_0, 5);
        return 1;
    }
    LED_G = 0;
    return 0;
}

void rtc_logTime()
{
    char demo_txt[ 30 ];
    uint8_t cTime[6] = {'T','i','m','e',':',0};
    uint8_t cDate[6] = {'D','a','t','e',':',0};
    uint8_t dot2[2] = {':',0};
    uint8_t dot1[2] = {'.',0};
    uint8_t cEND[2] ={'\0',0};

    strcpy(logTime, cTime);
    IntToStr(utc_time.hours, demo_txt);
    strcat(logTime, demo_txt);
    strcat(logTime, dot2);
    IntToStr(utc_time.minutes, demo_txt);
    strcat(logTime, demo_txt);
    strcat(logTime, dot2);
    IntToStr(utc_time.seconds, demo_txt);
    strcat(logTime, demo_txt);
    strcat(logTime,cEND);
    
    strcpy(logDate, cDate);
    IntToStr(utc_time.monthday, demo_txt);
    strcat(logDate, demo_txt);
    strcat(logDate, dot1);
    IntToStr(utc_time.month, demo_txt);
    strcat(logDate, demo_txt);
    strcat(logDate, dot1);
    IntToStr(utc_time.year, demo_txt);
    strcat(logDate, demo_txt);

#ifdef __DEMO_LOG_USBUART__
    mikrobus_logWrite("--- UTC time ---", _LOG_LINE);
    mikrobus_logWrite(logTime, _LOG_LINE);
    mikrobus_logWrite(logDate, _LOG_LINE);
#endif

#ifdef __DEMO_LOG_TFT_DISPLAY__
    TFT_Set_Font(&Tahoma12x16_Regular, 0x0000, FO_HORIZONTAL);
    TFT_Set_Pen(0xFFFF, 1);
    TFT_Set_Brush(1, 0xFFFF, 0, 0, 0, 0);
    TFT_Rectangle(300,135,500,170);
    TFT_Write_Text(logTime, 340, 140);
    TFT_Write_Text(logDate, 340, 160);
#endif
}