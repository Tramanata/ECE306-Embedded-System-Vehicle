////------------------------------------------------------------------------------
////
////  Description: Homework5
////
////  Jim Carlson
////  Jan 2016
////  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
////------------------------------------------------------------------------------
//
////------------------------------------------------------------------------------
//#include  "msp430.h"
//#include  <string.h>
//#include  "functions.h"
//#include  "LCD.h"
//#include  "ports.h"
//#include "macros.h"
//
//unsigned char sw1_position = RELEASED;
//unsigned char sw2_position  = RELEASED;
//unsigned int state = USE_GPIO;
//unsigned int Switched = RELEASED;
//
//extern volatile unsigned char display_changed;
//extern volatile unsigned char update_display;
//extern char display_line[4][11];
//
//void hw5(void){
//    if(Switched == Pressed){
//        switch(state){
//            case USE_GPIO:
//                Init_Port3(USE_SMCLK);
//                state = USE_SMCLK;
//                strcpy(display_line[1], "   SMCLK  ");
//                strcpy(display_line[2], "          ");
//                update_display = TRUE;
//            case USE_SMCLK:
//                Init_Port3(USE_GPIO);
//                state = USE_GPIO;
//                strcpy(display_line[1], "   GPIO   ");
//                strcpy(display_line[2], "          ");
//                update_display = TRUE;
//
//        }
//    }
//
//}
//
//void Switch2_Select(void){
//    Switched = PRESSED;
//    strcpy(display_line[0], "          ");
//    strcpy(display_line[1], " switch 2 ");
//    strcpy(display_line[2], " pressed  ");
//    strcpy(display_line[3], "          ");
//    display_changed = TRUE;
//}
//
//void Switch2_Process(void){
// //-----------------------------------------------------------------------------
//// Switch 2 Configurations
// //-----------------------------------------------------------------------------
//    if(sw2_position != PRESSED){
//            if (!(P2IN & SW2)){
//                sw2_position = PRESSED;
//                Switch2_Select();
//            }
//     }
//
//            if(sw2_position == PRESSED){
//                if(P2IN & SW2){
//                    sw2_position = RELEASED;
//                    strcpy(display_line[3], "          ");
//                    display_changed = TRUE;
//                }
//            }
//
// }
