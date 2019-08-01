#include <stdint.h>
#include <stdio.h>
#include "stdlib.h"
#include <stdbool.h>
#include "string.h"
#include "inc/tm4c123gh6pm.h"
#include "LCD1602.h"
#include "SYSTICK.h"
#include "PLL80MHZ.h"
#include "string.h"
#include "stdint.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"


void PLL_Init(void);

unsigned char msg1[] = "180201006";
unsigned char *msg[1] = {msg1};

void veriGonder(uint32_t nBase, char *veri)
{
	int i = 0;
	while(veri[i] != '\0')
	{

			while(UARTCharPutNonBlocking(nBase, veri[i++]));

	}
}


volatile char c[10];
volatile int i = 0, katSayi = 100,


		stokCay = 0, stokKahve = 0, stokBiskuvi = 0, // stok yiyecekler
		stokSu = 0, stokCikolata = 0,

		krs25, krs50, tl1 // otomattaki paralar
		;
void Int_Uart()
{
	uint32_t durum;

	durum = UARTIntStatus(UART0_BASE, true);
	UARTIntClear(UART0_BASE, durum);
	char c = UARTCharGetNonBlocking(UART0_BASE);


	if (c == 'C'){
		stokCay = i;
		i = 0;
		katSayi = 100;
	}

	else if(c == 'K'){
		stokKahve = i;
		i = 0;
		katSayi = 100;
	}

	else if(c == 'B'){
		stokBiskuvi = i;
		i = 0;
		katSayi = 100;
	}

	else if(c == 'S'){
		stokSu = i;
		i = 0;
		katSayi = 100;
	}

	else if (c == 'c'){
		stokCikolata = i;
		i = 0;
		katSayi = 100;
	}

	else if(c == 'Y'){
		krs25 = i;
		i = 0;
		katSayi = 100;
	}

	else if(c == 'E'){
		krs50 = i;
		i = 0;
		katSayi = 100;
	}

	else if(c == 'T'){
		tl1 = i;
		i = 0;
		katSayi = 100;
	}
	else {
		int temp = c - '0';
		i += temp*katSayi;
		katSayi /= 10;
	}


}

void init_UART()
{
	SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_INT_PIN_1);

	IntDisable(INT_UART0);
	UARTDisable(UART0_BASE);
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

	UARTConfigSetExpClk(UART0_BASE, 16000000, 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
	UARTFIFODisable(UART0_BASE);

	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	UARTIntRegister(UART0_BASE, Int_Uart);

	UARTEnable(UART0_BASE);

	IntEnable(INT_UART0);

}



void init_port_C() {
	volatile unsigned long tmp; // bu degisken gecikme yapmak icin gerekli
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOC; // Port F’nin saatini aktifleştir
	tmp = SYSCTL_RCGCGPIO_R;    // Saatin başlaması için gecikme
	GPIO_PORTC_LOCK_R = 0x40006000;   // Port F GPIO kilidini aç

	GPIO_PORTC_CR_R = 0b00000000;       // PF4-0 kilidini aç (Sadece PF0 kilitlidir, diğer bitler kilitli değildir.)
	GPIO_PORTC_DIR_R = 0b00000000;      // PF4,PF0 giriş, PF3-1 çıkış
	GPIO_PORTC_PUR_R = 0b11110000; // PF0 ve PF4 üzerindeki pull-up direncini aktifleştir
	GPIO_PORTC_DEN_R = 0b11110000; // PF4-0 digital I/O aktifleştir }

}

void init_port_D() {

	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // Port D’yi aktiflestir
	delay = SYSCTL_RCGC2_R;  // Saatin başlaması için gecikme
	GPIO_PORTD_DIR_R |= 0b00001111; // PD 3,2,1,0 pinlerini cikis yap
	GPIO_PORTD_AFSEL_R &= ~(0b00001111); // PD 3,2,1,0 pinlerini alternatif fonksinunu 0 yap
	GPIO_PORTD_DEN_R |= 0b00001111; // PD 3,2,1,0 pinlerini aktiflestir }

}

