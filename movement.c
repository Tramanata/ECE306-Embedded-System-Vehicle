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


void forward_on(void){

    RIGHT_FORWARD_SPEED = PERCENT_35;
    LEFT_FORWARD_SPEED = PERCENT_35;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    strcpy(display_line[3], "forward on");
}

void reverse_on(void){

    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = PERCENT_35;
    LEFT_REVERSE_SPEED = PERCENT_35;
    strcpy(display_line[3], "reverse on");
}

void forward_slow(void){
    RIGHT_FORWARD_SPEED = PERCENT_20;
    LEFT_FORWARD_SPEED = PERCENT_20;


    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    //strcpy(display_line[3], "forwardslo");

}

void forward_circle(void){
    RIGHT_FORWARD_SPEED = PERCENT_20; //switch to 20
    LEFT_FORWARD_SPEED = PERCENT_20; //switch to 20

//    RIGHT_FORWARD_SPEED = PERCENT_60;
//    LEFT_FORWARD_SPEED = PERCENT_40;

    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    //strcpy(display_line[3], "forwardslo");

}


void reverse_slow(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = PERCENT_15; //30
    LEFT_REVERSE_SPEED = PERCENT_15;
    //strcpy(display_line[3], "reverse   ");

}

void turn(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = PERCENT_20; //80
    LEFT_FORWARD_SPEED = PERCENT_20; //60
    //strcpy(display_line[2], "turn      ");
}

void turn_cw(void){
    RIGHT_FORWARD_SPEED = PERCENT_25; //30
    LEFT_REVERSE_SPEED = PERCENT_25;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    //strcpy(display_line[2], "turn cw   ");
}

void turn_cw_circle(void){
    RIGHT_FORWARD_SPEED = PERCENT_20; //30
    LEFT_REVERSE_SPEED = PERCENT_20;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    //strcpy(display_line[2], "turn cw   ");
}


void turn_ccw(void){
    RIGHT_REVERSE_SPEED = PERCENT_25; //30
    LEFT_FORWARD_SPEED = PERCENT_25;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;

    //strcpy(display_line[3], "turn ccw  ");
}

void turn_ccw_circle(void){
    RIGHT_REVERSE_SPEED = PERCENT_20; //30
    LEFT_FORWARD_SPEED = PERCENT_20;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    //strcpy(display_line[2], "turn cw   ");
}

void move_arc(void){
    RIGHT_FORWARD_SPEED = PERCENT_30; //30
    LEFT_FORWARD_SPEED = PERCENT_225;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}
void move_arc_slow(void){
    RIGHT_FORWARD_SPEED = PERCENT_25; //30
    LEFT_FORWARD_SPEED = PERCENT_20;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}


void stop(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}

