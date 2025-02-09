/********

                Ping-Pong Game for 8051 w/ PlatformIO.

                Copyright Team MyGensou 2018-2025

                The code is licensed under the GPLv3 Open Source License

********/

#include <8052.h>
#include <STC89xx.h>
#include <stdlib.h>
#define DP_PORT P2

// Data type define
typedef unsigned int u16;
typedef unsigned char u8;

// Bit define
#define P2_7 P27
#define P2_6 P26
#define P2_5 P25
#define P2_4 P24
#define P2_3 P23
#define P2_2 P22
#define P2_1 P21
#define P2_0 P20
#define KEY1 P31
#define KEY2 P30
#define KEY3 P32
#define KEY4 P33

/*******

        Nixie Table

Reff: 0 1 2 3 4 5 6 7 8 9 a  b  c  d  e  f  h  j  l  t  -  NUL p	o
n  u/v r

********/
// Data Table
unsigned char NixieTable[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
                              0x07, 0x7F, 0x6F, 0x77, 0x7c, 0x39, 0x5e,
                              0x79, 0x71, 0x76, 0x0e, 0x38, 0x78, 0x40,
                              0x00, 0x73, 0x5c, 0x54, 0x1c, 0x50};
unsigned char LEDTable[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF,
                            0xDF, 0xBF, 0x7F, 0x00, 0xFF};

// Delay Function
// TODO: Totally remove this function for **NOTHING**
void delay(unsigned int ten_us) { while(ten_us--); }

// Initialize interrupt, can be unused if you want.
// TODO: Replace with HAL things and ISR things.
void exti0_init() {
  IT0 = 1;
  EX0 = 1;
  IT1 = 1;
  EX1 = 1;
  EA = 1;
}

// Nixie Display Function
void doShowNixie(unsigned char Location, int Number) {
  switch (Location) {
  case 1:
    P2_4 = 1;
    P2_3 = 1;
    P2_2 = 1;
    break;
  case 2:
    P2_4 = 1;
    P2_3 = 1;
    P2_2 = 0;
    break;
  case 3:
    P2_4 = 1;
    P2_3 = 0;
    P2_2 = 1;
    break;
  case 4:
    P2_4 = 1;
    P2_3 = 0;
    P2_2 = 0;
    break;
  case 5:
    P2_4 = 0;
    P2_3 = 1;
    P2_2 = 1;
    break;
  case 6:
    P2_4 = 0;
    P2_3 = 1;
    P2_2 = 0;
    break;
  case 7:
    P2_4 = 0;
    P2_3 = 0;
    P2_2 = 1;
    break;
  case 8:
    P2_4 = 0;
    P2_3 = 0;
    P2_2 = 0;
    break;
  }
  P0 = NixieTable[Number];
  delay(100);
}

// LED Clear Function
void doClearDisp() { DP_PORT = 0x00; }

// LED Display Function
void doShowLED(unsigned char Location, unsigned int Timing) {
  DP_PORT = LEDTable[Location];
  delay(Timing);
}

// Beeper Beep Function
void doBeep(unsigned int Timing) {
  P2_5 = 0;
  while (Timing--) {
    P2_5 = !P2_5;
    delay(100);
  }
  P2_5 = 0;
}

// Keycode scanner function
unsigned char doKeyScan(u8 mode) {
  static u8 key = 1;

  if (mode)
    key = 1;
  if (key == 1 && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)) {
    delay(200);
    key = 0;
    if (KEY1 == 0)
      return 1;
    else if (KEY2 == 0)
      return 2;
    else if (KEY3 == 0)
      return 3;
    else if (KEY4 == 0)
      return 4;
  } else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1) {
    key = 1;
  }
  return 0;
}

// Random number generator for fun
unsigned int doRandomGen() {
  unsigned int a;
  srand(rand());
  a = rand() % 10;
  return a;
}

// Some Delay Function for specificed time
// TODO: REPLACE IT WITH HARDWARE TIMER PLZ.
void Delay80ms() //@11.0592MHz
{
  unsigned char i, j, k;

  i = 4;
  j = 93;
  k = 155;
  do {
    do {
      while (--k)
        ;
    } while (--j);
  } while (--i);
}

void Delay60ms() //@11.0592MHz
{
  unsigned char i, j, k;

  i = 3;
  j = 134;
  k = 115;
  do {
    do {
      while (--k)
        ;
    } while (--j);
  } while (--i);
}

void Delay40ms() //@11.0592MHz
{
  unsigned char i, j, k;

  i = 2;
  j = 175;
  k = 75;
  do {
    do {
      while (--k)
        ;
    } while (--j);
  } while (--i);
}

void Delay20ms() //@11.0592MHz
{
  unsigned char i, j, k;

  i = 1;
  j = 216;
  k = 35;
  do {
    do {
      while (--k)
        ;
    } while (--j);
  } while (--i);
}

// Universal countdown function + LED display
// TODO: ... how i did this in 2022, plz rewrite
void CountDown(u16 i) {
  int DelayTime;
  for (; i > 0; i--) {
    for (DelayTime = 100; DelayTime > 0; DelayTime--) {
      doShowNixie(1, 20);
      doShowNixie(1, 21);
      doShowNixie(2, (i / 10) % 10);
      doShowNixie(2, 21);
      doShowNixie(3, i % 10);
      doShowNixie(3, 21);
      doShowNixie(4, 20);
      doShowNixie(4, 21);
    }
  }
}