void init_port_F() {
	volatile unsigned long tmp; // bu degisken gecikme yapmak icin gerekli
	SYSCTL_RCGCGPIO_R |= 0x00000008; // Port F’nin saatini aktifleştir
	tmp = SYSCTL_RCGCGPIO_R;    // Saatin başlaması için gecikme
	GPIO_PORTD_LOCK_R = 0x4C4F434B;   // Port F GPIO kilidini aç
	GPIO_PORTD_CR_R = 0x01;       // PF4-0 kilidini aç (Sadece PF0 kilitlidir, diğer bitler kilitli değildir.)
	GPIO_PORTD_DIR_R = 0x0E;      // PF4,PF0 giriş, PF3-1 çıkış
	GPIO_PORTD_PUR_R = 0x11; // PF0 ve PF4 üzerindeki pull-up direncini aktifleştir
    GPIO_PORTD_DEN_R = 0x1F; // PF4-0 digital I/O aktifleştir }
}

volatile long delay;

void lcdReset()
{


	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Reset Modu      ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	delay = 1;

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("$ BTN1 - Y BTN2 ");
}

void lcdYiyecek()
{

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Yiyecek Modu    ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	delay = 1;
}

void lcdKasaGoster(int krs25, int krs50, int tl1)
{
	char para[2];
	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("                ");

	// 25 krs
	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("25 krs:");

	itoa(krs25, para, 10);

	LCD1602_DisplayPosition(LINE1,7);
	LCD1602_DisplayString(para);

	// 50 krs
	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("50 krs:");

	itoa(krs50, para, 10);

	LCD1602_DisplayPosition(LINE2,7);
	LCD1602_DisplayString(para);

	// 1TL
	LCD1602_DisplayPosition(LINE2,9);
	LCD1602_DisplayString("1TL:");

	itoa(tl1, para, 10);

	LCD1602_DisplayPosition(LINE2,13);
	LCD1602_DisplayString(para);

	delay = 1;
}

void lcdPara()
{
	//bayrak6=0, bayrak5 = 0, bayrak4=0;

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Para Modu       ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	delay = 1;
}



void lcdYiyecekOnayi()
{

	//bayrak6=0, bayrak5 = 0, bayrak4=0;

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Yiyecek Onayi   ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("icin Buton 2 bas");
}

void temp(int a, char *c)
{
	int i= 0,temp = a;
	while(temp > 0)
	{
		c[i++] = temp%10;
		temp = temp/10;
	}
}

void lcdSuYazdir(int stok)
{
	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Su : 50         ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("Stok : ");


	char c[3];

	itoa(stok, c, 10);

	LCD1602_DisplayPosition(LINE2,8);
	LCD1602_DisplayString(c);


}

void lcdCikolataYazdir(int stok)
{
	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Cikolata : 1.75");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("               ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("Stok: ");

	char c[3];

	itoa(stok, c, 10);

	LCD1602_DisplayPosition(LINE2,8);
	LCD1602_DisplayString(c);
}

void lcdCayYazdir(int stok)
{
	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Cay : 1 TL      ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("Stok : ");

	char c[3];

	itoa(stok, c, 10);

	LCD1602_DisplayPosition(LINE2,8);
	LCD1602_DisplayString(c);
}

void lcdBiskuviYazdir(int stok)
{

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Biskuvi : 2 TL  ");



	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("Stok : ");

	char c[3];

	itoa(stok, c, 10);

	LCD1602_DisplayPosition(LINE2,8);
	LCD1602_DisplayString(c);
}

