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


extern volatile unsigned int DB1;
extern volatile unsigned int DB2;
 volatile unsigned int DB1A = 0;
 volatile unsigned int DB2A = 0;
extern volatile unsigned int countD;
extern volatile unsigned int backlight;
extern volatile unsigned char update_display;
unsigned volatile int LCD_interupt_count = 0;
unsigned volatile int ADC_interupt_count = 0;
unsigned volatile int time_counting = 0;
unsigned volatile int seconds = 0;
unsigned volatile int my_seconds = 0;
unsigned volatile int milliseconds = 0;
unsigned volatile int Command_Pressed = 0;

unsigned int Start_I = 1;
unsigned int TimeStart = 0;
//update every 5 milliseconds

unsigned volatile int IOT_step_counter = 0;
extern unsigned volatile int run_bootup;
extern unsigned volatile int IOT_step;
extern unsigned volatile int step_temp;
unsigned volatile int run_timer = 0;




void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK; // SMCLK source
    TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS; // Continuous up
    TB0CTL |= ID__2; // Divide clock by 2
    TB0EX0 = TBIDEX__8; // Divide clock by an additional 8
    TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
    TB0CCTL0 |= CCIE; // CCR0 enable interrupt

//     TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
//     TB0CCTL1 &= ~CCIE; // CCR1 enable interrupt
//
//     TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
//     TB0CCTL2 &= ~CCIE; // CCR2 enable interrupt

    TB0CTL &= ~TBIE; // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}
//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR // Timer B0 – 0 Capture compare
__interrupt void Timer0_B0_ISR(void){
//--------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//--------------------------------------------------------------------
    ADCCTL0 |= ADCENC; //run ADC every 20 ms
    ADCCTL0 |= ADCSC;
    update_display = TRUE;
    time_counting++;

    if (countD == 1){
        update_display = 1;
        countD = 0;

    }else{
        countD++;
    }
    if(DB1 == 0 && DB2 == 0){
        if(backlight == 2){
            //P6OUT |= LCD_BACKLITE;
            backlight = 0;
        }else{
            backlight++;
            //P6OUT &= ~LCD_BACKLITE;
        }
    }

    if(milliseconds < 10){
        milliseconds += 1;
    }else{
        seconds++;
        milliseconds = 0;

        IOT_step_counter++;

        if(IOT_step_counter > 10 && run_bootup == 1 && step_temp < 6){
            IOT_step_counter = 0;
            IOT_step = step_temp++;

        }

        if(Command_Pressed == 1){
            run_timer++;
            if(run_timer > 1){
                run_timer = 0;
                Command_Pressed = 0;
            }
        }

    }
    TB0CCR0 += TB0CCR0_INTERVAL;

//--------------------------------------------------------------------
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
    case 0: break; // No interrupt
    case 2: // CCR1 not used
    //...... Add What you need happen in the interrupt ......

        if(DB1A == 8){
            TB0CCTL1 &= ~CCIE;
            P4IFG &= ~SW1;
            P4IE |= SW1;
            DB1 = 0;
            DB1A = 0;
        }else{
            DB1A++;
        }
        TB0CCR1 += TB0CCR1_INTERVAL; // Add Offset to TBCCR1
        break;
    case 4: // CCR2 not used
    //...... Add What you need happen in the interrupt ......
        if(DB2A == 8){
            TB0CCTL2 &= ~CCIE;
            P2IFG &= ~SW2;
            P2IE |= SW2;
            DB2 = 0;
            DB2A = 0;
        }else{
            DB2A++;
        }

         TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR1
        break;
    case 14: // overflow
    //...... Add What you need happen in the interrupt ......
    break;
    default: break;
}
//--------
}

void Init_Timer_B3(void) {
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.0 LCD_BACKLITE
// TB3.2 P6.1 R_FORWARD
// TB3.3 P6.2 R_REVERSE
// TB3.4 P6.3 L_FORWARD
// TB3.5 P6.4 L_REVERSE
//------------------------------------------------------------------------------
     TB3CTL = TBSSEL__SMCLK; // SMCLK
     TB3CTL |= MC__UP; // Up Mode
     TB3CTL |= TBCLR; // Clear TAR
     PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]
     TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
     LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM duty cycle
     TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
     RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
     TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
     LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
     TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
     RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
     TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
     LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
//------------------------------------------------------------------------------
}
