
/* Hardware connection between Launchpad and LCD1602 module
||     Lauchpad Pin         <==>      LCD1602 Pin
||         GND              <==>         Pin1
||         Vcc              <==>         Pin2 (3.3V)
||         GND              <==>         Pin3 (connect a 3.9k to GND)
||         PA6              <==>         Pin4  (RS)
||         PA5              <==>         Pin5  (R/W, To GND is only enable to write)
||         PA7              <==>         Pin6  (E)
||         PB0              <==>         Pin7
||         PB1              <==>         Pin8
||         PB2              <==>         Pin9
||         PB3              <==>         Pin10
||         PB4              <==>         Pin11
||         PB5              <==>         Pin12
||         PB6              <==>         Pin13
||         PB7              <==>         Pin14
||         Vcc              <==>         Pin15,  (A, connect a 330R to Vcc)
||         GND              <==>         Pin16,  (K, connect to Ground) */

#include <stdint.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "LCD1602.h"
#include "SYSTICK.h"

#define RW 0x20  //PA5
#define RS 0x40  //PA6
#define E  0x80  //PA7

//********************************************************************************
// LCD1602 Initial GPIO PORTB and PORTA- PA5,PA6,PA7 : GPIO_PortAB_Init()
//********************************************************************************
void GPIO_PortAB_Init(void){
    volatile unsigned long delay;
    //Initial PA7/PA6/PA5
    SYSCTL_RCGC2_R |= 0x00000001;
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTA_AMSEL_R &= ~0xE0;
    GPIO_PORTA_PCTL_R &= ~0xFFF00000;
    GPIO_PORTA_DIR_R |= 0xE0;
    GPIO_PORTA_AFSEL_R &= ~0xE0;
    GPIO_PORTA_DEN_R |= 0xE0;
    GPIO_PORTA_DR8R_R |= 0xE0;

    //Initial PB7 ~ 0
    SYSCTL_RCGC2_R |= 0x00000002;
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTB_AMSEL_R &= ~0xFF;
    GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF;
    GPIO_PORTB_DIR_R |= 0xFF;
    GPIO_PORTB_AFSEL_R &= ~0xFF;
    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTB_DR8R_R |= 0xFF;
}

//********************************************************************************
//LCD1602 Write Commend to LCD module
//********************************************************************************
void Write_Command(unsigned char LCD_Comment){
    GPIO_PORTA_DATA_R &= ~(RS+RW+E);     //RS=0,RW=0,E=0
    GPIO_PORTB_DATA_R = LCD_Comment;         //Write Command
    GPIO_PORTA_DATA_R |= E;                  //RS=0,RW=0,E=1
    GPIO_PORTA_DATA_R &= ~(RS+RW);
    SysTick_Delay(19);                       //Enable width 230 ns
    GPIO_PORTA_DATA_R &= ~(RS+RW+E);     //RS=0,RW=0,E=0
    SysTick_Delay1ms(1);                     //Delay 1 ms
}
//********************************************************************************
//LCD1602 Write DATA to LCD module
//********************************************************************************
void Write_Data(unsigned char LCD_Data){
    GPIO_PORTB_DATA_R = LCD_Data;            //Write Data
    GPIO_PORTA_DATA_R |= RS+E;               //RS=1,RW=0,E=1
    GPIO_PORTA_DATA_R &= ~RW;
    SysTick_Delay(19);                       //Enable width 230 ns
    GPIO_PORTA_DATA_R &= ~(RS+RW+E);     //RS=0,RW=0,E=0
    SysTick_Delay1ms(1);                     //Delay 1 ms
}
//********************************************************************************
//LCD1602 Initialize:
//********************************************************************************
void LCD1602_Init(){
    GPIO_PortAB_Init();
    SysTick_Delay1ms(15);                    //Delay 15ms
    Write_Command(0x38);
    SysTick_Delay1ms(5);                     //Delay 5ms
    Write_Command(0x38);
    SysTick_Delay1us(150);                   //Delay 150us

    Write_Command(0x0C);
    Write_Command(0x01);
    Write_Command(0x06);

    SysTick_Delay1ms(50);                    //Delay 50ms
}
//********************************************************************************
//LCD1602 Clear Screen:
//********************************************************************************
void LCD1602_Clear(){
    Write_Command(0x01);
}

//********************************************************************************
//LCD1602 Display String to Screen:
//********************************************************************************
void LCD1602_DisplayString(unsigned char *str){
    while(*str != 0){
        Write_Data(*str++);
    }
}

//********************************************************************************
//LCD1602 Display Char to Screen:
//********************************************************************************
void LCD1602_DisplayChar(unsigned char CHAR){
    Write_Data(CHAR);
}

//********************************************************************************
//LCD1602 Display Decimal number to Screen:
//********************************************************************************
void LCD1602_DisplayDec(unsigned int number){
    if(number >=10){
        LCD1602_DisplayDec(number/10);
        number = number%10;
    }
    LCD1602_DisplayChar(number+'0');
}
//********************************************************************************
//LCD1602 Set Cursor Position on the Screen:
//********************************************************************************
void LCD1602_DisplayPosition(unsigned char Line,unsigned int digit){
    Write_Command(Line + digit);
}
//********************************************************************************
//LCD1602 support printf() function
//********************************************************************************
int fputc(int ch, FILE *f){
    if((ch == 10) || (ch == 13) || (ch == 27)){
        LCD1602_DisplayChar(13);
        LCD1602_DisplayChar(10);
        return 1;
    }
    LCD1602_DisplayChar(ch);
    return 1;
}
