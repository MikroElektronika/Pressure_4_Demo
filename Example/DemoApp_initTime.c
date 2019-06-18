#include "DemoApp_initTime.h"
#include "Pressure4_demo_app_objects.h"
#include "Pressure4_demo_app_resources.h"

uint8_t _button;
uint8_t _flag = 0;
uint8_t pressButton = 0;

uint8_t _setHourNow = 0 absolute 0x20000001;
uint8_t _setMin = 0;
uint8_t _setSec = 0;
uint8_t _setDay = 0;
uint8_t _setMonth = 0;
uint8_t _setYear = 0;
uint16_t _alarmInSec = 0;

static char demoText[5] = {0};

uint8_t initTime()
{
    if (_flag == 0)
    {
        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 120, 56);
    }
/************************** SET HOUR ******************************************/
    if (_flag == 1 && pressButton == 1)
    {
        if( _button < 3)
        {
            _setHourNow += _button * 10;
        }
        else
        {
            _setHourNow += 20;
            _button = 2;
        }
        
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        TFT_Write_Text("0", 120, 56);
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 120, 56);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 120, 56);
        
        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 145, 56);
        pressButton = 0;
    }
    if (_flag == 2 && pressButton == 1)
    {
        _setHourNow += _button;

        if( _setHourNow > 23)
        {
            _setHourNow = 23;
            _button = 3;
        }
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 145, 56);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 145, 56);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 210, 56);
        pressButton = 0;
    }
/************************** SET MINUTE ************ ***************************/
    if (_flag == 3 && pressButton == 1)
    {
        if(_button < 6)
        {
            _setMin += _button * 10;
        }
        else
        {
            _setMin += 50;
            _button = 5;
        }
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 210, 56);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 210, 56);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 235, 56);
        pressButton = 0;
    }
    if (_flag == 4 && pressButton == 1)
    {
        _setMin += _button;
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 235, 56);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 235, 56);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 300, 56);
        pressButton = 0;
    }
/************************** SET SECUNDE ***************************************/
    if (_flag == 5 && pressButton == 1)
    {
        if(_button < 6)
        {
            _setSec += _button * 10;
        }
        else
        {
            _setSec += 50;
            _button = 5;
        }
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 300, 56);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 300, 56);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 325, 56);
        pressButton = 0;
    }
    if (_flag == 6 && pressButton == 1)
    {
        _setSec += _button;
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 325, 56);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 325, 56);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 120, 105);
        pressButton = 0;
    }
/*************************** SET DAY ******************************************/
    if (_flag == 7 && pressButton == 1)
    {
        if(_button < 4)
        {
            _setDay += _button * 10;
        }
        else
        {
            _setDay = 30;
            _button = 3;
        }
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 120, 105);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 120, 105);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 145, 105);
        pressButton = 0;
    }
    if (_flag == 8 && pressButton == 1)
    {
        _setDay += _button;
        if(_setDay > 31)
        {
            _setDay = 31;
            _button = 1;
        }
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 145, 105);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 145, 105);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 210, 105);
        pressButton = 0;
    }
/************************** SET MONTH *****************************************/
    if (_flag == 9 && pressButton == 1)
    {
        if(_button < 2)
        {
            _setMonth += _button * 10;
        }
        else
        {
            _setMonth += 10;
            _button = 1;
        }
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 210, 105);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 210, 105);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 235, 105);
        pressButton = 0;
    }
    if (_flag == 10 && pressButton == 1)
    {
        _setMonth += _button;
        if(_setMonth > 12)
        {
            _setMonth = 12;
            _button = 2;
        }
        else if( _setMonth == 0)
        {
            _setMonth = 1;
        }
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 235, 105);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 235, 105);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 300, 105);
        
        if((_setMonth == 4) || (_setMonth == 6) || (_setMonth == 9) || (_setMonth == 11))
        {
            if(_setDay > 30)
            {
                _setDay = 30;
                TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
                TFT_Write_Text("1", 145, 105);
                TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
                TFT_Write_Text("0", 145, 105);
            }
        }
        else if(_setMonth == 2 && _setDay > 28)
        {
            TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
            demoText[0] = (_setDay / 10) + 48;
            TFT_Write_Text(demoText, 120, 105);
            TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
            demoText[0] = (_setDay % 10) + 48;
            TFT_Write_Text(demoText, 145, 105);
        }
        pressButton = 0;
    }
/************************** SET YEAR ******************************************/
    if (_flag == 11 && pressButton == 1)
    {
        _setYear += _button * 10;
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 300, 105);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 300, 105);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 325, 105);
        pressButton = 0;
    }
    if (_flag == 12 && pressButton == 1)
    {
        _setYear += _button;
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 325, 105);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 325, 105);
        
        pressButton = 0;
        if (_setMonth == 2)
        {
            if(_setDay == 30)
            {
                TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
                TFT_Write_Text("3", 120, 105);
                TFT_Write_Text("0", 145, 105);
            }
            else if(_setDay == 31)
            {
                TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
                TFT_Write_Text("3", 120, 105);
                TFT_Write_Text("1", 145, 105);
            }
        
            if((_setYear + 2000)%4 == 0)
            {
                if(_setDay > 29)
                {
                    _setDay = 29;
                    TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
                    TFT_Write_Text("2", 120, 105);
                    TFT_Write_Text("9", 145, 105);
                }
            }
            else
            {
                if(_setDay > 28)
                {
                    if( _setDay == 29 )
                    {
                        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
                        TFT_Write_Text("9", 145, 105);
                    }
                    _setDay = 28;
                    TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
                    TFT_Write_Text("2", 120, 105);
                    TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
                    TFT_Write_Text("8", 145, 105);
                }
            }
        }
        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 370, 82);
    }
/************************** SET REPEAT ALARM **********************************/
    if (_flag == 13 && pressButton == 1)
    {
        _alarmInSec += _button * 1000;
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 370, 82);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 370, 82);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 395, 82);
        
        pressButton = 0;
    }
    if (_flag == 14 && pressButton == 1)
    {
        _alarmInSec += _button * 100;
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 395, 82);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 395, 82);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 420, 82);
        
        pressButton = 0;
    }
    if (_flag == 15 && pressButton == 1)
    {
        _alarmInSec += _button * 10;
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 420, 82);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 420, 82);

        TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
        TFT_Write_Text("0", 445, 82);
        
        pressButton = 0;
    }
    if (_flag == 16 && pressButton == 1)
    {
        _alarmInSec += _button;
        TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
        TFT_Write_Text("0", 445, 82);
        TFT_Set_Font(&Tahoma26x33_Regular, 0x0000, FO_HORIZONTAL);
        demoText[0] = _button + 48;
        TFT_Write_Text(demoText, 445, 82);

        pressButton = 0;
        Delay_ms( 1500 );
        DrawScreen(&Screen1);
        return 1;
    }
    return 0;
}