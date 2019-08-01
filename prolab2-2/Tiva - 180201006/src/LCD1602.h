
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

#define LINE1 0x80 //First Line of LCD1602
#define LINE2 0xC0 //Second Line of LCD1602
//********************************************************************************
//LCD1602 Initialize:
//********************************************************************************
void LCD1602_Init(void);

//********************************************************************************
//LCD1602 Clear Screen:
//********************************************************************************
void LCD1602_Clear(void);

//********************************************************************************
//LCD1602 Display String to Screen:
//********************************************************************************
void LCD1602_DisplayString(unsigned char *str);

//********************************************************************************
//LCD1602 Display Char to Screen:
//********************************************************************************
void LCD1602_DisplayChar(unsigned char CHAR);

//********************************************************************************
//LCD1602 Display Decimal number to Screen:
//********************************************************************************
void LCD1602_DisplayDec(unsigned int number);

//********************************************************************************
//LCD1602 Set Cursor Position on the Screen:
//Line : LINE1 or LINE2
//Digit : from 0 ~ 15
//********************************************************************************
void LCD1602_DisplayPosition(unsigned char Line,unsigned int digit);
