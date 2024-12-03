#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "stdio.h"

unsigned volatile int run_bootup = 1;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
unsigned volatile int IOT_step = 0;
unsigned volatile int step_temp = 0;
extern unsigned volatile int IOT_Send;
extern char PC_2_IOT[11];
extern volatile unsigned int seconds;
extern unsigned volatile int Command_Pressed;

extern char td_process_buffer[4][25];
extern unsigned volatile int pb_row;
extern unsigned volatile int pb_col;
unsigned volatile int blackline_start_flag = 0; //used to know when to start my black line function ^B
extern unsigned volatile int move_time;
unsigned volatile int got_ip = 0;
unsigned volatile int new_station = 0;
unsigned volatile int station_state = 0;



void boot_up(void){
    switch(IOT_step){
    case ENABLE_IOT:
        P3OUT |= IOT_EN;
        //IOT_step = STOP_SAVE;
        IOT_step = 6;
        break;
    case STOP_SAVE:
        //Stop Saving Commands
        strcpy(PC_2_IOT, "AT+SYSSTORE=0\r\n");
        IOT_Send = 1;
        UCA0IE |= UCTXIE;
        //IOT_step = UPDATE_CONNECTIONS;
        IOT_step = 6;
        break;
    case UPDATE_CONNECTIONS:
        //Update Connection Settings
        //AT+CIPMUX=1\r\n
        strcpy(PC_2_IOT, "AT+CIPMUX=1\r\n");
        IOT_Send = 1;
        UCA0IE |= UCTXIE;
        //IOT_step = SERVER_PORT;
        IOT_step = 6;
        break;
    case SERVER_PORT:
        //AT+CIPSERVER=1,8920\r\n
        strcpy(PC_2_IOT, "AT+CIPSERVER=1,8920\r\n");
        IOT_Send = 1;
        UCA0IE |= UCTXIE;

        //IOT_step = SSID;
        IOT_step = 6;
        break;
    case SSID:
        //AT+CWJAP?r\n
        strcpy(PC_2_IOT, "AT+CWJAP?\r\n");
//        strcpy(display_line[0], "   ncsu   ");
        IOT_Send = 1;
        UCA0IE |= UCTXIE;
        //IOT_step = GET_IP;
        IOT_step = 6;
        break;
    case GET_IP:
        //get the IP address
        //AT+CIFSR\r\n
        strcpy(PC_2_IOT, "AT+CIFSR\r\n");
        IOT_Send = 1;
        UCA0IE |= UCTXIE;
        got_ip = 1;
        //print IP Address
//        strcpy(display_line[1], "10.153.14 ");
//        strcpy(display_line[2], ".163      ");
        //strcpy(display_line[0], "bootupdone");

        display_changed = TRUE;
        update_display = TRUE;
        run_bootup = 0; //finish bootup and do not need to run anymore
        IOT_step = 6;
        break;
    default:
        break;
    }
}

void IOT_Control(void){
    display_changed = TRUE;
    update_display = TRUE;
    if(Command_Pressed == 0){
        stop();
    }
    else if(td_process_buffer[pb_row][pb_col+1] == CAP_F){
        move_time = 8;
        forward_on();
        new_station = 0;
        strcpy(display_line[3], "^F        ");
        display_changed = TRUE;
        update_display = TRUE;
    }
    else if(td_process_buffer[pb_row][pb_col+1] == LOW_F){
            move_time= 6;
            forward_slow();
            new_station = 0;
            strcpy(display_line[3], "^f        ");
            display_changed = TRUE;
            update_display = TRUE;
    }
    else if(td_process_buffer[pb_row][pb_col+1] == CAP_R){
            move_time = 8;
            reverse_on();
            new_station = 0;
            strcpy(display_line[3], "^R        ");
            display_changed = TRUE;
            update_display = TRUE;
    }

    else if(td_process_buffer[pb_row][pb_col+1] == CAP_T){
            move_time = 4;
            turn_cw();
            new_station = 0;
            strcpy(display_line[3], "^T        ");
            display_changed = TRUE;
            update_display = TRUE;
    }
    else if(td_process_buffer[pb_row][pb_col+1] == LOW_T){
            move_time = 4;
            turn_ccw();
            new_station = 0;
            strcpy(display_line[3], "^t        ");
            display_changed = TRUE;
            update_display = TRUE;
    }
    if(td_process_buffer[pb_row][pb_col+1] == CAP_B){
        //blackline_start_flag = 1;
        proj7_StateMachine();
        strcpy(display_line[3], "^B        ");
        display_changed = TRUE;
        update_display = TRUE;
    }
    else if(td_process_buffer[pb_row][pb_col+1] == CAP_E){
        //blackline_start_flag = 1;
        exiting_circle();

        strcpy(display_line[3], "^E        ");
        display_changed = TRUE;
        update_display = TRUE;
    }
    else if(td_process_buffer[pb_row][pb_col+1] == CAP_S){
        //blackline_start_flag = 1;
        new_station += 1;
        strcpy(display_line[3], "^S        ");
        display_changed = TRUE;
        update_display = TRUE;
    }

}

void parse(void) {
    static char ssid[25] = ""; // Buffer to store SSID
    static char ip_address[25] = ""; // Buffer to store IP address
    static unsigned volatile int ssid_set = 0; // Flag to check if SSID is set
    char temp_buffer[25]; // Temporary buffer for storing parsed data
    unsigned int temp_index = 0; // Index for temporary buffer
    unsigned int parsing = 0; // Parsing flag: 0 = not parsing, 1 = parsing
    unsigned int i = 0;

    // Loop through td_process_buffer to extract data
    if(parsing == 0){
        // Check for start of string
        if (td_process_buffer[i] == '"' && !parsing) {
            parsing = 1; // Start parsing
            temp_index = 0; // Reset temporary buffer index
        }

        // Check for end of string
        if (td_process_buffer[i] == '"' && parsing) {
            temp_buffer[temp_index] = '\0'; // Null-terminate the string
            parsing = 0; // Stop parsing

            if (!ssid_set) {
                // Store SSID if not already set
                strcpy(ssid, temp_buffer);
                ssid_set = 1; // Mark SSID as set
                strcpy(display_line[0], ssid); // Display SSID on the first line
                display_changed = TRUE;
                update_display = TRUE;
            } else {
                // Store IP address
                strcpy(ip_address, temp_buffer);
                strcpy(display_line[1], ip_address); // Display IP on the second line
                display_changed = TRUE;
                update_display = TRUE;
            }
        }

        // Store characters into the temporary buffer while parsing
        if (parsing && temp_index < sizeof(temp_buffer) - 1) {
            temp_buffer[temp_index++] = td_process_buffer[i];
        }
    }
}
