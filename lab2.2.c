/* 
 * File:   lcd_fader.c
 * Author: MenachemE
 *
 * Created on July 11, 2019, 5:49 PM
 */

#include <xc.h>
#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	EC
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1
#include <stdio.h>
#include <stdlib.h>
void busy(void);



void main(void)
 {
int counter=0;

    int j, i;
    int line[2] = {0x80, 0xc0};
    char CG_father[24] = {0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, //?
        0x18, 0x0c, 0x06, 0x1f, 0x1f, 0x06, 0x0c, 0x18, //?
        0x11, 0x0a, 0x0e, 0x0a, 0x0a, 0x0e, 0x0a, 0x11}; //?
    char father[7] = {0, 0, 0, 0, 1, 2, 2}; // ???
    char control[7] = {0x38, 0x38, 0x38, 0xe, 0x6, 0x1, 0x40};
    // set CGRAM=0x40
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    TRISE &= 0xff00;
    TRISBbits.TRISB14 = 0;
    ANSELBbits.ANSB14 = 0;
    ANSELEbits.ANSE2 = 0;
    ANSELEbits.ANSE4 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;
    ANSELEbits.ANSE7 = 0;
     TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input 
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog 
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input 
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    PORTBbits.RB15 = 0; //rs=0
    PORTDbits.RD5 = 0; //w=0
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    for (i = 0; i < 7; i++) {
        PORTE = control[i];
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
    PORTBbits.RB15 = 1; //rs
    for (i = 0; i < 24; i++) {
        PORTE = CG_father[i];
        PORTDbits.RD4 = 1; //enable=1
        PORTDbits.RD4 = 0; //enable=0
        busy();
    }
    PORTBbits.RB15 = 0; //rs control 
    PORTE = line[0]; //DDRAM
    PORTDbits.RD4 = 1; //enable=1
    PORTDbits.RD4 = 0; //enable=0
    //for(j=0;j<32000;j++);
    busy();
    PORTBbits.RB15 = 1; //rs 

    for (i = 0; i < 7; i++) {
        PORTE = father[i];
        PORTDbits.RD4 = 1; //enable=1
        PORTDbits.RD4 = 0; //enable=0
        busy();
    }
    TRISBbits.TRISB14 = 0;
    ANSELBbits.ANSB14 = 0;
    while (1) {
             if (PORTBbits.RB10) {
            PORTBbits.RB14 = 0;
            for (int i = 0; i < 1000; i++);
            PORTBbits.RB14 = 1;
            for (int i = 0; i < 1000; i++);
        }
        counter++;
        counter=counter%16;
        if (PORTBbits.RB10) {
            PORTBbits.RB14 = 0;
            for (int i = 0; i < 1000; i++);
            PORTBbits.RB14 = 1;
            for (int i = 0; i < 1000; i++);
        }
        if (PORTFbits.RF5&&counter==0) {
            
                        PORTBbits.RB15 = 0; //rs control 

                   PORTE = 0x01; //DDRAM
              PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();
            PORTE = line[0]; //DDRAM
              PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();

            PORTBbits.RB15 = 1; //rs 

            for (i = 0; i < 7; i++) {
                PORTE = father[i];
                PORTDbits.RD4 = 1; //enable=1
                PORTDbits.RD4 = 0; //enable=0
                busy();
            }
            if(!PORTFbits.RF3)
            {
                for(int x=0;x<10;x++)
                {
                     PORTBbits.RB15 = 0; //rs control 
                    PORTE = 0x1c;
                     PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();
                }
            }
        } else if(counter==0) {
                        PORTBbits.RB15 = 0; //rs control 

                           PORTE = 0x01; //DDRAM
              PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();
            PORTBbits.RB15 = 0; //rs control 
            PORTE = line[1]; //DDRAM
              PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();

            PORTBbits.RB15 = 1; //rs 

            for (i = 0; i < 7; i++) {
                PORTE = father[i];
                PORTDbits.RD4 = 1; //enable=1
                PORTDbits.RD4 = 0; //enable=0
                busy();
            }
                   if(!PORTFbits.RF3)
            {
                for(int x=0;x<10;x++)
                {
                     PORTBbits.RB15 = 0; //rs control 
                    PORTE = 0x1c;
                     PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();
                }
            }
        }
        //shift
        PORTBbits.RB15 = 0; //rs control 
 if (PORTFbits.RF3)
                   PORTE = 0x1c; //DDRAM
 else
      PORTE = 0x18;
              PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();
        for (int j = 0; j < 128000; j++);
  
    }
}

void busy(void) {
    char RD, RS;
    int STATUS_TRISE;
    int portMap;
    RD = PORTDbits.RD5;
    RS = PORTBbits.RB15;
    STATUS_TRISE = TRISE;
    PORTDbits.RD5 = 1; //w/r
    PORTBbits.RB15 = 0; //rs 
    portMap = TRISE;
    portMap |= 0x80;
    TRISE = portMap;
    do {
        PORTDbits.RD4 = 1; //enable=1
        PORTDbits.RD4 = 0; //enable=0
    } while (PORTEbits.RE7); // BF ?????
    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}