void lcdKahveYazdir(int stok)
{

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("Kahve : 1.5 TL  ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("Stok : ");

	char c[3];

	itoa(stok, c, 10);

	LCD1602_DisplayPosition(LINE2,8);
	LCD1602_DisplayString("        ");
	LCD1602_DisplayPosition(LINE2,8);
	LCD1602_DisplayString(c);
}

int paraUstuBirTL(float para, int tl1)
{
	int adet = 0;

	adet = para/1;

	if(tl1 >= adet)
		return adet;
	else
		return -1;
}

int paraUstu50Kurus(float para, int krs50)
{
	int adet = 0;

	adet = para/0.5;

	if(krs50 >= adet)
		return adet;
	else
		return -1;
}

int paraUstu25Kurus(float para, int krs25)
{
	int adet = 0;

	adet = para/0.25;

	if(krs25 >= adet)
		return adet;
	else
		return -1;
}

void paraUstu1TLYazdir(int tl1)
{

	char c[3];

	itoa(tl1, c, 10);

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("1TL:");

	LCD1602_DisplayPosition(LINE1,4);
	LCD1602_DisplayString(c);
}

void paraUstu50KrsYazdir(int kurus50)
{
	char c[3];

	itoa(kurus50, c, 10);


	LCD1602_DisplayPosition(LINE1,7);
	LCD1602_DisplayString("50Krs:");

	LCD1602_DisplayPosition(LINE1,14);
	LCD1602_DisplayString(c);

}

void paraUstu25KrsYazdir(int kurus25)
{
	char c[3];

	itoa(kurus25, c, 10);

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("25Krs:");

	LCD1602_DisplayPosition(LINE2,7);
	LCD1602_DisplayString(c);

}


void floatToString(char* p, float x)
{
    int n,i=0,k=0;
    n=(int)x;
    while(n>0)
    {
        x/=10;
        n=(int)x;
        i++;
    }
    *(p+i) = '.';
    x *= 10;
    n = (int)x;
    x = x-n;
    while((n>0)||(i>k))
    {
        if(k == i)
            k++;
        *(p+k)='0'+n;
        x *= 10;
        n = (int)x;
        x = x-n;
        k++;
    }
    /* Null-terminated string */
    *(p+k) = '\0';
}

void yetersizBakiye()
{
	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");
	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("Yetersiz Bakiye!");
}


void atilanParayiGoster(int atilan25, int atilan50, int atilan1, char para[])
{
	float atilanPara = 0.25*atilan25 + 0.5*atilan50 + 1*atilan1;

	floatToString(para, atilanPara);

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString(para);

	LCD1602_DisplayPosition(LINE2, 5);
	LCD1602_DisplayString("TL Attiniz");
}

void bekleyinizYesil()
{
	/*
	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");


	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("LUTFEN");
	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("BEKLEYINIZ");*/

	GPIO_PORTD_DATA_R |= 0b00001000;

	SysTick_Delay1ms(1000);

	GPIO_PORTD_DATA_R &= ~(0b00001000);

}

void bekleyinizKirmizi(int rnd)
{
	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");


	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("PARA SIKISTI");
	char c[3];
	itoa(rnd, c, 10);

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("RANDOM : ");


	LCD1602_DisplayPosition(LINE2,10);
	LCD1602_DisplayString(c);

	GPIO_PORTD_DATA_R |= 0b00000100;

	SysTick_Delay1ms(500);

	GPIO_PORTD_DATA_R &= ~(0b00000100);

}

void gonder25Kurus()
{
	/*
	-----------------------------------------------------------------------------------------------------------------------
	        								25 kurus bilgisini gondermek
	---------------------------------------------------------------------------------------------------------------------*/

	itoa(krs25, c, 10);
	if(krs25 >= 100){
		c[3] = 'Y';

		for(i = 0; i < 4; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(krs25 < 100){
		c[2] = 'Y';

		for(i = 0; i < 3; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(krs25 < 10)
	{
		c[1] = 'Y';
		for(i = 0; i < 2; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}


}

void gonder50Kurus()
{

	/*
	-----------------------------------------------------------------------------------------------------------------------
											50 kurus bilgisini gondermek
	---------------------------------------------------------------------------------------------------------------------*/

	itoa(krs50, c, 10);
	if(krs50 >= 100){
		c[3] = 'E';

		for(i = 0; i < 4; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(krs50 < 100 && krs50 >= 10){
		c[2] = 'E';

		for(i = 0; i < 3; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(krs50 < 10)
	{
		c[1] = 'E';
		for(i = 0; i < 2; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
}

void gonder1TL()
{
	/*
	-----------------------------------------------------------------------------------------------------------------------
											1TL bilgisini gondermek
	---------------------------------------------------------------------------------------------------------------------*/

	itoa(tl1, c, 10);
	if(tl1 >= 100){
		c[3] = 'T';

		for(i = 0; i < 4; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(tl1 < 100 && tl1 >= 10){
		c[2] = 'T';

		for(i = 0; i < 3; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));

	}
	else if(tl1 < 10)
	{
		c[1] = 'T';
		for(i = 0; i < 2; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
}

void gonderSu()
{
	/*
	-----------------------------------------------------------------------------------------------------------------------
											stokSu bilgisini gondermek
	---------------------------------------------------------------------------------------------------------------------*/

	itoa(stokSu, c, 10);
	if(stokSu >= 100){
		c[3] = 'S';

		for(i = 0; i < 4; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokSu < 100 && stokSu >= 10){
		c[2] = 'S';

		for(i = 0; i < 3; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokSu < 10)
	{
		c[1] = 'S';
		for(i = 0; i < 2; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
}

void gonderCay()
{

	/*
	-----------------------------------------------------------------------------------------------------------------------
											stokCay bilgisini gondermek
	---------------------------------------------------------------------------------------------------------------------*/

	itoa(stokCay, c, 10);
	if(stokCay >= 100){
		c[3] = 'C';

		for(i = 0; i < 4; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokCay < 100 && stokCay >= 10){
		c[2] = 'C';

		for(i = 0; i < 3; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokCay < 10)
	{
		c[1] = 'C';
		for(i = 0; i < 2; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
}

void gonderCikolata()
{



	/*
	-----------------------------------------------------------------------------------------------------------------------
											stokCikolata bilgisini gondermek
	---------------------------------------------------------------------------------------------------------------------*/

	itoa(stokCikolata, c, 10);
	if(stokCikolata >= 100){
		c[3] = 'c';

		for(i = 0; i < 4; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokCikolata < 100 && stokCikolata >= 10){
		c[2] = 'c';

		for(i = 0; i < 3; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokCikolata < 10)
	{
		c[1] = 'c';
		for(i = 0; i < 2; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
}

void gonderBiskuvi()
{

	/*
	-----------------------------------------------------------------------------------------------------------------------
											stokBiskuvi bilgisini gondermek
	---------------------------------------------------------------------------------------------------------------------*/

	itoa(stokBiskuvi, c, 10);
	if(stokBiskuvi >= 100){
		c[3] = 'B';

		for(i = 0; i < 4; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokBiskuvi < 100 && stokBiskuvi >= 10){
		c[2] = 'B';

		for(i = 0; i < 3; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokBiskuvi < 10)
	{
		c[1] = 'B';
		for(i = 0; i < 2; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
}

void gonderKahve()
{
	/*
	-----------------------------------------------------------------------------------------------------------------------
											stokKahve bilgisini gondermek
	---------------------------------------------------------------------------------------------------------------------*/

	itoa(stokKahve, c, 10);
	if(stokKahve >= 100){
		c[3] = 'K';

		for(i = 0; i < 4; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokKahve < 100 && stokKahve >= 10){
		c[2] = 'K';

		for(i = 0; i < 3; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}
	else if(stokKahve < 10)
	{
		c[1] = 'K';
		for(i = 0; i < 2; i++)
			while(!UARTCharPutNonBlocking(UART0_BASE, c[i]));
	}

}

void yetersizStok()
{

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");


	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("YETERSIZ STOK");
/*
	SysTick_Delay1ms(750);

	LCD1602_DisplayPosition(LINE1,0);
	LCD1602_DisplayString("                ");

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");*/
}

void alinanYazdir(int alinan)
{

	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString("                ");

	itoa(alinan, c, 10);




	LCD1602_DisplayPosition(LINE2,0);
	LCD1602_DisplayString(c);


	LCD1602_DisplayPosition(LINE2,3);
	LCD1602_DisplayString("adet aldiniz");

}


int main(void){

    PLL_Init80MHZ();
    SysTick_Init();
    LCD1602_Init();
    LCD1602_DisplayPosition(LINE1,3);
    LCD1602_DisplayString(msg[0]);

    init_port_D();
	init_port_C();
	init_port_F();
	init_UART();

	char para[3];
	volatile int  PC7, PC6, PC5, PC4,

			degistir = 0, secim = 0, // degistir -> otomat mod switch
									 // secim 	 -> ilgili switchdeki butonların modları (su ile cayin ayni butonda olmasi)
									 //				bir kere basmak su, iki kere basmak cay verir




			alinanCay = 0, alinanKahve = 0, alinanBiskuvi = 0, // alinan yiyecekler
			alinanSu = 0, alinanCikolata = 0,


			bayrak25 = 0, bayrak50 = 0, bayrak1 = 0,


			atilan25 = 0, atilan50 = 0, atilan1 = 0; // atilan paralar

	//krs25 = 20, krs50 = 20, tl1 = 10;
;
	int icecek = 0, yiyecek = 0;
	float odenecek = 0;

	float atilanPara = 0, tutAtilanPara = 0;

	unsigned int rasgele;

    while(1)
    {
    	rasgele++;
    	PC7 = GPIO_PORTC_DATA_R & 0b10000000;
		PC6 = GPIO_PORTC_DATA_R & 0b01000000;
		PC5 = GPIO_PORTC_DATA_R & 0b00100000;
		PC4 = GPIO_PORTC_DATA_R & 0b00010000;

		//GPIO_PORTD_DATA_R |= 0b00001000;


        if (PC7 == 0){
        	if(degistir%5 == 0)
        	{
        		lcdYiyecek();
        	}
        	else if(degistir%5 == 1)
        	{
        		lcdPara();
        	}

        	else if(degistir%5 == 2)
        	{
        		lcdReset();
        	}

        	else if(degistir%5 == 3)
			{
        		lcdYiyecekOnayi();
			}
        	else if(degistir%5 == 4)
        	{

				LCD1602_DisplayPosition(LINE1,0);
				LCD1602_DisplayString("                ");

				LCD1602_DisplayPosition(LINE2,0);
				LCD1602_DisplayString("                ");

				LCD1602_DisplayPosition(LINE1,0);
				LCD1602_DisplayString("KASA BILGISI");


				LCD1602_DisplayPosition(LINE2,0);
				LCD1602_DisplayString("BUTON2");


        	}

        	degistir++;
        }
        else if (PC6 == 0)
        {

        	if(degistir%5 == 0)
        		lcdKasaGoster(krs25, krs50, tl1);

				if(degistir%5 ==1)
				{
					icecek = 0;
					if(secim%2 == 0)
					{
						lcdBiskuviYazdir(stokBiskuvi);
						yiyecek = 1;
					}
					if(secim%2 == 1)
					{
						lcdCikolataYazdir(stokCikolata);
						yiyecek = 2;
					}
					secim++;
				}
				else if(degistir%5 == 2)
				{
					if(secim%3 == 0)
					{
						// 25 KURUS SECİMİ

						bayrak25 = 1; bayrak50 = bayrak1 = 0;

						LCD1602_DisplayPosition(LINE1,0);
						LCD1602_DisplayString("25 Kurus    ");

						LCD1602_DisplayPosition(LINE2,0);
						LCD1602_DisplayString("                ");

						atilanParayiGoster(atilan25, atilan50, atilan1, para);
					}
					else if(secim%3 == 1)
					{
						// 50 KURUS SECİMİ

						bayrak50 = 1; bayrak25 = bayrak1 = 0;

						LCD1602_DisplayPosition(LINE1,0);
						LCD1602_DisplayString("50 Kurus    ");

						LCD1602_DisplayPosition(LINE2,0);
						LCD1602_DisplayString("                ");

						atilanParayiGoster(atilan25, atilan50, atilan1, para);

					}
					else if(secim%3 == 2)
					{
						// 1 TL SECİMİ

						bayrak1 = 1; bayrak25 = bayrak50 = 0;

						LCD1602_DisplayPosition(LINE1,0);
						LCD1602_DisplayString("1 TL         ");

						LCD1602_DisplayPosition(LINE2,0);
						LCD1602_DisplayString("                ");

						atilanParayiGoster(atilan25, atilan50, atilan1, para);
					}


				secim++;

        	}
        	else if(degistir%5 == 3)
        	{
        		//bayrak6=0, bayrak5 = 0, bayrak4=0;
        		LCD1602_DisplayPosition(LINE1,0);
				LCD1602_DisplayString("PARA IADE EDILDI");

				LCD1602_DisplayPosition(LINE2,0);
				LCD1602_DisplayString("                ");

				krs25 = krs25 - atilan25;
				krs50 = krs50 - atilan50;
				tl1 = 	 tl1   - atilan1;
				atilan25 = atilan50 = atilan1 = 0;					// otomattaki para girdilerini sifirla
				odenecek = 0;
        	}
        	else if(degistir%5 == 4)
        	{
/*
-----------------------------------------------------------------------------------------------------------------------
        								EKRANI TEMIZLE
---------------------------------------------------------------------------------------------------------------------*/

        		LCD1602_DisplayPosition(LINE1,0);
        		LCD1602_DisplayString("                ");

        		LCD1602_DisplayPosition(LINE2,0);
        		LCD1602_DisplayString("                ");
/*
-----------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------*/

        		int rnd = rasgele%4+1;

        		if(rnd != 2)
        		{
            		if((tl1*1+krs50*2+krs25*4) >= odenecek)
            		{
            			// OTOMATTAKI PARA EKSILECEK

						//paraUstuHesapla(tutAtilanPara - odenecek, krs25, krs50, tl1);

						int gelenDeger;
						odenecek = tutAtilanPara - odenecek;
						gelenDeger = paraUstuBirTL(odenecek, tl1);

						if(gelenDeger != -1)
						{
							odenecek = odenecek - gelenDeger;
							tl1 = tl1 - gelenDeger;
							paraUstu1TLYazdir(gelenDeger);
						}

						gelenDeger = paraUstu50Kurus(odenecek, krs50);

						if(gelenDeger != -1)
						{
							odenecek = odenecek - gelenDeger*0.5;
							krs50 = krs50 - gelenDeger;
							paraUstu50KrsYazdir(gelenDeger);
						}

						gelenDeger = paraUstu25Kurus(odenecek, krs25);

						if(gelenDeger != -1)
						{
							odenecek = odenecek - gelenDeger*0.25;
							krs25 = krs25 - gelenDeger;
							paraUstu25KrsYazdir(gelenDeger);
						}

						// PARA GIRDISI SIFIRLANACAK
						atilan25 = atilan50 = atilan1 = 0;					// otomattaki para girdilerini sifirla
						odenecek = 0;
						tutAtilanPara = 0; atilanPara = 0;

						alinanCay = alinanSu = alinanKahve = alinanBiskuvi = alinanCikolata = 0;

/*
-----------------------------------------------------------------------------------------------------------------------
										Seri porta bilgileri yazmak
---------------------------------------------------------------------------------------------------------------------*/
											gonder25Kurus();
											gonder50Kurus();
											gonder1TL();
											gonderSu();
											gonderCay();
											gonderKahve();
											gonderCikolata();
											gonderBiskuvi();
/*
-----------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------*/

						bekleyinizYesil();

		        		LCD1602_DisplayPosition(LINE1,0);
		        		LCD1602_DisplayString("                ");

		        		LCD1602_DisplayPosition(LINE2,0);
		        		LCD1602_DisplayString("                ");
            		}
            		else
            		{

                		LCD1602_DisplayPosition(LINE1,0);
        				LCD1602_DisplayString("KASADA YETERLI  ");

        				LCD1602_DisplayPosition(LINE2,0);
        				LCD1602_DisplayString("PARA YOK!       ");


        				GPIO_PORTD_DATA_R |= 0b00000100;

        				SysTick_Delay1ms(500);

        				GPIO_PORTD_DATA_R &= ~(0b00000100);

            		}

        		}
        		else if(rnd == 2)
        		{
            		stokCay += alinanCay;
            		stokKahve += alinanKahve;
            		stokSu += alinanSu;
            		stokBiskuvi += alinanBiskuvi;
            		stokCikolata += alinanCikolata;

            		alinanCay = alinanSu = alinanKahve = alinanBiskuvi = alinanCikolata = 0;
            		odenecek = 0;

            		krs25 -=atilan25; krs50 -= atilan50; tl1 -= atilan1;

            		atilan25 = atilan50 = atilan1 = 0;

            		tutAtilanPara = 0;
            		odenecek = 0;

        			bekleyinizKirmizi(rnd);
        		}

        	}
        }
        else if(PC5 == 0)
        {
        	if(degistir%5 == 1)
        	{
        		yiyecek = 0;
				if(secim%3 == 0)
				{
					icecek = 1;
					lcdCayYazdir(stokCay);
				}
				if(secim%3 == 1)
				{
					icecek = 2;
					lcdSuYazdir(stokSu);
				}
				if(secim%3 == 2)
				{
					icecek = 3;
					lcdKahveYazdir(stokKahve);

				}
        		secim++;
        	}
        	else if(degistir%5 == 3)
        	{
        		// alinanlari geri birak
        		stokCay += alinanCay;
        		stokKahve += alinanKahve;
        		stokSu += alinanSu;
        		stokBiskuvi += alinanBiskuvi;
        		stokCikolata += alinanCikolata;

        		alinanCay = alinanSu = alinanKahve = alinanBiskuvi = alinanCikolata = 0;
        		odenecek = 0;
				LCD1602_DisplayPosition(LINE1,0);
				LCD1602_DisplayString("YIYECEK IADE    ");
				LCD1602_DisplayPosition(LINE2,0);
				LCD1602_DisplayString("EDILDI          ");
        	}
        }

        else if(PC4 == 0)
		{
        	if(degistir%5 == 1)
        	{
        		if(yiyecek != 0)
        		{
					if(yiyecek == 1)
					{
        				if(stokBiskuvi >= 0)
        				{
        					if( ( (tutAtilanPara - odenecek) - 2 ) >= 0 )
							{
								atilanPara -= 2;
								alinanBiskuvi++;
								stokBiskuvi--;
								odenecek += 2;
								alinanYazdir(alinanBiskuvi);
							}
							else
								yetersizBakiye();
        				}
        				else
        					yetersizStok();
					}
        			if(yiyecek == 2)
        			{
        				if(stokCikolata > 0)
        				{
        					if( ( (tutAtilanPara - odenecek) - 1.75 ) >= 0 )
							{
								atilanPara -= 1.75;
								alinanCikolata++;
								stokCikolata--;
								odenecek += 1.75;
								alinanYazdir(alinanCikolata);
							}
							else
								yetersizBakiye();
        				}
        				else
        					yetersizStok();

        			}
        		}

        		if(icecek != 0)
        		{
        			if(icecek == 1)
        			{
        				if(stokCay > 0)
        				{
        					if( ( (tutAtilanPara - odenecek) - 1 ) >= 0 )
							{
								atilanPara -= 1;
								alinanCay++;
								stokCay--;
								odenecek += 1;
								alinanYazdir(alinanCay);
							}
							else
								yetersizBakiye();
        				}
        				else
        					yetersizStok();

        			}
        			if(icecek == 2)
        			{
        				if(stokSu > 0)
        				{
        					if( ( (tutAtilanPara - odenecek) - 0.5 ) >= 0 )
							{
								atilanPara -= 0.5;
								alinanSu++;
								stokSu--;
								odenecek += 0.5;
								alinanYazdir(alinanSu);
							}
							else
								yetersizBakiye();
        				}
        				else
        					yetersizStok();

        			}
        			if(icecek == 3)
        			{
        				if(stokKahve > 0)
        				{
        					if( ( (tutAtilanPara - odenecek) - 1.5 ) >= 0 )
							{
								atilanPara -= 1.5;
								alinanKahve++;
								stokKahve--;
								odenecek += 1.5;
								alinanYazdir(alinanKahve);
							}
							else
								yetersizBakiye();
						}
        				else
        					yetersizStok();
        			}
        		}

        	}

        	else if(degistir%5 == 2)
        	{

        		if(bayrak25 == 1)
        		{
            		atilan25++; 		// kullanicinin attigi para
            		krs25 = krs25 +1; // otomattaki para
    				tutAtilanPara += 0.25;
        		}
        		if(bayrak50 == 1)
        		{
					atilan50++;			// kullanicinin attigi para
					krs50 = krs50 + 1; // otomattaki para
					tutAtilanPara += 0.5;
        		}
        		if(bayrak1 == 1)
        		{
					atilan1++;			// kullanicinin attigi para
					tl1 = tl1 + 1; 	// otomattaki para
					tutAtilanPara += 1;
        		}


				atilanParayiGoster(atilan25, atilan50, atilan1, para);

        	}

        	else if (degistir%5 == 4)
        	{

        		int r = rasgele%4+1;
        		if(r == 2)
        		{
        			GPIO_PORTD_DATA_R |= 0b00001000;

        			SysTick_Delay1ms(500);

        			GPIO_PORTD_DATA_R &= ~(0b00001000);

        		}
				LCD1602_DisplayPosition(LINE1,0);
				LCD1602_DisplayString("                ");

				LCD1602_DisplayPosition(LINE2,0);
				LCD1602_DisplayString("                ");

				itoa(r, c, 10);
				LCD1602_DisplayPosition(LINE2,0);
				LCD1602_DisplayString(c);


/*
				LCD1602_DisplayPosition(LINE1,0);
				LCD1602_DisplayString("                ");

				LCD1602_DisplayPosition(LINE2,0);
				LCD1602_DisplayString("                ");

        		lcdKasaGoster(krs25, krs50, tl1);*/
        	}

		}

        SysTick_Delay1ms(150);
    }
}

