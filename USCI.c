#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"
#include "stdio.h"


// Global Variables
char process_buffer[25]; // Size for appropriate Command Length
char pb_index; // Index for process_buffer
char USB_Ring_Rx[11];
char IOT_Ring_Rx[11];
unsigned volatile int usb_rx_wr = 0;
unsigned volatile int iot_rx_wr = 0;
unsigned volatile int direct_iot = 0;
unsigned volatile int usb_rx_wr_temp;
unsigned volatile int usb_rx_rd = 0;
unsigned volatile int i = 0;
extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
unsigned volatile int RING_UPDATED;
extern unsigned volatile int start_transmit;
unsigned volatile int transmit_received;


void readRecieved(void){
    usb_rx_wr_temp = usb_rx_wr;
    if(usb_rx_wr_temp != usb_rx_rd){
        display_changed = TRUE;
        update_display = TRUE;
        process_buffer[i] = USB_Ring_Rx[usb_rx_rd++];
        if(process_buffer[i] == 13){
            strcpy(IOT_Ring_Rx, process_buffer);
            strcpy(display_line[0], "Received  ");
            strcpy(display_line[3], process_buffer);
            strcpy(display_line[1], "          ");
            //reset buffer and print the string

//            display_changed = TRUE;
//            update_display = TRUE;
            //strcpy(display_line[2], "Isthiswork");


            i = 0;
        }
        i++;
    }
    if(usb_rx_rd >= sizeof(USB_Ring_Rx)){
        usb_rx_rd = BEGINNING;
    }
}



void USCI_A1_transmit(void){ // Transmit Function for USCI_A1

    // Contents must be in process_buffer
    // End of Transmission is identified by NULL character in process_buffer
    // process_buffer includes Carriage Return and Line Feed
    //strcpy(IOT_Ring_Rx, "Hello");
    strcpy(IOT_Ring_Rx, process_buffer);
    strcpy(display_line[1], "          ");
    strcpy(display_line[1], IOT_Ring_Rx);
    strcpy(process_buffer, "          ");
    i = 0;
    //strcpy(display_line[1], IOT_Ring_Rx);
    //pb_index = 0; // Set Array index to first location
     UCA1IE |= UCTXIE; // Enable TX interrupt

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
        USB_Ring_Rx[usb_rx_wr++] = usb_value; // Add to Ring Buffer

        if(usb_rx_wr >= sizeof(USB_Ring_Rx)){
            usb_rx_wr = BEGINNING;
        }
        //UCA1TXBUF = usb_value;
        // UCA1IE |= UCTXIE;
    }break;
    case 4:{ // Vector 4 - TX1IFG
        UCA1TXBUF = IOT_Ring_Rx[direct_iot++];
        // IOT_Ring_Rx[direct_iot++]= 0;
        if(direct_iot >= sizeof(IOT_Ring_Rx)){
            direct_iot = BEGINNING;
        }
        if(IOT_Ring_Rx[direct_iot-1] == 0){
            UCA1IE &= ~UCTXIE;
            direct_iot = 0;
        }
    }break;
    default:
        break;
    }
 //------------------------------------------------------------------------------
}

