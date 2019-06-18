
#include "DemoApp_FAT32.h"
#include "__Lib_FAT32.h"

sbit Mmc_Chip_Select           at GPIOD_ODR.B12;
sbit MMC_Card_Detect           at GPIOD_IDR.B3;

/* Button */
sbit flagButton at GPIOE_IDR.B3;
sbit pwmButton  at GPIOD_ODR.B15;

__HANDLE fileHandle;

void demoInit_FAT32()
{
    uint8_t FAT_cnt = 0;
    
    SPI2_Init_Advanced(_SPI_FPCLK_DIV64, 
                       _SPI_MASTER | 
                       _SPI_8_BIT |
                       _SPI_CLK_IDLE_LOW | 
                       _SPI_FIRST_CLK_EDGE_TRANSITION |
                       _SPI_MSB_FIRST |
                       _SPI_SS_DISABLE | 
                       _SPI_SSM_ENABLE | 
                       _SPI_SSI_1, &_GPIO_MODULE_SPI2_PB13_14_15);
    
    SPI_Set_Active(SPI2_Read, SPI2_Write);
    SDIO_Reset();
    SDIO_Init(_SDIO_CFG_POWER_SAVE_DISABLE | _SDIO_CFG_4_WIDE_BUS_MODE | _SDIO_CFG_CLOCK_BYPASS_DISABLE
            | _SDIO_CFG_CLOCK_RISING_EDGE | _SDIO_CFG_HW_FLOW_DISABLE, 125, &_GPIO_MODULE_SDIO_D0_D3);

    GPIOD_PUPDRbits.PUPDR2 = 1;
    GPIOC_PUPDRbits.PUPDR8 = 1;
    GPIOC_PUPDRbits.PUPDR9 = 1;
    GPIOC_PUPDRbits.PUPDR10 = 1;
    GPIOC_PUPDRbits.PUPDR11 = 1;

    Mmc_Set_Interface(_MMC_INTERFACE_SDIO);

    if (MMC_Card_Detect == 0)
    {
        while ((FAT32_Init() != 0) && (FAT_cnt < 5))
        FAT_cnt++;
        if (FAT_cnt < 5)
        {
            SDIO_Init(_SDIO_CFG_POWER_SAVE_DISABLE | _SDIO_CFG_4_WIDE_BUS_MODE | _SDIO_CFG_CLOCK_BYPASS_DISABLE
            | _SDIO_CFG_CLOCK_RISING_EDGE | _SDIO_CFG_HW_FLOW_DISABLE, 1, &_GPIO_MODULE_SDIO_D0_D3);
        }
    }
    
    /* Button */
    GPIO_Digital_Input(&GPIOE_BASE, _GPIO_PINMASK_3);
    GPIO_Digital_Output(&GPIOD_BASE, _GPIO_PINMASK_15);
    pwmButton = 1;
}

static void demoMMC_writeData(char *demoText, uint8_t nData)
{
    uint8_t k = 0;
    
    SPI_Set_Active(SPI2_Read, SPI2_Write);
    for (k = 0; k < nData; k++)
    {
        FAT32_Write(fileHandle, demoText, 20);
    }
    SPI_Set_Active(SPI1_Read, SPI1_Write);
}

int demoMMC_openFile()
{
    SPI_Set_Active(SPI2_Read, SPI2_Write);
    fileHandle = FAT32_Open("TODAY.TXT", FILE_WRITE);
    SPI_Set_Active(SPI1_Read, SPI1_Write);
    if (fileHandle < 0)
    {
        return 1;
    }
    return 0;
}

void demoMMC_closeFile()
{
    SPI_Set_Active(SPI2_Read, SPI2_Write);
    FAT32_Close(fileHandle);
    SPI_Set_Active(SPI1_Read, SPI1_Write);
}

void logTime_toFAT32(char *demoTime, char *demoDate)
{
    char mmcTime[20] = {0};
    char mmcDate[20] = {0};

    uint8_t cnt1 = 0;
    uint8_t cnt2 = 0;
    uint8_t cnt = 0;

    for(cnt = 0; cnt < 30; cnt++)
    {
        if((demoTime[cnt] != ' ') && (demoTime[cnt] != 0))
        {
            mmcTime[cnt1++] = demoTime[cnt];
        }
        if((demoDate[cnt] != ' ') && (demoDate[cnt] != 0))
        {
            mmcDate[cnt2++] = demoDate[cnt];
        }
    }

    mmcTime[cnt1] = '\r';
    mmcTime[cnt1 + 1] = '\n';
    mmcDate[cnt2] = '\r';
    mmcDate[cnt2 + 1] = '\n';

    demoMMC_writeData(mmcTime, 1);
    demoMMC_writeData(mmcDate, 1);

    if( flagButton == 1 )
    {
        pwmButton = 0;
        demoMMC_closeFile();
        Delay_100ms();
    }
}

void logData_toFAT32(float pressure, float temp)
{
    char pressureText[10] = {'P','r','e','s','s','u','r','e',':',0};
    char tempText[13] = {'T','e','m','p','e','r','a','t','u','r','e',':',0};
    char demoPressure[20] = {0};
    char demoTemp[20] = {0};
    char mmcPressure[20] = {0};
    char mmcTemp[20] = {0};
    char demoText[20] = {0};
    
    uint8_t cnt1 = 0;
    uint8_t cnt2 = 0;
    uint8_t cnt = 0;
    
    strcpy(mmcPressure, pressureText);
    strcpy(mmcTemp, tempText);
    FloatToStr(pressure, demoText);
    strcat(mmcPressure, demoText);
    FloatToStr(temp, demoText);
    strcat(mmcTemp, demoText);
    mmcPressure[17] = '\r';
    mmcPressure[18] = '\n';
    mmcTemp[18] = '\r';
    mmcTemp[19] = '\n';

    demoMMC_writeData(mmcTemp, 1);
    demoMMC_writeData(mmcPressure, 1);

    if( flagButton == 1 )
    {
        pwmButton = 0;
        demoMMC_closeFile();
        Delay_100ms();
    }
}