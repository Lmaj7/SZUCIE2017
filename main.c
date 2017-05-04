#include "./inc/data.h"
/*
    ============================
    --  Lmaj7 2017-5-4 10:40 a.m.
    
    This project is made for the Electronic Design Competition
    hold by the College of Information Engineering of Shenzhen
    University. The MCU used is STC11F32XE or STC12C5A60S2. The project use 
    2 MCU, one to process the data users input, and the other
    to push the data to the LED array screen.
    
    ============================
*/

void main(){
    init();
    while(1){
        interrupt0action();
        interrupt1action();
        exintAction();
    }
}

void t0() interrupt 1{
    extern signed char interr0;
    interr0=1;
}

void t1() interrupt 3{
    extern signed char interr1;
    interr1=1;
}

void exintfunc() interrupt 0{
    extern signed char exint0;
    exint0=1;
}

void uart0() interrupt 4{
    uartAction();
}