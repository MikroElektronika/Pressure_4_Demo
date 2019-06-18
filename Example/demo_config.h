#include "demo_types.h"

#ifndef _CFG_DEMO_APP_T_
#define _CFG_DEMO_APP_T_

static const uint32_t _PRESSURE4_SPI_CFG[ 2 ] =
{
    _SPI_FPCLK_DIV256,
    _SPI_FIRST_CLK_EDGE_TRANSITION |
    _SPI_CLK_IDLE_LOW |
    _SPI_MASTER |
    _SPI_MSB_FIRST |
    _SPI_8_BIT |
    _SPI_SSM_ENABLE |
    _SPI_SS_DISABLE |
    _SPI_SSI_1
};

static const uint32_t _RTC6_I2C_CFG[ 1 ] =
{
    100000
};

static const uint32_t _GSM2_UART_CFG[ 4 ]   =
{
    9600,
    _UART_8_BIT_DATA,
    _UART_NOPARITY,
    _UART_ONE_STOPBIT
};

#endif