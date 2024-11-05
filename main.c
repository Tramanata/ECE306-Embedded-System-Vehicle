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
unsigned volatile int baudswitch_state = 2;
extern unsigned volatile int baudswitch_update;
extern unsigned volatile int start_transmit;
extern unsigned volatile int RING_UPDATED;
extern unsigned volatile int transmit_received;
extern char process_buffer[25];

unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
unsigned int cycle_time; // is a new time base used to control making shapes
unsigned int time_change; // is an identifier that a change has occurred

volatile unsigned int DB1 = 0;
volatile unsigned int DB2 = 0;
volatile unsigned int countD = 0;
volatile unsigned int backlight = 0;

extern char USB_Ring_Rx[11];



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

// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], " Waiting  ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[3], "          ");
  strcpy(display_line[2], "115,200   ");
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
      //StateMachine();
      P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF


      if(baudswitch_update == 1){
          switch(baudswitch_state){
          case 1: //115,200
              UCA1BRW = 4; // 115,200 baud
              UCA1MCTLW = 0x5551;
              baudswitch_update = 0;
              baudswitch_state = 2;

              strcpy(display_line[2], "115,200   ");
              break;

          case 2: //460,800
              UCA1BRW = 17; // 460,800 baud
              UCA1MCTLW = 0x4A00;
              baudswitch_update = 0;
              baudswitch_state = 1;

              strcpy(display_line[2], "460,800   ");
              break;
          default: break;
          }
      }

      readRecieved();
//      if(transmit_received == 1){
//          strcpy(display_line[3], "          ");
//          strcpy(display_line[0], "Received  ");
//          strcpy(display_line[3], process_buffer);
//          //strcpy(display_line[3], process_buffer);
//          display_changed = TRUE;
//          update_display = TRUE;
//          transmit_received = 0;
//      }
      if(start_transmit == 1){
          strcpy(display_line[3], "          ");
          strcpy(display_line[0], "Transmit  ");
          USCI_A1_transmit();
          start_transmit = 0;
      }


//      HEXtoBCD(ADC_Left_Det);
//      adc_line(2,1);
//      display_changed = TRUE;
//
//      HEXtoBCD(ADC_Right_Det);
//      adc_line(1,1);
//      display_changed = TRUE;
//
//      HEXtoBCD(ADC_Thumb);
//      adc_line(3,1);
//      display_changed = TRUE;
//
//      if(display_value == 1){
//          sprintf(timearray, "%d.%d", seconds, milliseconds);
//          strcpy(display_line[3], timearray);
//          display_changed = TRUE;
//          update_display = TRUE;
//      }
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

