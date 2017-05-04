#include "./inc/data.h"

/*
     =======================================
     prepare all the things that is needed at the very first moment
     =======================================
*/
void init(void){

    extern unsigned char flag,screendata[];
    unsigned char i;
    
    LIGHT=0x00;
    TR0=0;                   //timer dont start

    /*   wait for steadiness   */
    for(i=0;i<64;i++)
        canYouBeFaster();

    /*   Init for 74hc595 
         Clear all bits of the line   */
    RCK     = 0;
    SCK     = 0;
    SI1     = 0;
    SI2     = 0;
    ADATA   = 0x00;
    for(i=0;i<8;i++){
        SCK=0;
        SI1=0;SI2=0;
        SCK=1;
    }
    RCK= 1;
    RCK= 0;
    
    /*   Init for vars   */
    //screendata[0]   = 0xAA;
    //screendata[1]   = 0x55;//data clear
    flag            = 0;
    
    /*   Init for interrupts   */
    //   timers' registers' settings
    IE      = 0x9B;          //enable timer0 n timer1, Serial COM1 interrupt, EX0
    TMOD    = 0x11;          //set timers to run under 16-bit mode
    AUXR    = AUXR&0x7F;     //set timer0 to run under 12T mode
    AUXR    = AUXR|0x40;     //set timer1 to run under 1T mode
    //   AUXR is definded in file "./inc/main.h"

    //   timers' settings
    TL0=TIMER0L;
    TH0=TIMER0H;             //per 1032T (12*86)
    TL1=TIMER1L;
    TH1=TIMER1H;             //per 256T  (1*256)
    
    //   PWM's settings
    CMOD    = 0x00;          //set PWM work under sysclk/12
    CCON    = 0x00;
    CCAPM0  = 0x42;          //enable PWM
    
    //Serial
    PCON    = PCON&0x7F;
    SCON    = 0x50;
    AUXR    = AUXR|0x04;
    BRT     = 0xDC;
    AUXR    = AUXR|0x11;
    
    
    //   external interrupt's settings
    IT0=1;
    EX0=1;
    
    shakehand();

}


/*
     =======================================
     Lmaj7 2017-5-2
     Each time when this function is  called
     the lighten line on the screen will move
     to the next
     =======================================   
*/
void scan(void){
    extern unsigned char line;
    if(line>=16)
        line=15;
    ADATA=line;
    line--;
}

/*
     =======================================
     Lmaj7 2017-5-2
     This function is used to transform the 16-bit parallel data
     into 2 serial ones. Two 74HC595 would recieve the data at the
     same time, and then transform the serial data back to the parallel format.
     =======================================
*/
void dataDisp(void){
    unsigned char i;
    extern unsigned char screendata[];
    unsigned char temp[2];

    temp[0]=screendata[0];
    temp[1]=screendata[1];

    RCK=0;
    for(i=0;i<8;i++){
        SCK=0;
        if( (temp[1]&0x80) == 0x80){
            SI1=1;
        } else {
            SI1=0;
        }
        if( (temp[0]&0x80) == 0x80){
            SI2=1;
        } else {
            SI2=0;
        }
        SCK=1;
        temp[0]=temp[0]<<1;
        temp[1]=temp[1]<<1;
    }
    RCK=1;
}


/*   a short delay   */
void canYouBeFaster(void){
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

/*   a longer delay, about 1ms   */
void yesICan(unsigned int x){
    unsigned char a,b;
    for(;x>0;x--){
        for(b=18;b>0;b--)
            for(a=152;a>0;a--);
        _nop_();
    }
}

void sendData(char x){
    SBUF=x;
    while(TI==0);
    TI=0;
}

/*
    ============================
    Lmaj7 2017-5-4
    This function is used to make sure that the MCU is
    successfully connected to the upper MCU.
    ============================
*/
void shakehand(void){
    extern signed   char uartConn;
    extern unsigned char screendata[],com1;
    
    CONN=1;
    while(uartConn==0){                             //wait until the connection is success
        
        while(com1==0){                             //wait for response, if no response, keep asking
            LIGHT=0x00;
            sendData('f');
            sendData('u');
            yesICan(150);
            sendData('c');
            sendData('k');
            LIGHT=0xff;
            yesICan(2);
        }
        
        if(screendata[0]=='f'&&screendata[1]=='u'){ //the first response come, check if it is correct
            LIGHT=0x00;
            com1=0;
            while(com1==0);                         //wait for the next response
            if(screendata[0]=='c'&&screendata[1]=='k'){
                LIGHT=0xff;
                uartConn=1;
            }
        }
        com1=0;
    }

    CONN=0;                                         //send the ready signal
    while(com1==0){                                 //wait for the first data
        yesICan(50);
        LIGHT=0xff;
        yesICan(2);
        LIGHT=0x00;
    }
    TR1=1;
}
