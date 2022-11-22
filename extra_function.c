/********
	
		DLNU School Creative Lab Exam Project
		Designed by MuTong233
		
		Copyright MyGensou Studio 2018-2022
		
		The code is licensed under the Unlicensed Open Source License
		
********/

#include<reg52.h>
#include<intrins.h>
#include<stdlib.h>
#define DP_PORT P2

// Data type define
typedef unsigned int u16;
typedef unsigned char u8;

// Bit define
sbit P2_7=P2^7;
sbit P2_6=P2^6;
sbit P2_5=P2^5;
sbit P2_4=P2^4;
sbit P2_3=P2^3;
sbit P2_2=P2^2;
sbit P2_1=P2^1;
sbit P2_0=P2^0;
sbit KEY1=P3^1;
sbit KEY2=P3^0;
sbit KEY3=P3^2;
sbit KEY4=P3^3;
/*******
	
	Nixie Table 
	
	Code: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21  22 23 24 25  26
				| | | | | | | | | | |  |  |  |  |  |  |  |  |  |  |  |   |	|  | 	|  	|
	Reff: 0 1 2 3 4 5 6 7 8 9 a  b  c  d  e  f  h  j  l  t  -  NUL p	o	 n  u/v r
	
********/
// Data Table
unsigned char NixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7c,0x39,0x5e,0x79,0x71,0x76,0x0e,0x38,0x78,0x40,0x00,0x73,0x5c,0x54,0x1c,0x50};
unsigned char LEDTable[]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0x00,0xFF};

// Delay Function
void delay(unsigned int ten_us)
{
	while(ten_us--);
}

// Initialize intrpt, can be unused if you want.
void exti0_init()
{
    IT0=1;
    EX0=1;
		IT1=1;
		EX1=1;
		EA=1;
}

// Nixie Display Function
void doShowNixie(unsigned char Location,Number)
{
	switch(Location)
	{
		case 1:P2_4=1;P2_3=1;P2_2=1;break;
		case 2:P2_4=1;P2_3=1;P2_2=0;break;
		case 3:P2_4=1;P2_3=0;P2_2=1;break;
		case 4:P2_4=1;P2_3=0;P2_2=0;break;
		case 5:P2_4=0;P2_3=1;P2_2=1;break;
		case 6:P2_4=0;P2_3=1;P2_2=0;break;
		case 7:P2_4=0;P2_3=0;P2_2=1;break;
		case 8:P2_4=0;P2_3=0;P2_2=0;break;
	}
	P0=NixieTable[Number];
	delay(100);
}

// LED Clear Function
void doClearDisp()
{
	DP_PORT=0x00;
}

// LED Display Function
void doShowLED(unsigned char Location, unsigned int Timing)
{
	DP_PORT=LEDTable[Location];
	delay(Timing);
}

// Beeper Beep Function
void doBeep(unsigned int Timing)
{
	P2_5=0;
	while(Timing--)
	{
		P2_5=!P2_5;
		delay(100);
	}
	P2_5=0;
}

// Keycode scanner function
unsigned char doKeyScan(u8 mode)
{
	static u8 key=1;

	if(mode)key=1;
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay(200);
		key=0;
		if(KEY1==0)
			return 1;
		else if(KEY2==0)
			return 2;
		else if(KEY3==0)
			return 3;
		else if(KEY4==0)
			return 4;	
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
	{
		key=1;			
	}
	return 0;
}

// Random number generator for fun
unsigned int doRandomGen()
{
	unsigned int a;
	srand(rand());
	a=rand()%10;
	return a;
}

// Some Delay Function for specificed time
// Software Delay, the accuracy is 95%
void Delay80ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 4;
	j = 93;
	k = 155;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay60ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 134;
	k = 115;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay40ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 2;
	j = 175;
	k = 75;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay20ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

// Universal CountDown Function
void CountDown(u16 i)
{
	int DelayTime;
	for(;i>0;i--){
	for(DelayTime=100;DelayTime>0;DelayTime--)
	{
		doShowNixie(1,20);
		doShowNixie(1,21);
		doShowNixie(2,(i/10)%10);
		doShowNixie(2,21);
		doShowNixie(3,i%10);
		doShowNixie(3,21);
		doShowNixie(4,20);
		doShowNixie(4,21);
	}
	}
}

// Waiter(LED Flow)
void doWaitLED(u16 i)
{
	for(;i>0;i--)
		{
			doShowLED(0,10000);
			doShowLED(1,10000);
			doShowLED(2,10000);
			doShowLED(3,10000);
			doShowLED(4,10000);
			doShowLED(5,10000);
			doShowLED(6,10000);
			doShowLED(7,10000);
		}
		doClearDisp();
}

