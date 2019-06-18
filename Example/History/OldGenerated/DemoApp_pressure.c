
#include "Pressure4_demo_app_resources.h"
#include "DemoApp_pressure.h"
#include "DemoApp_GSM.h"
#include "demo_types.h"
#include "demo_config.h"

static float Temp_newData = 0.00;
static float Pressure_newData = 0.00;
static float Temp_oldData = 0.00;
static float Pressure_oldData = 0.00;

static char temp_text[ 20 ] = {"0.00"};
static char pressure_text[ 20 ] = {"0.00"};

static float pressureBuffer[22];
static float pressureBuffer2[8];
static uint8_t buffer_cnt = 0;
static uint8_t buffer_cnt2 = 0;

sbit LED_R at GPIOG_ODR.B15;

void pressure_logData(float newData)
{
    Pressure_newData = newData;
#ifdef __DEMO_LOG_TFT_DISPLAY__
    TFT_Set_Font(&Tahoma12x16_Regular, 0xFFFF, FO_HORIZONTAL);
    TFT_Write_Text(pressure_text, 355, 35);
    TFT_Set_Font(&Tahoma12x16_Regular, 0x0000, FO_HORIZONTAL);
    FloatToStr(Pressure_newData, pressure_text);
    TFT_Write_Text(pressure_text, 355, 35);
#endif
    Pressure_oldData = Pressure_newData;
#ifdef __DEMO_LOG_USBUART__
    mikrobus_logWrite("Temperature : ",_LOG_TEXT);
    mikrobus_logWrite(pressure_text,_LOG_TEXT);
    mikrobus_logWrite(" C", _LOG_LINE);
#endif
}

void temperature_logData(float newData)
{
    Temp_newData = newData;
#ifdef __DEMO_LOG_TFT_DISPLAY__
    TFT_Set_Font(&Tahoma12x16_Regular, 0xFFFF, FO_HORIZONTAL);
    TFT_Write_Text(temp_text, 354, 100);
    TFT_Set_Font(&Tahoma12x16_Regular, 0x0000, FO_HORIZONTAL);
    FloatToStr(Temp_newData, temp_text);
    TFT_Write_Text(temp_text, 354, 100);
#endif
    Temp_oldData = Temp_newData;
#ifdef __DEMO_LOG_USBUART__
    mikrobus_logWrite("Temperature : ",_LOG_TEXT);
    mikrobus_logWrite(temp_text,_LOG_TEXT);
    mikrobus_logWrite(" C", _LOG_LINE);
#endif
}

void demoMeasurement(float *demoTemp, float *demoPressure)
{
    float Temp_data;
    float Pressure_Data;
    
    Temp_data = pressure4_getTemperature();
    Pressure_Data = pressure4_getPressure();
    
    pressure_logData(Pressure_Data);
    demoPressure[0] = Pressure_Data;
    
    if(Temp_data > 0)
    {    
        temperature_logData(Temp_data);
        demoTemp[0] = Temp_data;
    }
}

void demoInit_Pressure()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_spiInit( _MIKROBUS1, &_PRESSURE4_SPI_CFG[0] );
    mikrobus_logInit( _MIKROBUS2, 9600 );
    pressure4_spiDriverInit( (T_PRESSURE4_P)&_MIKROBUS1_GPIO, (T_PRESSURE4_P)&_MIKROBUS1_SPI );
    pressure4_init();
    
    GPIO_Digital_Output(&GPIOG_BASE, _GPIO_PINMASK_15);
    LED_R = 0;
#ifdef __DEMO_LOG_USBUART__
    mikrobus_logWrite(" --- Pressure 4 is Initialized --- ", _LOG_LINE  );
#endif
}

void pressure_reportCalculation(float pressure)
{
    pressureBuffer[buffer_cnt++] = pressure;
    if(buffer_cnt > 21)
    {
        uint8_t i;
        uint8_t pressureFlag = 0;
        
        for(i = 0; i < 21; i+=3)
        {
            pressureBuffer2[buffer_cnt2++] = (pressureBuffer[i] + pressureBuffer[i + 1] + pressureBuffer[i + 2])/3.0;
        }
        if(pressureBuffer2[6] < pressureBuffer2[5]) pressureFlag++;
        if(pressureBuffer2[5] < pressureBuffer2[4]) pressureFlag++;
        if(pressureBuffer2[4] < pressureBuffer2[3]) pressureFlag++;
        if(pressureBuffer2[3] < pressureBuffer2[2]) pressureFlag++;
        if(pressureBuffer2[2] < pressureBuffer2[1]) pressureFlag++;
        if(pressureBuffer2[1] < pressureBuffer2[0]) pressureFlag++;
        
        if(pressureFlag >= 4)
        {
            LED_R = 1;
            sendSMS("Pressure is dropping !!!");
            Delay_ms( 500 );
        }
        buffer_cnt = 0;
        buffer_cnt2 = 0;
        memset(pressureBuffer, 0 , 22);
        memset(pressureBuffer2, 0 , 8);
        LED_R = 0;
    }
}