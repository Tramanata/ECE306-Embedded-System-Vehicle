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
unsigned volatile int blackVal_turn = 150;
unsigned volatile int blackVal = 100; //was 150
unsigned volatile int RIGHT_FORWARD_SPEED = 0;
unsigned volatile int LEFT_FORWARD_SPEED = 0;
extern unsigned volatile int START_RUNNING;
unsigned volatile int display_value = 0;

extern volatile unsigned int ADC_Right_Det;
extern volatile unsigned int ADC_Left_Det;
extern unsigned volatile int seconds;
extern unsigned volatile int milliseconds;
unsigned volatile int move_value;

extern unsigned volatile int bl_timer;
extern unsigned volatile int blackline_timer;

volatile int difference;
volatile int difference_speed;

void proj7_StateMachine(void){
    switch(state){
        case 0:
            P2OUT &= ~IR_LED; //set emitter to off
            //strcpy(display_line[3], "w         ");
            seconds = 0;
            milliseconds = 0;
            break;
        case 1:
            P2OUT |= IR_LED;//set emitter to on
            //strcpy(display_line[3], "f         ");
            update_display = TRUE;
            if(seconds >= 1){
                //forward_on();
                forward_slow();
//                goforward();
                if(START_RUNNING && ADC_Right_Det >= blackVal_turn || ADC_Left_Det >= blackVal_turn){
//                if(seconds >= 10){
                    stop();
                    //off();
                    state = 2;
                    seconds = 0;
                    milliseconds = 0;

                }
            }
            break;
        case 2:
            //strcpy(display_line[3], "t         ");
            update_display = TRUE;
            if(seconds >= 1){

                //cw();
                turn();
                if(START_RUNNING && ADC_Right_Det >= blackVal_turn & ADC_Left_Det >= blackVal_turn){
//                if(seconds >= 2){
                    state = 3;
                    seconds = 0;
                    milliseconds = 0;

                }
            }
            break;
        case 3:
            display_value = 1;
            if(seconds < 80){
                blackline_timer = 1;
                if(ADC_Right_Det >= blackVal && ADC_Left_Det >= blackVal){
                    stop();
                    forward_circle();

                    if(bl_timer > 1){
                        stop();
                        blackline_timer = 0;
                        bl_timer = 0;
                    }


                }else if(ADC_Right_Det >= blackVal && ADC_Left_Det < blackVal){
                    //slight_left();
                    stop();
                    turn_ccw();

                    if(bl_timer > 1){
                        stop();
                        blackline_timer = 0;
                        bl_timer = 0;
                    }

                    move_value = 1;
                }else if(ADC_Right_Det < blackVal && ADC_Left_Det >= blackVal){
                    //slight_riht();
                    stop();
                    turn_cw();

                    if(bl_timer > 1){
                        stop();
                        blackline_timer = 0;
                        bl_timer = 0;
                    }

                    move_value = 2;
                }else{
                    stop();
                    reverse_on();

                }

            }else{
                stop();
                blackline_timer = 0;
                bl_timer = 0;
                state = 5;
                seconds = 0;
            }
            break;
        case 5:
            blackline_timer = 1;
            //turn_ccw();
            RIGHT_REVERSE_SPEED = PERCENT_40; //40
            LEFT_FORWARD_SPEED = PERCENT_30;
            if(bl_timer > 12){
                stop();
                blackline_timer = 0;
                bl_timer = 0;
                state = 6;
            }
            //state = 6;
            break;
        case 6:
            blackline_timer = 1;
            forward_slow();
            if(bl_timer > 22){
                stop();
                blackline_timer = 0;
                bl_timer = 0;
                state = 0;
            }
            break;
        default: break;
    }
}