// System Post Screen
void SystemPost()
{
	int DelayTime;
		for(DelayTime=250;DelayTime>0;DelayTime--)
		{
			doShowNixie(1,20);
			doShowNixie(1,21);
			doShowNixie(2,20);
			doShowNixie(2,21);
			doShowNixie(3,22);
			doShowNixie(3,21);
			doShowNixie(4,0);
			doShowNixie(4,21);
			doShowNixie(5,5);
			doShowNixie(5,21);
			doShowNixie(6,19);
			doShowNixie(6,21);
			doShowNixie(7,20);
			doShowNixie(7,21);
			doShowNixie(8,20);
			doShowNixie(8,21);
		}
		for(DelayTime=50;DelayTime>0;DelayTime--)
		{
			doShowNixie(1,20);
			doShowNixie(1,21);
			doShowNixie(2,20);
			doShowNixie(2,21);
			doShowNixie(3,11);
			doShowNixie(3,21);
			doShowNixie(4,14);
			doShowNixie(4,21);
			doShowNixie(5,14);
			doShowNixie(5,21);
			doShowNixie(6,22);
			doShowNixie(6,21);
			doShowNixie(7,20);
			doShowNixie(7,21);
			doShowNixie(8,20);
			doShowNixie(8,21);
			doBeep(1);
		}
		doClearDisp();
		for(DelayTime=100;DelayTime>0;DelayTime--)
		{
			doShowNixie(1,20);
			doShowNixie(1,21);
			doShowNixie(2,20);
			doShowNixie(2,21);
			doShowNixie(3,18);
			doShowNixie(3,21);
			doShowNixie(4,14);
			doShowNixie(4,21);
			doShowNixie(5,13);
			doShowNixie(5,21);
			doShowNixie(6,5);
			doShowNixie(6,21);
			doShowNixie(7,20);
			doShowNixie(7,21);
			doShowNixie(8,20);
			doShowNixie(8,21);
		}
		doWaitLED(8);
		doClearDisp();
		for(DelayTime=8;DelayTime>0;DelayTime--){Delay80ms();}
}

// System Attract Screen
void SystemAttract()
{
	int i=1,DelayTime,KeyOp;
	while(i==1)
	{
		for(DelayTime=150;DelayTime>0;DelayTime--)
		{
			doShowNixie(1,22);
			doShowNixie(1,21);
			doShowNixie(2,23);
			doShowNixie(2,21);
			doShowNixie(3,24);
			doShowNixie(3,21);
			doShowNixie(4,9);
			doShowNixie(4,21);
			KeyOp=doKeyScan(0);
			if(KeyOp!=0) {i=0;}
		}
		for(DelayTime=150;DelayTime>0;DelayTime--)
		{
			doShowNixie(5,23);
			doShowNixie(5,21);
			doShowNixie(6,25);
			doShowNixie(6,21);
			doShowNixie(7,14);
			doShowNixie(7,21);
			doShowNixie(8,26);
			doShowNixie(8,21);
			KeyOp=doKeyScan(0);
			if(KeyOp!=0) {i=0;}
		}
		for(DelayTime=50;DelayTime>0;DelayTime--)
		{
			doShowNixie(1,20);
			doShowNixie(1,21);
			doShowNixie(2,doRandomGen());
			doShowNixie(2,21);
			doShowNixie(3,doRandomGen());
			doShowNixie(3,21);
			doShowNixie(4,20);
			doShowNixie(4,21);
			doShowNixie(5,20);
			doShowNixie(5,21);
			doShowNixie(6,doRandomGen());
			doShowNixie(6,21);
			doShowNixie(7,doRandomGen());
			doShowNixie(7,21);
			doShowNixie(8,20);
			doShowNixie(8,21);
			KeyOp=doKeyScan(0);
			if(KeyOp!=0) {i=0;}
		}
		for(DelayTime=150;DelayTime>0;DelayTime--)
		{
			doShowNixie(1,25);
			doShowNixie(1,21);
			doShowNixie(2,14);
			doShowNixie(2,21);
			doShowNixie(3,26);
			doShowNixie(3,21);
			doShowNixie(4,20);
			doShowNixie(4,21);
			doShowNixie(5,2);
			doShowNixie(5,21);
			doShowNixie(6,0);
			doShowNixie(6,21);
			doShowNixie(7,2);
			doShowNixie(7,21);
			doShowNixie(8,2);
			doShowNixie(8,21);
			KeyOp=doKeyScan(0);
			if(KeyOp!=0) {i=0;}
		}
		KeyOp=doKeyScan(0);
		if(KeyOp!=0) {i=0;}
	}
}

// Initialize Error Handler
void doErrorReport(u16 i,j)
{
	while(1)
	{
		doShowNixie(1,14);
		doShowNixie(1,21);
		doShowNixie(2,i%10);
		doShowNixie(2,21);
		doShowNixie(3,20);
		doShowNixie(3,21);
		doShowNixie(4,j%10);
		doShowNixie(4,21);
		doBeep(1);
	}
}

// Interrupt Function
void exti0() interrupt 0
{
	delay(1000);
	if(KEY3==0)
	doShowNixie(1,1);
	doShowNixie(1,21);
	doShowNixie(2,2);
	doShowNixie(2,21);
	doShowNixie(3,3);
	doShowNixie(3,21);
	doShowNixie(4,4);
	doShowNixie(4,21);
	doShowNixie(5,5);
	doShowNixie(5,21);
	doShowNixie(6,6);
	doShowNixie(6,21);
	doShowNixie(7,7);
	doShowNixie(7,21);
	doShowNixie(8,8);
	doShowNixie(8,21);
}

void exti1() interrupt 2
{
	delay(1000);
	if(KEY4==0)
	doShowLED(8,10000);
}