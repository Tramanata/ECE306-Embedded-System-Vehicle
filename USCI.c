#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "stdio.h"


// Global Variables
char process_buffer[25]; // Size for appropriate Command Length
char td_process_buffer[4][25];
unsigned volatile int pb_row = 0;
unsigned volatile int pb_col = 0;

//char USB_Ring_Rx[11];
//char IOT_Ring_Rx[11];

char USB_2_PC[25]; //A1RX
char PC_2_USB[25]; //A1TX

char IOT_2_PC[25]; //A0RX
char PC_2_IOT[25]; //A0TX

unsigned volatile int usb_rx_wr1 = 0;
unsigned volatile int iot_rx_wr1 = 0;
unsigned volatile int usb_rx_wr_temp1;
unsigned volatile int usb_rx_rd1 = 0;
unsigned volatile int iot_rx_rd1 = 0;

unsigned volatile int usb_rx_wr0 = 0;
unsigned volatile int iot_rx_wr0 = 0;
unsigned volatile int usb_rx_wr_temp0;
unsigned volatile int usb_rx_rd0 = 0;
unsigned volatile int iot_rx_rd0 = 0;

unsigned volatile int start_transmit_A0;
unsigned volatile int start_transmit_A1;

unsigned volatile int direct_iot1 = 0;
unsigned volatile int direct_iot0 = 0;
unsigned volatile int IOT_Send = 0;

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern unsigned volatile int Command_Pressed;


void readRecieved_A1(void){
    usb_rx_wr_temp1 = usb_rx_wr1;
    if(usb_rx_wr_temp1 != usb_rx_rd1){
        td_process_buffer[pb_row][pb_col] = USB_2_PC[usb_rx_rd1++];
        if(td_process_buffer[pb_row][pb_col] == 0x0D){
            strcpy(PC_2_IOT, td_process_buffer[pb_row]);

            strcpy(display_line[0], PC_2_IOT);

            strcpy(td_process_buffer[pb_row], "          ");
            display_changed = TRUE;
            update_display = TRUE;
            //reset buffer and print the string
            pb_row++;
            if(pb_row > 3){
                pb_row = 0;
            }
            pb_col = 0;
            start_transmit_A0  = 1; //flag to transmit

        }
        pb_col++;
    }
    if(usb_rx_rd1 >= sizeof(USB_2_PC)){
        usb_rx_rd1 = BEGINNING;
    }
}

void readRecieved_A0(void){
    usb_rx_wr_temp0 = usb_rx_wr0;
    if(usb_rx_wr_temp0 != usb_rx_rd0){
        td_process_buffer[pb_row][pb_col] = IOT_2_PC[usb_rx_rd0++];
        if(td_process_buffer[pb_row][pb_col] == 0x0D){
            strcpy(PC_2_USB, td_process_buffer[pb_row]);

            strcpy(display_line[2], PC_2_USB);
            display_changed = TRUE;
            update_display = TRUE;

            strcpy(td_process_buffer[pb_row], "          ");
            //reset buffer and print the string
            pb_row++;
            if(pb_row > 3){
                pb_row = 0;
            }
            pb_col = 0;
            start_transmit_A1  = 1; //flag to transmit
        }
        pb_col++;
    }
    if(usb_rx_rd0 >= sizeof(IOT_2_PC)){
        usb_rx_rd0 = BEGINNING;
    }
}



void USCI_A1_transmit(void){ // Transmit Function for USCI_A1
    strcpy(display_line[3], PC_2_USB);
    display_changed = TRUE;
    update_display = TRUE;
    //UCA1IE |= UCTXIE; // Enable TX interrupt
    start_transmit_A1  = 0;
}
void USCI_A0_transmit(void){ // Transmit Function for USCI_A0
    strcpy(display_line[1], PC_2_IOT);
    display_changed = TRUE;
    update_display = TRUE;
    UCA0IE |= UCTXIE; // Enable TX interrupt
    start_transmit_A0  = 0;
}


void Init_Serial_UCA1(void){
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
// 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
// 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
// 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
    UCA1CTLW0 = 0;
     UCA1CTLW0 |= UCSWRST ; // Put eUSCI in reset
     UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
     UCA1CTLW0 &= ~UCMSB; // MSB, LSB select
     UCA1CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
     UCA1CTLW0 &= ~UCPEN; // No Parity
     UCA1CTLW0 &= ~UCSYNC;
     UCA1CTLW0 &= ~UC7BIT;
     UCA1CTLW0 |= UCMODE_0;
 // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
 // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
 // UCA?MCTLW = UCSx + UCFx + UCOS16
     UCA1BRW = 4 ; // 115,200 baud
     UCA1MCTLW = 0x5551;
     UCA1CTLW0 &= ~UCSWRST ; // release from reset
     UCA1TXBUF = 0x00; // Prime the Pump
     UCA1IE |= UCRXIE; // Enable RX interrupt
//------------------------------------------------------------------------------
}

