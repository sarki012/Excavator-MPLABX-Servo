/*
 * File:   servo_main.c
 * Author: Erik
 *
 * Created on February 12, 2022, 8:17 AM
 */




// DSPIC33EP32MC202 Configuration Bit Settings

// 'C' source line config statements

// FICD

//#define FCY 3685000

#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC3 and PGED3)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF             // Alternate I2C1 pins (I2C1 mapped to ASDA1/ASCL1 pins)
#pragma config ALTI2C2 = OFF             // Alternate I2C2 pins (I2C2 mapped to ASDA2/ASCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF              // Watchdog Timer Enable bit (Watchdog timer always enabled)

// FOSC
#pragma config POSCMD = NONE           // Primary Oscillator disabled
#pragma config OSCIOFNC = ON         // OSC2 is general purpose digital I/O pin
#pragma config IOL1WAY = OFF             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FOSCSEL
#pragma config FNOSC = FRC          // Internal Fast RC (FRC) Oscillator 7.37 MHz
#pragma config PWMLOCK = OFF            // PWM Lock Enable bit (Certain PWM registers may only be written after key sequence)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <p33ep32mc202.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void);
int x = 0;
char rxval[20];
int curl = 0, boom = 0;
void servo_init(void);
int char_to_int(char digit2, char digit1, char digit0);
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)             
{
    IFS1bits.U2RXIF = 0;
    rxval[x] = U2RXREG;
    x++;
    if(x == 20)
    {  
        x = 0;
    }
    return;
 }
void main(void) {
    int i = 0, j = 0, k = 0, count = 0;
    servo_init();
    for(i = 0; i < 20; i++)
    {
        rxval[i] = 0;
    }
    /*
    PHASE1 = 2303;
    PDC1 = 92;
    PHASE2 = 2303;
    PDC2 = 253;
    PHASE3 = 2303;
    PDC3 = 173;
     */
    //PHASE2 = 2303;
    while(1)
    {
        for(i = 0; i < 15; i++)
        {
            if(rxval[i] == 'c')
            {
                curl = char_to_int(rxval[i+1], rxval[i+2], rxval[i+3]);
                //Motor Arithmitic Here
                //PHASE3 and PDC3 are for PWM3L, the bucket curl motor
                //PHASE is always 2303 to give a rising edge every 20ms
                //Max Duty Cycle is PDC = 253
                //Neutral Duty Cycle is 173
                //Min Duty Cycle is PDC = 92
                PHASE3 = 2303;
                PDC3 = (int)(172.5 + curl);
                count++;
                if(count == 2)
                {
                    count = 0;
                    break;
                }
         //       for(j = 0; j < 10000; j++);
               // for(j = 0; j < 5000; j++);
            }
            else if(rxval[i] == 'b')
            {
                boom = char_to_int(rxval[i+1], rxval[i+2], rxval[i+3]);
                //Motor Arithemetic Here
                //PHASE3 and PDC3 are for PWM3L, the bucket curl motor
                //PHASE is always 2303 to give a rising edge every 20ms
                //Max Duty Cycle is PDC = 253
                //Neutral Duty Cycle is 173
                //Min Duty Cycle is PDC = 92
                PHASE1 = 2303;
                PDC1 = (int)(172.5 + boom);
                count++;
                if(count == 2)
                {
                    count = 0;
                    break;
                }
            }
        }
        
    }
    return;
}
            /*
            if(rxval[i] == 'U')
            {
                PHASE1 = 2303;
                PDC1 = 253;
            }
            else if(rxval[i] == 'D')
            {
                PHASE1 = 2303;
                PDC1 = 92;
            }
            if(rxval[i] == 'R')
            {
                PHASE2 = 2303;
                PDC2 = 253;
            }
            else if(rxval[i] == 'L')
            {
                PHASE2 = 2303;
                PDC2 = 92;
            }
        }
             * */
        /*
        for(i = 0; i < 100; i++)
        {
            U1TXREG = rxval[i];
            while(!U1STAbits.TRMT);
        }
         * */
        /*
        PDC2 = 92;
        for(i = 0; i < 10000; i++)
        {
            for(j = 0; j < 100; j++);
        }
        PDC2 = 253;
        for(i = 0; i < 10000; i++)
        {
            for(j = 0; j < 100; j++);
        }
         */
         /*
        U1TXREG = 'E';
        while(!U1STAbits.TRMT);
        U1TXREG = 'R';
        while(!U1STAbits.TRMT);
        U1TXREG = 'I';
        while(!U1STAbits.TRMT);
        U1TXREG = 'K';
        while(!U1STAbits.TRMT);
        U1TXREG = ' ';
        while(!U1STAbits.TRMT);
        U1TXREG = 'I';
        while(!U1STAbits.TRMT);
        U1TXREG = 'S';
        while(!U1STAbits.TRMT);
        U1TXREG = ' ';
        while(!U1STAbits.TRMT);
        U1TXREG = 'T';
        while(!U1STAbits.TRMT);
        U1TXREG = 'H';
        while(!U1STAbits.TRMT);
        U1TXREG = 'E';
        while(!U1STAbits.TRMT);
        U1TXREG = ' ';
        while(!U1STAbits.TRMT);
        U1TXREG = 'B';
        while(!U1STAbits.TRMT);
        U1TXREG = 'E';
        while(!U1STAbits.TRMT);
        U1TXREG = 'S';
        while(!U1STAbits.TRMT);
        U1TXREG = 'T';
        while(!U1STAbits.TRMT);
        U1TXREG = ' ';
        while(!U1STAbits.TRMT);
        */
  
