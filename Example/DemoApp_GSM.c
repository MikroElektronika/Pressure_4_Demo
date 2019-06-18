
#include "DemoApp_GSM.h"
#include "demo_types.h"
#include "demo_config.h"
#include "demo_timer.h"

/* Parser config */
const uint32_t _GSM2_TIMER_LIMIT      = 25;
const uint16_t _GSM2_BUF_WARNING      = 192;
const uint8_t  _GSM2_POLL_ENABLE      = 0;
const uint8_t  _GSM2_CALLBACK_ENABLE  = 1;

static void gsm2_default_handler( uint8_t *rsp, uint8_t *evArgs )
{
    UART6_Write_Text( rsp );
}

void sendSMS(char *demoText)
{
    gsm2_cmdSingle ("AT+CSCS=\"GSM\"");
    gsm2_cmdSingle( "AT+CMGS=\"+381XXXXXXXXX\"\r");
    Delay_ms(300);
    gsm2_cmdSingle( demoText );
    Delay_100ms();
    UART2_Write(0x1A);
    UART2_Write_Text("\r\n");
    Delay_ms(300);
}

void demoInit_GSM()
{
    mikrobus_gpioInit( _MIKROBUS3, _MIKROBUS_AN_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS3, _MIKROBUS_PWM_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS3, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS3, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS3, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_uartInit( _MIKROBUS3, &_GSM2_UART_CFG[0] );

    RXNEIE_USART2_CR1_bit = 1;
    NVIC_IntEnable( IVT_INT_USART2 );
    EnableInterrupts();
    
    UART6_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_UART6_PC67);
    
// TIMER INIT
    gsm2_configTimer();

// DRIVER INIT
    gsm2_uartDriverInit((T_GSM2_P)&_MIKROBUS3_GPIO, (T_GSM2_P)&_MIKROBUS3_UART);
    gsm2_coreInit( gsm2_default_handler, 1500 );

// MODULE POWER ON
    gsm2_hfcEnable( 1 );
    gsm2_modulePower( 1 );

// MODULE INIT
    gsm2_cmdSingle( "AT" );
    gsm2_cmdSingle( "ATE0" );
    gsm2_cmdSingle( "AT+CMGF=1" );
    
    sendSMS("Start of measurement and logging :)");
}

static void UART_RX_ISR()iv IVT_INT_USART2 ics ICS_AUTO
{
    if( RXNE_USART2_ISR_bit )
    {
        char tmp = USART2_RDR;
        gsm2_putc(tmp);
    }
}