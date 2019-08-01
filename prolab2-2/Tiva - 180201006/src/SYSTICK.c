

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "SYSTICK.h"

//********************************************************************************
//                  SysTick Timer Assume System clock is 80MHZ
//********************************************************************************

//********************************************************************************
//Systick Initialize:
//********************************************************************************
//Initial Systick Timer
void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_CTRL_R = 0x00000005;
}

//********************************************************************************
//Systick 12.5 ns Delay:
//********************************************************************************
// The delay parameter is 12.5ns in 80 MHz core clock.
void SysTick_Delay(unsigned long delay){
    NVIC_ST_RELOAD_R = delay-1;
    NVIC_ST_CURRENT_R = 0;
    while((NVIC_ST_CTRL_R&0x00010000)==0){
    }
}

//********************************************************************************
//Systick 1 us Delay:
//********************************************************************************
// 80*12.5ns => 1 us
void SysTick_Delay1us(unsigned long delay){
    unsigned long i;
    for(i=0; i<delay; i++)    {
       SysTick_Delay(80);                 // wait 1us
    }
}

//********************************************************************************
//Systick 1 ms Delay:
//********************************************************************************
// 80000*12.5ns => 1 ms
void SysTick_Delay1ms(unsigned long delay){
    unsigned long i;
    for(i=0; i<delay; i++)    {
       SysTick_Delay(80000);              // wait 1ms
    }
}
