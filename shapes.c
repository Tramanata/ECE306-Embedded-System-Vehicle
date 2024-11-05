/*
 * shapes.c
 *
 *  Created on: Sep 25, 2024
 *      Author: trama
 */


#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')

#define WHEEL_COUNT_TIME (20)
#define RIGHT_COUNT_TIME (18) //18
#define LEFT_COUNT_TIME (8) //8
#define TRAVEL_DISTANCE (150)
#define WAITING2START (50)

#define FIGURE8_WHEEL_COUNT_TIME (20)    // Shorter cycle time to reduce the loop size

#define FIGURE8_RIGHT_COUNT_TIME (300)    // Smaller time to reduce the radius of the curves
#define FIGURE8_LEFT_COUNT_TIME (10)      // Decrease turn time to make a smaller loop

#define FIGURE8_REVERSE_RIGHT_COUNT_TIME (2)    // Smaller time to reduce the radius of the curves
#define FIGURE8_REVERSE_LEFT_COUNT_TIME (18)      // Decrease turn time to make a smaller loop

#define FIGURE8_TRAVEL_DISTANCE (80)     // Reduced distance for a smaller loop

#define TRIANGLE_WHEEL_COUNT_TIME (20)     // Time for moving straight
#define TRIANGLE_RIGHT_TURN_TIME (5)      // Time for turning by running only the right motor
#define TRIANGLE_TRAVEL_DISTANCE (20)       // Straight segments count per triangle side
#define TRIANGLE_SEGMENTS (6)              // Total segments to complete two triangles (3 sides * 2)
unsigned int turn_state = 0;
unsigned int LOOP = 1;
unsigned int LOOP2 = 0;




#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

extern unsigned int START_RUNNING;
extern unsigned int prev_timeseq;
extern unsigned int time_change;
extern unsigned int delay_start = 0;
extern unsigned int cycle_time;
extern volatile unsigned char display_changed;
extern char display_line[4][11];
unsigned int state = WAIT;
unsigned int segment_count;
unsigned int right_motor_count;
unsigned int left_motor_count;

extern unsigned int event;
extern char display_line[4][11];

void Run_FigureEight(void){
    switch(state){
        case WAIT: //wait
            wait_case();
            break;
        case START:
            start_case();
            break;
        case RUN:
            figure8_code();
            break;
        case END:
            end_case();
            break;
    default: break;
        }
}
void Run_Triangle(void){
    switch(state){
        case WAIT: //wait
            wait_case();
            break;
        case START:
            start_case();
            break;
        case RUN:
            triangle_code();
            break;
        case END:
            end_case();
            break;
    default: break;
        }
}

void wait_case(void){
    if(time_change){
        time_change = 0;
        if(delay_start++ >= WAITING2START){
            delay_start = 0;
            state = START;
        }
    }
}

void Forward_On(void){
    P6OUT |= R_FORWARD;
    P6OUT |= L_FORWARD;
}

void start_case(void){
    cycle_time = 0;
    right_motor_count = 0;
    left_motor_count = 0;
    Forward_On();
    segment_count = 0;
    state = RUN;
}


void circle_code(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= TRAVEL_DISTANCE){
            if(right_motor_count++ >= RIGHT_COUNT_TIME){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_On();
            }
        }
        if(segment_count <= TRAVEL_DISTANCE){
                    if(right_motor_count++ >= RIGHT_COUNT_TIME){
                        P6OUT &= ~R_FORWARD;
                    }
                    if(left_motor_count++ >= LEFT_COUNT_TIME){
                        P6OUT &= ~L_FORWARD;
                    }
                    if(cycle_time >= WHEEL_COUNT_TIME){
                        cycle_time = 0;
                        right_motor_count = 0;
                        left_motor_count = 0;
                        segment_count++;
                        Forward_On();
                    }

        }else{
            end_case();
        }
     }
}



void figure8_code(void) {
    if(time_change){
            time_change = 0;
            if(LOOP == 1){
                if(segment_count <= 80){
                                if(right_motor_count++ >= 18){
                                    P6OUT &= ~R_FORWARD;
                                }
                                if(left_motor_count++ >= 2){
                                    P6OUT &= ~L_FORWARD;
                                }
                                if(cycle_time >= 20){
                                    cycle_time = 0;
                                    right_motor_count = 0;
                                    left_motor_count = 0;
                                    segment_count++;
                                    Forward_On();
                                }
                            }else{
                                LOOP = 2;
                                segment_count = 0;
                                P6OUT &= ~R_FORWARD;
                                P6OUT &= ~L_FORWARD;
                                Forward_On();
                                //state = START; // changed

                            }
            }

         if(LOOP == 2){
                if(segment_count <= 80){
                    if(right_motor_count++ >= 2){
                        P6OUT &= ~R_FORWARD;

                    }
                    if(left_motor_count++ >= 18){
                        P6OUT &= ~L_FORWARD;

                    }
                    if(cycle_time >= 20){
                        cycle_time = 0;
                        right_motor_count = 0;
                        left_motor_count = 0;
                        segment_count++;
                        Forward_On();

                    }
                }else{

                    state = END;
                }
             }
    }
}





