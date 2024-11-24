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
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle

//    RIGHT_FORWARD_SPEED = SLOW_RIGHT;
//    LEFT_FORWARD_SPEED = SLOW;
    RIGHT_FORWARD_SPEED = PERCENT_80;
    LEFT_FORWARD_SPEED = PERCENT_40;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    strcpy(display_line[3], "forward on");
}

void forward_slow(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle

    RIGHT_FORWARD_SPEED = PERCENT_40;
    LEFT_FORWARD_SPEED = PERCENT_30;

//    RIGHT_FORWARD_SPEED = PERCENT_60;
//    LEFT_FORWARD_SPEED = PERCENT_40;

    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    //strcpy(display_line[3], "forwardslo");

}

void forward_circle(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle

    RIGHT_FORWARD_SPEED = PERCENT_25; //30
    LEFT_FORWARD_SPEED = PERCENT_25; //30

//    RIGHT_FORWARD_SPEED = PERCENT_60;
//    LEFT_FORWARD_SPEED = PERCENT_40;

    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    //strcpy(display_line[3], "forwardslo");

}
//void slight_left(void){
//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
//    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
//    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
//    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
//
//    RIGHT_FORWARD_SPEED = PERCENT_40;
//    LEFT_FORWARD_SPEED = PERCENT_40;
//    RIGHT_REVERSE_SPEED = WHEEL_OFF;
//    LEFT_REVERSE_SPEED = WHEEL_OFF;
//}
//
//void slight_right(void){
//    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
//    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
//    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
//    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
//
//    RIGHT_FORWARD_SPEED = WHEEL_OFF;
//    LEFT_FORWARD_SPEED = WHEEL_OFF;
//    RIGHT_REVERSE_SPEED = PERCENT_40;
//    LEFT_REVERSE_SPEED = PERCENT_60;
//}

void reverse_on(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle

    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = PERCENT_25; //30
    LEFT_REVERSE_SPEED = PERCENT_20;
    //strcpy(display_line[3], "reverse   ");

}

void turn(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle

    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = PERCENT_40; //80
    LEFT_FORWARD_SPEED = PERCENT_20; //60
    //strcpy(display_line[3], "turn      ");
}

void turn_cw(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle

    RIGHT_FORWARD_SPEED = PERCENT_25; //30
    LEFT_REVERSE_SPEED = PERCENT_20;
    //strcpy(display_line[3], "turn cw   ");
}


void turn_ccw(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle

    RIGHT_REVERSE_SPEED = PERCENT_30; //30
    LEFT_FORWARD_SPEED = PERCENT_20;

    //strcpy(display_line[3], "turn ccw  ");
}



void stop(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}

