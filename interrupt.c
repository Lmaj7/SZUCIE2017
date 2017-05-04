#include "./inc/data.h"

void interrupt0action(void){
    unsigned char x;
    extern unsigned char screendata[];
    extern unsigned char flag,com1;
    extern signed   char interr0;
    if(interr0){
    interr0=0;

    TR0=0;
    TL1=TIMER1L;
    TH1=TIMER1H;

    #if ___DISP_MODE==1
    x=flag/16;
    
    if(x>=8){
        screendata[1]=0x80 >> (x-8);
        screendata[0]=0x00;
    } else {
        screendata[1]=0x00;
        screendata[0]=0x80 >> x;
    }
    
    dataDisp();
    #endif
    if(com1)
    flag++;
    TR1=1;
    }

}

void interrupt1action(void){
    extern signed char interr1;
    extern unsigned char com1;

    
    if(interr1){
    interr1=0;
    CONN=0;         //ask for data of next line
    TL0=TIMER0L;
    TH0=TIMER0H;
    if(com1==1){
        scan();
        dataDisp();
        com1=0;
        LIGHT=0xFF;
    }
    CONN=1;
    
    TR0=1;
    TR1=0;

    }

}

void exintAction(void){
    extern unsigned char flag,line;
    extern signed   char exint0,interr1;
    
    if(exint0){
        flag=0;
        interr1=1;
        line=15;
        interrupt1action();
        exint0=0;
    }
}

/*   
    If com1==1, the data recieved will be throw out of the window
*/
void uartAction(void){
    static unsigned char com1cnt=0;
    extern unsigned char screendata[];
    if(RI && (com1==0)){
        LIGHT=0x00;
        screendata[com1cnt]=SBUF;
        com1cnt++;
        if(com1cnt>=2){
            com1cnt=0;
            com1=1;
        }
        RI=0;
    }
}