void triangle_code(void) {
    if(time_change){
                time_change = 0;
                if(LOOP == 1){
                    if(segment_count <= 10){
                                    if(right_motor_count++ >= 10){
                                        P6OUT &= ~R_FORWARD;
                                    }
                                    if(left_motor_count++ >= 10){
                                        P6OUT &= ~L_FORWARD;
                                    }
                                    if(cycle_time >= 15){
                                        cycle_time = 0;
                                        right_motor_count = 0;
                                        left_motor_count = 0;
                                        segment_count++;
                                        Forward_On();
                                    }
                                }else{
                                    LOOP = 2;
                                    segment_count = 0;
                                    P6OUT &= ~R_FORWARD;
                                    P6OUT &= ~L_FORWARD;
                                    Forward_On();
                                }
                }

             if(LOOP == 2){
                 strcpy(display_line[0], "          ");
                 strcpy(display_line[1], " loop2    ");
                 strcpy(display_line[2], "          ");
                 strcpy(display_line[3], "          ");
                 display_changed = TRUE;
                    if(segment_count <= 6){

                        if(left_motor_count++ >= 2){
                            P6OUT &= ~L_FORWARD;

                        }
                        if(cycle_time >= 15){
                            cycle_time = 0;
                            right_motor_count = 0;
                            left_motor_count = 0;
                            segment_count++;
                            //Forward_On();
                            strcpy(display_line[0], "          ");
                            strcpy(display_line[1], " ended    ");
                            strcpy(display_line[2], "          ");
                            strcpy(display_line[3], "          ");
                            //state = END;
                        }
                    }else{
                        LOOP = 3;
                        segment_count = 0;
                        P6OUT &= ~R_FORWARD;
                        P6OUT &= ~L_FORWARD;
                        Forward_On();
                    }
                 }
             if(LOOP == 3){
                                 if(segment_count <= 10){
                                                 if(right_motor_count++ >= 10){
                                                     P6OUT &= ~R_FORWARD;
                                                 }
                                                 if(left_motor_count++ >= 10){
                                                     P6OUT &= ~L_FORWARD;
                                                 }
                                                 if(cycle_time >= 15){
                                                     cycle_time = 0;
                                                     right_motor_count = 0;
                                                     left_motor_count = 0;
                                                     segment_count++;
                                                     Forward_On();
                                                 }
                                             }else{
                                                 LOOP = 4;
                                                 segment_count = 0;
                                                 P6OUT &= ~R_FORWARD;
                                                 P6OUT &= ~L_FORWARD;
                                                 Forward_On();
                                             }
                             }

                          if(LOOP == 4){
                              strcpy(display_line[0], "          ");
                              strcpy(display_line[1], " loop2    ");
                              strcpy(display_line[2], "          ");
                              strcpy(display_line[3], "          ");
                              display_changed = TRUE;
                                 if(segment_count <= 6){
                                     if(left_motor_count++ >= 2){
                                         P6OUT &= ~L_FORWARD;
                                         strcpy(display_line[0], "          ");
                                                                 strcpy(display_line[1], " left     ");
                                                                 strcpy(display_line[2], "          ");
                                                                 strcpy(display_line[3], "          ");
                                     }
                                     if(cycle_time >= 15){
                                         cycle_time = 0;
                                         right_motor_count = 0;
                                         left_motor_count = 0;
                                         segment_count++;
                                         //Forward_On();
                                         strcpy(display_line[0], "          ");
                                         strcpy(display_line[1], " ended    ");
                                         strcpy(display_line[2], "          ");
                                         strcpy(display_line[3], "          ");
                                         //state = END;
                                     }
                                 }else{
                                     LOOP = 5;
                                     segment_count = 0;
                                     P6OUT &= ~R_FORWARD;
                                     P6OUT &= ~L_FORWARD;
                                     Forward_On();
                                 }
                              }
                          if(LOOP == 5){
                                              if(segment_count <= 10){
                                                              if(right_motor_count++ >= 10){
                                                                  P6OUT &= ~R_FORWARD;
                                                              }
                                                              if(left_motor_count++ >= 10){
                                                                  P6OUT &= ~L_FORWARD;
                                                              }
                                                              if(cycle_time >= 15){
                                                                  cycle_time = 0;
                                                                  right_motor_count = 0;
                                                                  left_motor_count = 0;
                                                                  segment_count++;
                                                                  Forward_On();
                                                              }
                                                          }else{
                                                              LOOP = 6;
                                                              segment_count = 0;
                                                              P6OUT &= ~R_FORWARD;
                                                              P6OUT &= ~L_FORWARD;
                                                              Forward_On();
                                                          }
                                          }

                                       if(LOOP == 6){
                                           strcpy(display_line[0], "          ");
                                           strcpy(display_line[1], " loop2    ");
                                           strcpy(display_line[2], "          ");
                                           strcpy(display_line[3], "          ");
                                           display_changed = TRUE;
                                              if(segment_count <= 6){

                                                  if(left_motor_count++ >= 2){
                                                      P6OUT &= ~L_FORWARD;
                                                      strcpy(display_line[0], "          ");
                                                                              strcpy(display_line[1], " left     ");
                                                                              strcpy(display_line[2], "          ");
                                                                              strcpy(display_line[3], "          ");
                                                  }
                                                  if(cycle_time >= 15){
                                                      cycle_time = 0;
                                                      right_motor_count = 0;
                                                      left_motor_count = 0;
                                                      segment_count++;
                                                      //Forward_On();
                                                      strcpy(display_line[0], "          ");
                                                      strcpy(display_line[1], " ended    ");
                                                      strcpy(display_line[2], "          ");
                                                      strcpy(display_line[3], "          ");
                                                      //state = END;
                                                  }
                                              }else{
                                                      LOOP = 7;
                                                      segment_count = 0;
                                                      P6OUT &= ~R_FORWARD;
                                                      P6OUT &= ~L_FORWARD;
                                                      Forward_On();
                                              }
                                           }
                                       if(LOOP == 7){
                                                           if(segment_count <= 10){
                                                                           if(right_motor_count++ >= 10){
                                                                               P6OUT &= ~R_FORWARD;
                                                                           }
                                                                           if(left_motor_count++ >= 10){
                                                                               P6OUT &= ~L_FORWARD;
                                                                           }
                                                                           if(cycle_time >= 15){
                                                                               cycle_time = 0;
                                                                               right_motor_count = 0;
                                                                               left_motor_count = 0;
                                                                               segment_count++;
                                                                               Forward_On();
                                                                           }
                                                                       }else{
                                                                           LOOP = 8;
                                                                           segment_count = 0;
                                                                           P6OUT &= ~R_FORWARD;
                                                                           P6OUT &= ~L_FORWARD;
                                                                           Forward_On();
                                                                       }
                                                       }

                                                    if(LOOP == 8){
                                                        strcpy(display_line[0], "          ");
                                                        strcpy(display_line[1], " loop2    ");
                                                        strcpy(display_line[2], "          ");
                                                        strcpy(display_line[3], "          ");
                                                        display_changed = TRUE;
                                                           if(segment_count <= 6){

                                                               if(left_motor_count++ >= 2){
                                                                   P6OUT &= ~L_FORWARD;

                                                               }
                                                               if(cycle_time >= 15){
                                                                   cycle_time = 0;
                                                                   right_motor_count = 0;
                                                                   left_motor_count = 0;
                                                                   segment_count++;
                                                                   //Forward_On();
                                                                   strcpy(display_line[0], "          ");
                                                                   strcpy(display_line[1], " ended    ");
                                                                   strcpy(display_line[2], "          ");
                                                                   strcpy(display_line[3], "          ");
                                                                   //state = END;
                                                               }
                                                           }else{
                                                               LOOP = 9;
                                                               segment_count = 0;
                                                               P6OUT &= ~R_FORWARD;
                                                               P6OUT &= ~L_FORWARD;
                                                               Forward_On();
                                                           }
                                                        }
                                                    if(LOOP == 9){
                                                                        if(segment_count <= 10){
                                                                                        if(right_motor_count++ >= 10){
                                                                                            P6OUT &= ~R_FORWARD;
                                                                                        }
                                                                                        if(left_motor_count++ >= 10){
                                                                                            P6OUT &= ~L_FORWARD;
                                                                                        }
                                                                                        if(cycle_time >= 15){
                                                                                            cycle_time = 0;
                                                                                            right_motor_count = 0;
                                                                                            left_motor_count = 0;
                                                                                            segment_count++;
                                                                                            Forward_On();
                                                                                        }
                                                                                    }else{
                                                                                        LOOP = 10;
                                                                                        segment_count = 0;
                                                                                        P6OUT &= ~R_FORWARD;
                                                                                        P6OUT &= ~L_FORWARD;
                                                                                        Forward_On();
                                                                                    }
                                                                    }

                                                                 if(LOOP == 10){
                                                                     strcpy(display_line[0], "          ");
                                                                     strcpy(display_line[1], " loop2    ");
                                                                     strcpy(display_line[2], "          ");
                                                                     strcpy(display_line[3], "          ");
                                                                     display_changed = TRUE;
                                                                        if(segment_count <= 6){
                                                                            if(left_motor_count++ >= 2){
                                                                                P6OUT &= ~L_FORWARD;
                                                                                strcpy(display_line[0], "          ");
                                                                                                        strcpy(display_line[1], " left     ");
                                                                                                        strcpy(display_line[2], "          ");
                                                                                                        strcpy(display_line[3], "          ");
                                                                            }
                                                                            if(cycle_time >= 15){
                                                                                cycle_time = 0;
                                                                                right_motor_count = 0;
                                                                                left_motor_count = 0;
                                                                                segment_count++;
                                                                                //Forward_On();
                                                                                strcpy(display_line[0], "          ");
                                                                                strcpy(display_line[1], " ended    ");
                                                                                strcpy(display_line[2], "          ");
                                                                                strcpy(display_line[3], "          ");
                                                                                //state = END;
                                                                            }
                                                                        }else{
                                                                            LOOP = 11;
                                                                            segment_count = 0;
                                                                            P6OUT &= ~R_FORWARD;
                                                                            P6OUT &= ~L_FORWARD;
                                                                            Forward_On();
                                                                        }
                                                                     }
                                                                 if(LOOP == 11){
                                                                                     if(segment_count <= 10){
                                                                                                     if(right_motor_count++ >= 10){
                                                                                                         P6OUT &= ~R_FORWARD;
                                                                                                     }
                                                                                                     if(left_motor_count++ >= 10){
                                                                                                         P6OUT &= ~L_FORWARD;
                                                                                                     }
                                                                                                     if(cycle_time >= 15){
                                                                                                         cycle_time = 0;
                                                                                                         right_motor_count = 0;
                                                                                                         left_motor_count = 0;
                                                                                                         segment_count++;
                                                                                                         Forward_On();
                                                                                                     }
                                                                                                 }else{
                                                                                                     LOOP = 12;
                                                                                                     segment_count = 0;
                                                                                                     P6OUT &= ~R_FORWARD;
                                                                                                     P6OUT &= ~L_FORWARD;
                                                                                                     Forward_On();
                                                                                                 }
                                                                                 }

                                                                              if(LOOP == 12){
                                                                                  strcpy(display_line[0], "          ");
                                                                                  strcpy(display_line[1], " loop2    ");
                                                                                  strcpy(display_line[2], "          ");
                                                                                  strcpy(display_line[3], "          ");
                                                                                  display_changed = TRUE;
                                                                                     if(segment_count <= 7){

                                                                                         if(left_motor_count++ >= 2){
                                                                                             P6OUT &= ~L_FORWARD;
                                                                                             strcpy(display_line[0], "          ");
                                                                                                                     strcpy(display_line[1], " left     ");
                                                                                                                     strcpy(display_line[2], "          ");
                                                                                                                     strcpy(display_line[3], "          ");
                                                                                         }
                                                                                         if(cycle_time >= 15){
                                                                                             cycle_time = 0;
                                                                                             right_motor_count = 0;
                                                                                             left_motor_count = 0;
                                                                                             segment_count++;
                                                                                             //Forward_On();
                                                                                             strcpy(display_line[0], "          ");
                                                                                             strcpy(display_line[1], " ended    ");
                                                                                             strcpy(display_line[2], "          ");
                                                                                             strcpy(display_line[3], "          ");
                                                                                             //state = END;
                                                                                         }
                                                                                     }else{
                                                                                             state = END;
                                                                                     }
                                                                                  }

    }
}


void Forward_Off(void){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
}

void end_case(void){
    Forward_Off();
    state = WAIT;
    event = NONE;
    START_RUNNING = FALSE;
}





