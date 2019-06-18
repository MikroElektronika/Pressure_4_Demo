#include "Pressure4_demo_app_resources.h"
#include "DemoApp_displayGraphics.h"

static uint8_t pMatrix[28][80] absolute 0x20000000;
static uint8_t tMatrix[28][80] absolute 0x200008C1;

static const uint8_t _TEMP_MATRIX = 0x00;
static const uint8_t _PRESSURE_MATRIX = 0x01;

void START_INIT()
{
    TFT_Set_Font(&Tahoma26x33_Regular, 0xF800, FO_HORIZONTAL);
    TFT_Write_Text("Initializes sensors", 200, 140);
}

void END_INIT()
{
    TFT_Set_Font(&Tahoma26x33_Regular, 0xFFFF, FO_HORIZONTAL);
    TFT_Write_Text("Initializes sensors", 200, 140);
}

static void drawDot_3x3(uint16_t x, uint16_t y, uint16_t color)
{
    TFT_Dot(x - 1 , y - 1, color);
    TFT_Dot(x     , y - 1, color);
    TFT_Dot(x + 1 , y - 1, color);

    TFT_Dot(x - 1 , y, color);
    TFT_Dot(x     , y, color);
    TFT_Dot(x + 1 , y, color);

    TFT_Dot(x - 1 , y + 1, color);
    TFT_Dot(x     , y + 1, color);
    TFT_Dot(x + 1 , y + 1, color);
}

void resetMatrix()
{
    uint8_t i;
    uint8_t j;

    for(i = 0; i < 28; i++)
    {
        for (j = 0; j < 80; j++)
        {
            pMatrix[i][j] = 0;
            tMatrix[i][j] = 0;
        }
    }
}

static void display_newElement( uint8_t pos, uint8_t matrix )
{
    uint8_t i;
    uint8_t j;
    uint8_t newMatrix[28][80];

    for (i = 0; i < 28; i++)
    {
        for (j = 0; j < 80; j++)
        {
            if(matrix != _TEMP_MATRIX)
            {
                newMatrix[i][j] = pMatrix[i][j];
                pMatrix[i][j] = 0;
            }
            else
            {
                newMatrix[i][j] = tMatrix[i][j];
                tMatrix[i][j] = 0;
            }
        }
    }

    for (i = 0; i < 28; i++)
    {
        for (j = 1; j < 80; j++)
        {
            if(matrix != _TEMP_MATRIX)
            {
                pMatrix[i][j] = newMatrix[i][j - 1];
            }
            else
            {
                tMatrix[i][j] = newMatrix[i][j - 1];
            }
        }
    }
    
    if(matrix != _TEMP_MATRIX)
    {
        pMatrix[pos][0] = 1;
    }
    else
    {
        tMatrix[pos][0] = 1;
    }
}

void display_pressureGraphics(float demoPressure)
{
    uint8_t i;
    uint8_t j;
        
    if(demoPressure > 0)
    {
        display_newElement((uint8_t)((demoPressure - 1000) * 0.5), _PRESSURE_MATRIX);
    }
    else
    {
        display_newElement((uint8_t)((1000 -  demoPressure) * 0.5), _PRESSURE_MATRIX);
    }

    for (i = 0; i < 28; i++)
    {
        for (j = 0; j < 80; j++)
        {
            if(pMatrix[i][j] == 1)
            {
                drawDot_3x3((j * 3) + 50 , 110 - (i * 3), 0x3660);
            }
            else
            {
                drawDot_3x3((j * 3) + 50 , 110 - (i * 3), 0xFFFF);
            }
        }
    }
}

void display_temperatureGraphics(float demoTemperature)
{
    uint8_t i;
    uint8_t j;

    if((uint8_t)demoTemperature > 0)
    {
        display_newElement((uint8_t)(demoTemperature * 0.7), _TEMP_MATRIX);
    }

    for (i = 0; i < 28; i++)
    {
        for (j = 0; j < 80; j++)
        {
            if(tMatrix[i][j] == 1)
            {
                drawDot_3x3((j * 3) + 50 , 235 - (i * 3), 0x633F);
            }
            else
            {
                drawDot_3x3((j * 3) + 50 , 235 - (i * 3), 0xFFFF);
            }
        }
    }
}