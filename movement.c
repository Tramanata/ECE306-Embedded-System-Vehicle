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
unsigned int check_start;
extern char display_line[4][11];
extern volatile unsigned char display_changed;

void project5(void){
    //bunch of if statements determining if a certain time has been hit to make the car do what it has to
    check_start = 1;
    if(time_counter >= 1 && time_counter < 3){
        Forward_On();
        strcpy(display_line[1], " forward ");
        display_changed = TRUE;
    }
    if(time_counter >= 3 && time_counter < 5){
        stop();
        strcpy(display_line[1], "   wait  ");
        display_changed = TRUE;
    }
    if(time_counter >= 5 && time_counter < 9){
        Reverse_On();
        strcpy(display_line[1], " reverse ");
        display_changed = TRUE;
    }
    if(time_counter >= 9 && time_counter < 11){
        stop();
        strcpy(display_line[1], "   wait  ");
        display_changed = TRUE;
    }
    if(time_counter >= 11 && time_counter < 15){
        Forward_On();
        strcpy(display_line[1], " forward ");
        display_changed = TRUE;
    }
    if(time_counter >= 15 && time_counter < 17){
        stop();
        strcpy(display_line[1], "   wait  ");
        display_changed = TRUE;
    }
    if(time_counter >= 17 && time_counter < 23){
        cw();
        strcpy(display_line[1], "clockwise");
        display_changed = TRUE;
    }
    if(time_counter >= 23 && time_counter < 27){
        stop();
        strcpy(display_line[1], "   wait  ");
        display_changed = TRUE;
    }
    if(time_counter >= 27 && time_counter < 33){
        ccw();
        strcpy(display_line[1], " counter ");
        strcpy(display_line[1], "clockwise");
        display_changed = TRUE;
    }
    if(time_counter >= 33 && time_counter < 37){
        stop();
        strcpy(display_line[1], "   wait  ");
        display_changed = TRUE;
    }
    if(time_counter >= 37){
        stop();
        strcpy(display_line[1], "   done  ");
        display_changed = TRUE;
    }
}


void Forward_On(void){
    P6OUT |= R_FORWARD;
    P6OUT |= L_FORWARD;
}

void Forward_Off(void){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
}

void Reverse_On(void){
    P6OUT |= R_REVERSE;
    P6OUT |= L_REVERSE;
}

void Reverse_Off(void){
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_REVERSE;
}

void stop(void){
    Forward_Off();
    Reverse_Off();
}

void cw(void){
    P6OUT |= R_FORWARD;
    P6OUT |= L_REVERSE;
}

void ccw(void){
    P6OUT |= L_FORWARD;
    P6OUT |= R_REVERSE;
}

void end_case(void){
    Forward_Off();
    Reverse_Off();
}
