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

extern unsigned int time_counter;
unsigned int timing_temp;
unsigned int check_start;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

unsigned volatile int state = 0;
unsigned volatile int blackVal = 400;
unsigned volatile int RIGHT_FORWARD_SPEED = 0;
unsigned volatile int LEFT_FORWARD_SPEED = 0;
extern unsigned volatile int START_RUNNING;

extern volatile unsigned int ADC_Right_Det;
extern volatile unsigned int ADC_Left_Det;
extern unsigned volatile int seconds;

void proj6_StateMachine(void){
    switch(state){
        case 0:
            P2OUT &= ~IR_LED; //set emitter to off
            strcpy(display_line[3], " waiting ");
            seconds = 0;
            break;
        case 1:
            P2OUT |= IR_LED;//set emitter to on
            strcpy(display_line[3], "  forward ");
            update_display = TRUE;
            if(seconds >= 1){
                forward_on();
//                goforward();
                if(START_RUNNING && ADC_Right_Det >= blackVal || ADC_Left_Det >= blackVal){
//                if(seconds >= 10){
                    stop();
                    //off();
                    state = 2;
                    seconds = 0;

                }
            }
            break;
        case 2:
            strcpy(display_line[3], " turning  ");
            update_display = TRUE;
            if(seconds >= 1){
                //cw();
                turn();
                if(START_RUNNING && ADC_Right_Det >= blackVal && ADC_Left_Det >= blackVal){
//                if(seconds >= 2){
                    state = 3;
                    seconds = 0;

                }
            }
            break;
        case 3:
            stop();
//            off();
            strcpy(display_line[3], " stopped  ");
            state = 0;
            break;
        default: break;
    }
}


