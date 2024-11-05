//------------------------------------------------------------------------------
//
//  Description: Timers file to create an interupt for the project 5
//
//  Jim Carlson
//  Jan 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

//update every 5 milliseconds

#define FIFTY_MS_COUNT 10

extern volatile unsigned int DB1;
extern volatile unsigned int DB2;
extern volatile unsigned int countD;
extern volatile unsigned char backlight;

unsigned int time_counter = 0;
extern unsigned check_start;
volatile unsigned int Time_Sequence;
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
unsigned int blink_count = 0;
unsigned int one_time = 0;
extern unsigned volatile int state;
extern unsigned volatile int undetected;

unsigned volatile int START_RUNNING = 0;
unsigned volatile int baudswitch_update = 0;
unsigned volatile int start_transmit = 0;
extern unsigned volatile int transmit_received;

#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
// Switch 1
    if (P4IFG & SW1) {
        //strcpy(display_line[3], " Switch 1  ");
        state = 1;
        display_changed = 1;
        update_display = 1;

        P4IFG &= ~SW1; // IFG SW1 cleared
        P4IE &= ~SW1;

        TB0CCR1 = TB0R + TB0CCR1_INTERVAL;
        TB0CCTL1 |= CCIE;



        START_RUNNING = 1;
        DB1 = 1;

        baudswitch_update = 1;



    }
}
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
    // Switch 2
    if (P2IFG & SW2) {
        //strcpy(display_line[3], " Switch 2 ");
        state = 1;
        display_changed = 1;
        update_display = 1;

        P2IFG &= ~SW2; // IFG SW1 cleared
        P2IE &= ~SW2;

        TB0CCR2 = TB0R + TB0CCR2_INTERVAL;
        TB0CCTL2 |= CCIE;


        START_RUNNING = 1;
        DB2 = 1;

        start_transmit = 1;

    }
}





//#pragma vector = TIMER0_B0_VECTOR
//__interrupt void Timer0_B0_ISR(void){
//    //------------------------------------------------------------------------------
//    // TimerB0 0 Interrupt handler
//    //----------------------------------------------------------------------------
//    //...... Add What you need happen in the interrupt ......
//    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
//    Time_Sequence += 1;
//
//    if(Time_Sequence % 40 == 0){
//        update_display = TRUE;
//        Time_Sequence = 0;
//        if(check_start == 1){
//            time_counter += 1;
//        }
//    }
//}




