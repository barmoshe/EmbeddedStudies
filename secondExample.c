#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF
#pragma config FNOSC = FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_1
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

void busy(void);
void firstLine(char str[]);
int secondLine(char str[]);
void shiftText(char msg[], int delay, int step);
void data();



void busy(void) 
{
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
        PORTDbits.RD4 = 1; //enable=1        PORTDbits.RD4 = 0; //enable=0
    } while (PORTEbits.RE7); // BF ?????

    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}

void data()
{
    PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();
}

void firstLine(char line1[]) 
{
    int size = strlen(line1);
    //PORTDbits.RD5 = 0; //w=0
    PORTBbits.RB15 = 0; //rs=0
    PORTE = 0x80;
    data();
    PORTBbits.RB15 = 1; //rs=1
    for (int i = 0; i < size; i++) {
        PORTE = line1[i];
        data();
    }
}

int secondLine(char line2[]) 
{
    
    PORTBbits.RB15 =0; //rs=0 control
    PORTE = 0xc0;
    data();
    PORTBbits.RB15 = 1; //rs=1 ascii
    for (int i = 0; i < 16; i++) {
        PORTE = line2[i];
        data();
    }
}

void shiftText(char msg[], int delay, int step)
{
    int size = (int)strlen(msg);
    int count1 = 0, count2 = 0;
    char st1[size - step];
    char st2[size];
    
    for(int i = 0; i < size - step; i++)
        st1[i] = msg[i];
    for(int i = 0; i < step; i++)
        st2[i] = msg[size - step + i];
    for(int i = 0; i < size; i++){
        if(i < step)
            msg[i] = st2[count2++];
        else
            msg[i] = st1[count1++];        
    }
    secondLine(msg);
    for(int i = 0; i < delay; i++); 
}
void main() {
    
    int j = 1;
    TRISA &= 0xff00;
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


    char control[] = {0x38, 0x38, 0x38, 0xe, 0x6, 0x1};
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    //TRISEbits.TRISE0 = 1; // RE0 (DB0) set as input (change 1 to 0 for
    TRISBbits.TRISB14 = 0; // RB14 configured as output
    ANSELBbits.ANSB14 = 0; // RB14 (SW7) disabled analog
    TRISE &= 0xff00;
    ANSELEbits.ANSE2 = 0;
    ANSELEbits.ANSE4 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;
    ANSELEbits.ANSE7 = 0;
    


    //////Control////////////
    PORTBbits.RB15 = 0; //rs=0
    PORTDbits.RD5 = 0; //w=0
    for (int i = 0; i < 6; i++)
    {
        PORTE = control[i];
        data();
    }
    //////Control///////////

    ///////////////////////////////LCD//////////////////LCD//////////////////////LCD////////// 
    int delay;
    int res0 = 0x01, res1 = 0x1, res2a = 0x10, res2b = 0x08;
    char str1[8];
    char str2[13];
    char str3[7];
    char str4[18];
    int step = 0;
    while (1) {
        
        
        if (PORTBbits.RB11){ // SW5            
            strcpy(str1, "Mode 5:");
            firstLine(str1);
            strcpy(str4, "      Halt      ");
            shiftText(str4, delay, step++%(int)(strlen(str4)));
            continue;
        }
        
        if(PORTBbits.RB9){          // SW7
            strcpy(str1, "Mode 7:");
            firstLine(str1);
            strcpy(str4, "      Exit      ");
            shiftText(str4, delay, step++%(int)(strlen(str4)));
            res0 = 0x01;
            res1 = 0x1;
            res2a = 0x10;
            res2b = 0x08;
            PORTA = 0;
            continue;
        }
        
        ///////////////////////////////////////////////////////////////
        if (PORTDbits.RD14){ // SW4
            delay = 96000;
            strcpy(str3," Fast ");
        }else{
            delay = 192000;
            strcpy(str3," Slow ");
        }
        //////////////////////////////////////////////////////////////////////////// /////////////////
        
        if(PORTBbits.RB10){          // SW6
            strcpy(str1, "Mode 6:");
            firstLine(str1);
            strcpy(str4, "   Beep Mode    ");
            shiftText(str4, delay, step++%(int)(strlen(str4)));            
            PORTBbits.RB14 = 1;
            for(int i = 0; i <= delay/3000; i++);
            PORTBbits.RB14 = 0;
            //for(int i = 0; i <= delay/3000; i++);
            continue;
        }
        

        
        if(PORTFbits.RF3)                //SW0
        {     
            strcpy(str1,"Mode 0:");
            firstLine(str1);
            if(PORTDbits.RD15){         //SW3
                strcpy(str2,"Counter Down");
                strcat(str2, str3);
                shiftText(str2, delay, step++%(int)(strlen(str2) + strlen(str3)));
                if (res0 == 0x00)
                    res0 = 0xff;
                PORTA = res0--;
            }else{
                strcpy(str2,"Counter Up");
                strcat(str2, str3);
                shiftText(str2, delay, step++%(int)(strlen(str2) + strlen(str3)));
                if (res0 == 0xff)
                    res0 = 0;
                PORTA = res0++; 
            }
        }
       //////////////////////////////////////////////////////////////////////////////////////////////////
        if (PORTFbits.RF5) {            // SW1
            strcpy(str1, "Mode 1:");
            firstLine(str1);
            if(PORTDbits.RD15){         //SW3
                strcpy(str2, "Shift Left");
                strcat(str2, str3);
                shiftText(str2, delay, step++%(int)(strlen(str2) + strlen(str3)));
                if (res1 == 0x100)
                    res1 = 0x01;
                PORTA = res1;
                res1 = res1 << 1; 
            }else{    
                strcpy(str2, "Shift Right");
                strcat(str2, str3);
                shiftText(str2, delay, step++%(int)(strlen(str2) + strlen(str3)));
                if (res1 == 0x00)
                    res1 = 0x80;
                PORTA = res1;
                res1 = res1 >> 1;
            }
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////
        if (PORTFbits.RF4) {            //SW2
            strcpy(str1, "Mode 2:");
            firstLine(str1);            
            if (PORTDbits.RD15) {       //SW3
                strcpy(str2, " Swing Up ");
                strcat(str2, str3);
                shiftText(str2, delay, step++%(int)(strlen(str2) + strlen(str3)));
                if (res2a < res2b) {
                    res2a = 0x80;
                    res2b = 0x01;
                }
                PORTA = res2a | res2b;
                res2a = res2a >> 1;
                res2b = res2b << 1;
            } else {
                strcpy(str2, "Swing Down");
                strcat(str2, str3);
                shiftText(str2, delay, step++%(int)(strlen(str2) + strlen(str3)));
                if (res2a == 0x100 && res2b == 0x00) {
                    res2a = 0x10;
                    res2b = 0x08;
                }
                PORTA = res2a | res2b;
                res2a = res2a << 1;
                res2b = res2b >> 1;
            }
        }
        ///////////////////////////////////////////////////////////////
        for (j = 0; j < delay; j++); // delay
    }
}