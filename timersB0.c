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

unsigned int time_counter = 0;
extern unsigned check_start;
volatile unsigned int Time_Sequence;
extern volatile unsigned char update_display;
extern char display_line[4][11];

void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS; // Continuous up
    TB0CTL |= ID__2; // Divide clock by 2
    TB0EX0 = TBIDEX__8; // Divide clock by an additional 8
    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt
    // TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
    // TB0CCTL1 |= CCIE; // CCR1 enable interrupt
    // TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
    // TB0CCTL2 |= CCIE; // CCR2 enable interrupt
    TB0CTL &= ~TBIE; // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    //------------------------------------------------------------------------------
    // TimerB0 0 Interrupt handler
    //----------------------------------------------------------------------------
    //...... Add What you need happen in the interrupt ......
    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
    Time_Sequence += 1;

    if(Time_Sequence % 40 == 0){
        update_display = TRUE;
        Time_Sequence = 0;
        if(check_start == 1){
            time_counter += 1;
        }
    }
}




