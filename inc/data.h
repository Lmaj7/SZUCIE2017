#ifndef __DATA_H
    #define __DATA_H

#include "main.h"

//data used for communication
#define D_HANDSHAKE fuck
#define D_CHECK 0x82

//ports definition
#define ADATA P0
#define LIGHT P1

sbit PWM_CR = CCON^6;

sbit SCK    = P2^0;
sbit RCK    = P2^1;
sbit SI2    = P2^2;
sbit SI1    = P2^3;
sbit CONN   = P2^4;


/*
     =======================================
     prepare all the things that is needed at the very begining moment
     =======================================
*/
void init(void);


/*
     =======================================
     Lmaj7 2017-5-2
     Each time when this function is  called
     the lighten line on the screen will move
     to the next
     =======================================   
*/
void scan(void);


/*
     =======================================
     Lmaj7 2017-5-2
     This function is used to transform the 16-bit parallel data into
     2 serial ones. Two 74HC595s would recieve the data at the same time,
     and then transform the serial data back to the parallel format.
     =======================================
*/
void dataDisp(void);

void interrupt0action(void);
void interrupt1action(void);
void uartAction(void);
void exintAction(void);

/*   a short delay   */
void canYouBeFaster(void);
void yesICan(unsigned int x);

void sendData(char x);
void shakehand(void);
#endif