// Waiter(LED Flow)
void doWaitLED(u16 i) {
  for (; i > 0; i--) {
    doShowLED(0, 10000);
    doShowLED(1, 10000);
    doShowLED(2, 10000);
    doShowLED(3, 10000);
    doShowLED(4, 10000);
    doShowLED(5, 10000);
    doShowLED(6, 10000);
    doShowLED(7, 10000);
  }
  doClearDisp();
}

// System Post Screen
// TODO: Rewrite this dumb stuff with real hardware POST check.
void SystemPost() {
  int DelayTime;
  for (DelayTime = 250; DelayTime > 0; DelayTime--) {
    doShowNixie(1, 20);
    doShowNixie(1, 21);
    doShowNixie(2, 20);
    doShowNixie(2, 21);
    doShowNixie(3, 22);
    doShowNixie(3, 21);
    doShowNixie(4, 0);
    doShowNixie(4, 21);
    doShowNixie(5, 5);
    doShowNixie(5, 21);
    doShowNixie(6, 19);
    doShowNixie(6, 21);
    doShowNixie(7, 20);
    doShowNixie(7, 21);
    doShowNixie(8, 20);
    doShowNixie(8, 21);
  }
  for (DelayTime = 50; DelayTime > 0; DelayTime--) {
    doShowNixie(1, 20);
    doShowNixie(1, 21);
    doShowNixie(2, 20);
    doShowNixie(2, 21);
    doShowNixie(3, 11);
    doShowNixie(3, 21);
    doShowNixie(4, 14);
    doShowNixie(4, 21);
    doShowNixie(5, 14);
    doShowNixie(5, 21);
    doShowNixie(6, 22);
    doShowNixie(6, 21);
    doShowNixie(7, 20);
    doShowNixie(7, 21);
    doShowNixie(8, 20);
    doShowNixie(8, 21);
    doBeep(1);
  }
  doClearDisp();
  for (DelayTime = 100; DelayTime > 0; DelayTime--) {
    doShowNixie(1, 20);
    doShowNixie(1, 21);
    doShowNixie(2, 20);
    doShowNixie(2, 21);
    doShowNixie(3, 18);
    doShowNixie(3, 21);
    doShowNixie(4, 14);
    doShowNixie(4, 21);
    doShowNixie(5, 13);
    doShowNixie(5, 21);
    doShowNixie(6, 5);
    doShowNixie(6, 21);
    doShowNixie(7, 20);
    doShowNixie(7, 21);
    doShowNixie(8, 20);
    doShowNixie(8, 21);
  }
  doWaitLED(8);
  doClearDisp();
  for (DelayTime = 8; DelayTime > 0; DelayTime--) {
    Delay80ms();
  }
}

// System Attract Screen
void SystemAttract() {
  int i = 1, DelayTime, KeyOp;
  while (i == 1) {
    for (DelayTime = 150; DelayTime > 0; DelayTime--) {
      doShowNixie(1, 22);
      doShowNixie(1, 21);
      doShowNixie(2, 23);
      doShowNixie(2, 21);
      doShowNixie(3, 24);
      doShowNixie(3, 21);
      doShowNixie(4, 9);
      doShowNixie(4, 21);
      KeyOp = doKeyScan(0);
      if (KeyOp != 0) {
        i = 0;
      }
    }
    for (DelayTime = 150; DelayTime > 0; DelayTime--) {
      doShowNixie(5, 23);
      doShowNixie(5, 21);
      doShowNixie(6, 25);
      doShowNixie(6, 21);
      doShowNixie(7, 14);
      doShowNixie(7, 21);
      doShowNixie(8, 26);
      doShowNixie(8, 21);
      KeyOp = doKeyScan(0);
      if (KeyOp != 0) {
        i = 0;
      }
    }
    for (DelayTime = 50; DelayTime > 0; DelayTime--) {
      doShowNixie(1, 20);
      doShowNixie(1, 21);
      doShowNixie(2, doRandomGen());
      doShowNixie(2, 21);
      doShowNixie(3, doRandomGen());
      doShowNixie(3, 21);
      doShowNixie(4, 20);
      doShowNixie(4, 21);
      doShowNixie(5, 20);
      doShowNixie(5, 21);
      doShowNixie(6, doRandomGen());
      doShowNixie(6, 21);
      doShowNixie(7, doRandomGen());
      doShowNixie(7, 21);
      doShowNixie(8, 20);
      doShowNixie(8, 21);
      KeyOp = doKeyScan(0);
      if (KeyOp != 0) {
        i = 0;
      }
    }
    for (DelayTime = 150; DelayTime > 0; DelayTime--) {
      doShowNixie(1, 25);
      doShowNixie(1, 21);
      doShowNixie(2, 14);
      doShowNixie(2, 21);
      doShowNixie(3, 26);
      doShowNixie(3, 21);
      doShowNixie(4, 20);
      doShowNixie(4, 21);
      doShowNixie(5, 2);
      doShowNixie(5, 21);
      doShowNixie(6, 0);
      doShowNixie(6, 21);
      doShowNixie(7, 2);
      doShowNixie(7, 21);
      doShowNixie(8, 2);
      doShowNixie(8, 21);
      KeyOp = doKeyScan(0);
      if (KeyOp != 0) {
        i = 0;
      }
    }
    KeyOp = doKeyScan(0);
    if (KeyOp != 0) {
      i = 0;
    }
  }
}

// Initialize Error Handler
void doErrorReport(u16 i, u16 j) {
  while (1) {
    doShowNixie(1, 14);
    doShowNixie(1, 21);
    doShowNixie(2, i % 10);
    doShowNixie(2, 21);
    doShowNixie(3, 20);
    doShowNixie(3, 21);
    doShowNixie(4, j % 10);
    doShowNixie(4, 21);
    doBeep(1);
  }
}
