#ifndef _extra_function_H
#define _extra_function_H

#include<reg52.h>

void delay(unsigned int ten_us);
void exti0_init();
void doShowNixie(unsigned int Location,Number);
void doClearDisp();
void doShowLED(unsigned char Location, unsigned int Timing);
void doBeep(unsigned int Timing);
unsigned int doRandomGen();
unsigned char doKeyScan(unsigned char mode);

void Delay80ms();
void Delay60ms();
void Delay40ms();
void Delay20ms();

void CountDown(unsigned int i);

void SystemPost();
void SystemAttract();
void doWaitLED(unsigned int i);
void doErrorReport(unsigned int i,j);
void exti0();
void exti1();
#endif