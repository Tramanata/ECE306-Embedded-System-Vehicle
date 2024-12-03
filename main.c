//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "stdio.h"



// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);

  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;
unsigned int wheel_move;
char forward;
extern volatile unsigned int ADC_Thumb;
extern volatile unsigned int ADC_Right_Det;
extern volatile unsigned int ADC_Left_Det;
extern volatile unsigned int seconds;
extern volatile unsigned int milliseconds;
extern unsigned volatile int display_value;
char timearray[10];

extern unsigned volatile int start_transmit_A0;
extern unsigned volatile int start_transmit_A1;

unsigned volatile int this_does_literally_nothing = 0;
extern char process_buffer[25];

unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
unsigned int cycle_time; // is a new time base used to control making shapes
unsigned int time_change; // is an identifier that a change has occurred

volatile unsigned int DB1 = 0;
volatile unsigned int DB2 = 0;
volatile unsigned int countD = 0;
volatile unsigned int backlight = 0;
extern unsigned volatile int run_bootup;
extern unsigned volatile int IOT_step;
unsigned volatile int prev_ADC_Thumb;
extern unsigned volatile int got_ip;

extern char USB_Ring_Rx[11];

unsigned volatile int my_black_val;
unsigned volatile int my_white_val;
unsigned volatile int calibration = 0;
extern unsigned volatile int new_station;
extern unsigned volatile int station_state;
unsigned volatile int ping_flag = 0;
extern char PC_2_IOT[25]; //A0TX
extern unsigned volatile int IOT_Send;


void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings
  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                    // Initialize Variables and Initial Conditions
  Init_ADC();
  //Init_Timer();                       // Initialize Timers
  Init_Timer_B0();
  Init_Timer_B3();
  Init_LCD();                          // Initialize LCD
  Init_Serial_UCA1();
  Init_Serial_UCA0();
  Init_DAC();

// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "Arrived 0 ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

  wheel_move = 0;
  forward = TRUE;

//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
      Display_Process();                  // Update Display

      //proj7_StateMachine();
      if(calibration == 1){
          P2OUT |= IR_LED;
      }else if(calibration == 2){
          my_black_val = ADC_Right_Det;
          strcpy(display_line[2], "cal black ");
      }else if(calibration == 3){
          my_white_val = ADC_Right_Det;
          strcpy(display_line[2], "cal white ");
          calibration = 0;
      }


      //StateMachine();
      P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF

      if(prev_ADC_Thumb != ADC_Thumb){
          strcpy(display_line[0], "          ");
          strcpy(display_line[1], "          ");
          strcpy(display_line[2], "          ");
          strcpy(display_line[3], "          ");
          display_changed = TRUE;
          update_display = TRUE;
      }

      switch(ADC_Thumb){
          case 0x00:
              strcpy(display_line[0], "Check Emit");

              HEXtoBCD(ADC_Left_Det);
              strcpy(display_line[2], "L:        ");
              adc_line(3,3);
              display_changed = TRUE;
              update_display = TRUE;

              HEXtoBCD(ADC_Right_Det);
              strcpy(display_line[1], "R:        ");
              adc_line(2,3);
              display_changed = TRUE;
              update_display = TRUE;

              prev_ADC_Thumb = 0;
              break;
          case 0x01:
              if(got_ip == 1){
                  strcpy(display_line[0], " IP & Wifi");
                  strcpy(display_line[1], " 10.153.4 ");
                  strcpy(display_line[2], "   3.72   ");
                  strcpy(display_line[3], "   ncsu   ");
              }

              display_changed = TRUE;
              update_display = TRUE;

              prev_ADC_Thumb = 1;
              break;
          case 0x02:
              strcpy(display_line[0], " Thumb #3 ");
//              bat_value = bat_value * 154;
//              HEXtoBCD(bat_value);
//              strcpy(display_line[2], "mV:        ");
//              adc_line(3,4);
              display_changed = TRUE;
              update_display = TRUE;

              prev_ADC_Thumb = 2;
              break;
          case 0x03:

              if(new_station == 1){
                  station_state += 1;
                  display_line[0][9] = station_state + 0x30;
                  display_changed = TRUE;
                  update_display = TRUE;
                  new_station == 2;
              }else if(new_station > 1){
                  this_does_literally_nothing = 1;
                  //
              }else if(new_station == 0){
                  strcpy(display_line[0], "Arrived 0 ");
                  display_changed = TRUE;
                  update_display = TRUE;
              }

              if(got_ip == 1){
                  strcpy(display_line[1], "  Tyler   ");
                  strcpy(display_line[2], " Ramanata ");
              }
              display_changed = TRUE;
              update_display = TRUE;
              prev_ADC_Thumb = 3;
              break;
          default: break;
      }



      //"AT+PING=\"www.google.com\"\r\n"

//      sprintf(timearray, "%d.%d", seconds, milliseconds);
//      //strcpy(display_line[3], timearray);
//      strncpy(display_line[3], timearray, 10);
//      display_changed = TRUE;
//      update_display = TRUE;
//
//      HEXtoBCD(ADC_Left_Det);
//      adc_line(2,1);
//      display_changed = TRUE;
//
//      HEXtoBCD(ADC_Right_Det);
//      adc_line(1,1);
//      display_changed = TRUE;

//      HEXtoBCD(ADC_Thumb);
//      adc_line(3,1);
//      display_changed = TRUE;

      if(run_bootup == 1){
          if(seconds > 4){
              boot_up();
          }
      }else{
          if(ping_flag == 1){
              ping_flag = 0;
              strcpy(PC_2_IOT, "AT+PING=\"www.google.com\"\r\n");
              IOT_Send = 1;
              UCA0IE |= UCTXIE;
          }
          IOT_Control();
      }


  }
//------------------------------------------------------------------------------
}


void Carlson_StateMachine(void){
    switch(Time_Sequence){
      case 250:                        //
        if(one_time){
          Init_LEDs();
          lcd_BIG_mid();
          display_changed = 1;
          one_time = 0;
        }
        Time_Sequence = 0;             //
        break;
      case 200:                        //
        if(one_time){
//          P1OUT &= ~RED_LED;            // Change State of LED 4
          P6OUT |= GRN_LED;            // Change State of LED 5
          P6OUT |= L_FORWARD;
          P6OUT |= R_FORWARD;
          one_time = 0;
        }
        break;
      case 150:                         //
        if(one_time){
          P1OUT |= RED_LED;            // Change State of LED 4
          P6OUT &= ~GRN_LED;            // Change State of LED 5
          P6OUT &= ~L_FORWARD;
          P6OUT &= ~R_FORWARD;
          one_time = 0;
        }
        break;
      case 100:                         //
        if(one_time){
//          lcd_4line();
          lcd_BIG_bot();
          P6OUT |= GRN_LED;            // Change State of LED 5
          display_changed = 1;
          one_time = 0;
        }
        break;
      case  50:                        //
        if(one_time){
          one_time = 0;
        }
        break;                         //
      default: break;
    }
}

