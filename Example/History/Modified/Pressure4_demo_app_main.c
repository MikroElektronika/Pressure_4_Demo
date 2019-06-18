#include "Pressure4_demo_app_objects.h"
#include "Pressure4_demo_app_resources.h"
#include "DemoApp_displayGraphics.h"
#include "DemoApp_initTime.h"
#include "DemoApp_pressure.h"
#include "DemoApp_rtc.h"
#include "DemoApp_FAT32.h"
#include "DemoApp_GSM.h"

static char demoTime[50];
static char demoDate[50];
static float demoTemperature = 0.00;
static float demoPressure = 0.00;
static uint8_t alarm;

void applicationInit()
{
    START_INIT();
    
    demoInit_Pressure();
    demoInit_RTC();
    demoInit_FAT32();
    demoInit_GSM();
    
    END_INIT();

    if( demoMMC_openFile() != 0 )
    {
        mikrobus_logWrite("Error, open file !!!", _LOG_LINE);
    }
    resetMatrix();
}

void applicationTask()
{
    demoMeasurement( &demoTemperature, &demoPressure );
    rtc_readTime( demoTime, demoDate );
    alarm = rtc_checkAlarm();
    if(alarm != 0)
    {
        pressure_reportCalculation(demoPressure);
    }

    display_pressureGraphics(demoPressure);
    display_temperatureGraphics(demoTemperature);

    logTime_toFAT32(demoTime, demoDate);
    logData_toFAT32(demoPressure, demoTemperature);
    Delay_ms( 500 );
}

void main()
{
  Start_TP();

    while( initTime() != 1)
    {
    Check_TP();
    }
    
    applicationInit();

    while (1)
    {
        applicationTask();
  }
}
