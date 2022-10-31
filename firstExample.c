#include <xc.h>
#include <stdio.h>
#pragma config JTAGEN=OFF
#pragma config FWDTEN=OFF
#pragma config  FNOSC=FRCPLL
#pragma config  FSOSCEN=OFF
#pragma config POSCMOD=EC
#pragma config  OSCIOFNC=ON
#pragma config  FPBDIV=DIV_1
#pragma config FPLLIDIV=DIV_2
#pragma config FPLLMUL=MUL_20
#pragma config FPLLODIV=DIV_1

enum state {
    SW0, SW1, SW2, OFF, STOP
};


void main() {
//    /*1)
//     * int j;
//TRISA &= 0xff00;
//     *  PORTA =0x55;
//     */
//    /* 2)
//     *  int j;
//    TRISA &= 0xff00; 
//    while(1)
//    {
//    PORTA ++; 
//    for(j=0;j<64000;j++);
//    } */
//    /*3)
//     * TRISA &= 0xff00;
//
//    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input 
//    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input 
//    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input 
//    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input 
//    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input 
//    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input 
//    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog 
//    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input 
//    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog 
//    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input 
//    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
//    while(1) {
//    PORTAbits.RA0=PORTFbits.RF3 ; // RF3 (SW0) configured as input 
//    PORTAbits.RA1=PORTFbits.RF5 ; // RF5 (SW1) configured as input 
//    PORTAbits.RA2=PORTFbits.RF4; // RF4 (SW2) configured as input 
//    PORTAbits.RA3=PORTDbits.RD15 ; // RD15 (SW3) configured as input 
//    PORTAbits.RA4=PORTDbits.RD14; // RD14 (SW4) configured as input 
//    PORTAbits.RA5=PORTBbits.RB11 ; // RB11 (SW5) configured as input 
//    PORTAbits.RA6=PORTBbits.RB10 ; // RB10 (SW6) configured as input 
//    PORTAbits.RA7=PORTBbits.RB9 ; // RB9 (SW7) configured as input
//    } */
//    /*
//     * 4)  int j;
//      TRISA &= 0xff00;
//  TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
//  while(1) {
//  if(PORTFbits.RF3) // RF3 (SW0) configured as input 
//      PORTA++;
//  else PORTA--;
//  for(j=0;j<64000;j++);
//  }
//     */
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input 
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input 
    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input 
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input 
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input 
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input 
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog 
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input 
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog 
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input 
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    TRISBbits.TRISB14 = 0;
    ANSELBbits.ANSB14 = 0;

    TRISA &= 0xff00;
    PORTA = 0x00;
    int shift = 0;
    int sw2States[4] = {0x18, 0x24, 0x42, 0x81};

    enum state current;
    while (1) {
       
        if (PORTBbits.RB10) {
            PORTBbits.RB14=0;
            for(int i=0;i<1000;i++);
            PORTBbits.RB14=1;
            for(int i=0;i<1000;i++);
        }
        if (!PORTBbits.RB9) {
            if (!PORTBbits.RB11) {
                if (PORTFbits.RF4) {//sw2
                    current = SW2;
                } else {
                    if (PORTFbits.RF5) {//sw1
                        current = SW1;
                    } else {//sw0
                        if (PORTFbits.RF3)
                            current = SW0;
                        else
                            current = OFF;
                    }

                }
            } else
                current = STOP;
            switch (current) {
                case SW2:
                    if (shift < 0)
                        shift = shift + 4;
                    shift = shift % 4;

                    PORTA = sw2States[shift];
                    if (!PORTDbits.RD15)
                        shift++;
                    else
                        shift--;
                    for (int j = 0; j < 128000 - (PORTDbits.RD14 * 100000); j++);
                    break;
                case SW1:
                    if (shift < 0)
                        shift = 7;
                    if (shift > 7)
                        shift = 0;
                    if (!PORTDbits.RD15) {
                        PORTA = 0x01;
                        PORTA = PORTA << shift;
                        shift++;
                    } else {
                        PORTA = 0x80;
                        PORTA = PORTA>>shift;
                        shift++;
                    }
                    for (int j = 0; j < 128000 - (PORTDbits.RD14 * 100000); j++);
                    break;
                case SW0:
                    if (!PORTDbits.RD15)
                        PORTA++;
                    else
                        PORTA--;
                    for (int j = 0; j < 128000 - (PORTDbits.RD14 * 100000); j++);
                    break;
                case STOP:

                    break;
                case OFF:
                    PORTA = 0x00;
                    shift = 0;
                    break;
            }

        } else {
            PORTA = 0x00;
            current = OFF;
            shift = 0;
        }
    }
}