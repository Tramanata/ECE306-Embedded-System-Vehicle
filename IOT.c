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
        //print IP Address
        strcpy(display_line[2], "10.153.14 ");
        strcpy(display_line[3], ".163      ");
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
    strcpy(display_line[0], "IOT_CONTRO");
    display_changed = TRUE;
    update_display = TRUE;
    if(Command_Pressed == 0){
        stop();
    }
    else if(td_process_buffer[pb_row][pb_col+1] == CAP_F){
        forward_on();

        strcpy(display_line[0], "forward on");
        display_changed = TRUE;
        update_display = TRUE;
    }
    else if(td_process_buffer[pb_row][pb_col+1] == LOW_F){
            forward_slow();
            strcpy(display_line[0], "forward sl");
            display_changed = TRUE;
            update_display = TRUE;
    }
    else if(td_process_buffer[pb_row][pb_col+1] == CAP_R){
            reverse_on();
            strcpy(display_line[0], "reverse on");
            display_changed = TRUE;
            update_display = TRUE;
    }

    else if(td_process_buffer[pb_row][pb_col+1] == CAP_T){
            turn_cw();
            strcpy(display_line[0], "turn right");
            display_changed = TRUE;
            update_display = TRUE;
    }
    else if(td_process_buffer[pb_row][pb_col+1] == LOW_T){
            turn_ccw();
            strcpy(display_line[0], "turn left ");
            display_changed = TRUE;
            update_display = TRUE;
    }


}