void Init_Serial_UCA0(void){
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
// 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
// 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
// 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
    UCA0CTLW0 = 0;
     UCA0CTLW0 |= UCSWRST ; // Put eUSCI in reset
     UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
     UCA0CTLW0 &= ~UCMSB; // MSB, LSB select
     UCA0CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
     UCA0CTLW0 &= ~UCPEN; // No Parity
     UCA0CTLW0 &= ~UCSYNC;
     UCA0CTLW0 &= ~UC7BIT;
     UCA0CTLW0 |= UCMODE_0;
 // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
 // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
 // UCA?MCTLW = UCSx + UCFx + UCOS16
     UCA0BRW = 4 ; // 115,200 baud
     UCA0MCTLW = 0x5551;
     UCA0CTLW0 &= ~UCSWRST ; // release from reset
     UCA0TXBUF = 0x00; // Prime the Pump
     UCA0IE |= UCRXIE; // Enable RX interrupt
//------------------------------------------------------------------------------
}



#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){ //This interrupt is the interrupt relating to serial communication port UCA1
 //------------------------------------------------------------------------------
 // Interrupt name: eUSCI_A0_ISR
 // Description: This interrupt transmits and receives through UCA0
 //------------------------------------------------------------------------------
    char usb_value;
    switch(__even_in_range(UCA0IV,0x08)){
    case 0: break; //Vector 0 - no interrupt
    case 2:{ // Vector 2 - Rx1IFG
        usb_value = UCA0RXBUF;
        UCA1TXBUF = usb_value;

        IOT_2_PC[usb_rx_wr0++] = usb_value; // Add to Ring Buffer

        //look for carrot
        if(IOT_2_PC[usb_rx_wr0 - 2] == CARROT){
            Command_Pressed = 1;
            td_process_buffer[pb_row][pb_col] = IOT_2_PC[usb_rx_wr0 - 2];
            td_process_buffer[pb_row][pb_col+1] = IOT_2_PC[usb_rx_wr0 - 1];
            td_process_buffer[pb_row][pb_col+2] = IOT_2_PC[usb_rx_wr0];
            IOT_2_PC[usb_rx_wr0 - 2]  = ' ';
            IOT_2_PC[usb_rx_wr0 - 1]  = ' ';
            IOT_2_PC[usb_rx_wr0]  = ' ';

        }

        if(usb_rx_wr0 >= sizeof(IOT_2_PC)){
            usb_rx_wr0 = BEGINNING;
        }

        UCA1IE |= UCTXIE;
    }break;
    case 4:{ // Vector 4 - TXIFG
        if(IOT_Send == 1){
            UCA0TXBUF = PC_2_IOT[direct_iot0++];
            // IOT_Ring_Rx[direct_iot++]= 0;
            if(direct_iot0 >= sizeof(PC_2_IOT)){
                direct_iot0 = BEGINNING;
            }
            if(PC_2_IOT[direct_iot0-1] == 0){
                UCA0IE &= ~UCTXIE;
                direct_iot0 = 0;
                IOT_Send = 0;
            }
        }else{
            UCA0IE &= ~UCTXIE;
        }
        break;
    }
    default:
           break;
    }
 //------------------------------------------------------------------------------
}


#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){ //This interrupt is the interrupt relating to serial communication port UCA1
 //------------------------------------------------------------------------------
 // Interrupt name: eUSCI_A1_ISR
 // Description: This interrupt transmits and receives through UCA1
 //------------------------------------------------------------------------------
    char usb_value;
    switch(__even_in_range(UCA1IV,0x08)){
    case 0: break; //Vector 0 - no interrupt
    case 2:{ // Vector 2 - Rx1IFG
        usb_value = UCA1RXBUF;
        USB_2_PC[usb_rx_wr1++] = usb_value; // Add to Ring Buffer


        if(usb_rx_wr1 >= sizeof(USB_2_PC)){
            usb_rx_wr1 = BEGINNING;
        }
        UCA0TXBUF = usb_value;
        UCA0IE |= UCTXIE;

    }break;
    case 4:{ // Vector 4 - TXIFG
        /*UCA1TXBUF = PC_2_USB[direct_iot1++];
        // IOT_Ring_Rx[direct_iot++]= 0;
        if(direct_iot1 >= sizeof(PC_2_USB)){
            direct_iot1 = BEGINNING;
        }
        if(PC_2_USB[direct_iot1-1] == 0){
            UCA1IE &= ~UCTXIE;
            direct_iot1 = 0;
        }*/

        UCA1IE &= ~UCTXIE;

        break;

        }
    default:break;
    }
 //------------------------------------------------------------------------------
}

