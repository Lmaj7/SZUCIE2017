#ifndef __definitions
    #define __definitions
    
#include<reg52.h>
#include<intrins.h>


sfr AUXR    = 0x8E;//to set timer mode

sfr CMOD    = 0xD9;//pwm
sfr CCON    = 0xD8;
sfr CCAPM0  = 0xDA;
sfr PWM_CL  = 0xE9;
sfr BRT     = 0x9C;

extern unsigned char screendata[];
extern unsigned char flag,com1,line;
extern signed   char uartConn;
extern signed   char interr1,interr0,exint;

#define TIMER0L 0x86
#define TIMER0H 0xFF
#define TIMER1L 0x80
#define TIMER1H 0xFF

#define ___DEBUG
#define ___DISP_MODE 1
/*      0: without a pen and another MCU
        1: with a pen and another MCU
        2: without a pen but another MCU
*/
    
#endif
