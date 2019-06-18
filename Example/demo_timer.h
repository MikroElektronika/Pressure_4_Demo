#include "demo_types.h"

#define __GSM_2_TIMER__

static void gsm2_configTimer()
{
    RCC_APB1ENR.TIM2EN = 1;
    TIM2_CR1.CEN = 0;
    TIM2_PSC = 3;
    TIM2_ARR = 53999;
    NVIC_IntEnable(IVT_INT_TIM2);
    TIM2_DIER.UIE = 1;
    TIM2_CR1.CEN = 1;
}

static void Timer_interrupt() iv IVT_INT_TIM2
{
    gsm2_tick();
    TIM2_SR.UIF = 0;
}