

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "PLL80MHZ.h"

//********************************************************************************
//                  USE PLL to set system clock to 80MHZ
//********************************************************************************
//Initial PLL 80MHZ
void PLL_Init80MHZ(void)
{
    SYSCTL_RCC2_R |=  0x80000000;
    SYSCTL_RCC2_R |=  0x00000800;
    SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)
                 + 0x00000540;
    SYSCTL_RCC2_R &= ~0x00000070;
    SYSCTL_RCC2_R &= ~0x00002000;
    SYSCTL_RCC2_R |= 0x40000000;
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)
                  + (4<<22);
    while((SYSCTL_RIS_R&0x00000040)==0){};
    SYSCTL_RCC2_R &= ~0x00000800;
}
