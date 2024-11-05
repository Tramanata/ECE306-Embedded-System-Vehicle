/*
 * switches.c
 *
 *  Created on: Sep 25, 2024
 *      Author: trama
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

#define PRESSED (1)
#define RELEASED (0)

#define BEG (0)
#define CIRCLE (1)
#define FIGUREEIGHT (2)
#define TRIANGLE (3)

unsigned char sw1_position = RELEASED;
unsigned char sw2_position  = RELEASED;
unsigned int event =  BEG;
unsigned int START_RUNNING = RELEASED;

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];



void StateMachine(void){
    if(START_RUNNING == PRESSED){
        switch(event){
             case CIRCLE: // Straight
                 //Run_Circle();
                 circle_code();
                 break; //
             case FIGUREEIGHT: // Circle
                 //Run_FigureEight();
                 //run_fig8();
                 Run_FigureEight();
                 break; //
             case TRIANGLE: // Circle
                 Run_Triangle();
                 break; //
             default: break;
             }
    }
}

void Switches_Process(void){
    Switch1_Process();
    Switch2_Process();
}

void Switch1_Select(void){
    switch(event){
        case BEG:
            event = CIRCLE;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "  circle  ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            break;
        case CIRCLE:
            event = FIGUREEIGHT;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], " figure 8 ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            break;
        case FIGUREEIGHT:
            event = TRIANGLE;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], " triangle ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            break;
        case TRIANGLE:
            event = BEG;
            strcpy(display_line[0], "    Hit   ");
            strcpy(display_line[1], " to begin ");
            strcpy(display_line[2], "          ");
            strcpy(display_line[3], "          ");
            display_changed = TRUE;
            break;
        default: break;

    }
}

void Switch1_Process(void){
 //-----------------------------------------------------------------------------
// Switch 1 Configurations
 //-----------------------------------------------------------------------------
    if(sw1_position != PRESSED){
        if (!(P4IN & SW1)){
            sw1_position = PRESSED;
            Switch1_Select();
        }
    }
    if(sw1_position == PRESSED){
         if(P4IN & SW1){
             sw1_position = RELEASED;
         }
    }
}

void Switch2_Select(void){
    START_RUNNING = PRESSED;
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], " starting ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
}

void Switch2_Process(void){
 //-----------------------------------------------------------------------------
// Switch 2 Configurations
 //-----------------------------------------------------------------------------
    if(sw2_position != PRESSED){
            if (!(P2IN & SW2)){
                sw2_position = PRESSED;
                START_RUNNING = PRESSED;
                Switch2_Select();
            }
     }

            if(sw2_position == PRESSED){
                if(P2IN & SW2){
                    sw2_position = RELEASED;
                    strcpy(display_line[3], "          ");
                    display_changed = TRUE;
                }
            }

 }






