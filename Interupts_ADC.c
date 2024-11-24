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

volatile unsigned int ADC_Channel;

volatile unsigned int ADC_Thumb;
volatile unsigned int ADC_Right_Det;
volatile unsigned int ADC_Left_Det;

extern volatile char display_line[4][11];
extern volatile unsigned char display_changed;
volatile char adc_char[4];
extern volatile unsigned char update_display;
extern unsigned volatile int undetected;



#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
     switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
     case ADCIV_NONE:
         break;
     case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
         // before its previous conversion result was read.
         break;
     case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
         break;
     case ADCIV_ADCHIIFG: // Window comparator interrupt flags
         break;
     case ADCIV_ADCLOIFG: // Window comparator interrupt flag
         break;
     case ADCIV_ADCINIFG: // Window comparator interrupt flag
         break;
     case ADCIV_ADCIFG: // ADCMEM0 memory register is loaded with the conversion result
         ADCCTL0 &= ~ADCENC; // Disable Conversion.
         switch (ADC_Channel++){
             case 0x00:{ // ADC_Thumb Wheel
                 ADC_Left_Det = ADCMEM0; // Channel A5
                 ADC_Left_Det = ADC_Left_Det >> 2;
//                 HEXtoBCD(ADC_Left_Det);
//                 adc_line(2,3);
//                 display_changed = TRUE;

                 //ADCCTL0 |= ADCSC; // Start next sample

                 ADCMCTL0 &= ~ADCINCH_2; // Last channel A2
                 ADCMCTL0 |= ADCINCH_3; // Next channel A3
             }break;
             case 0x01:{ // ADC_Right_Det
                 ADC_Right_Det = ADCMEM0; // Channel A3
                 ADC_Right_Det = ADC_Right_Det >> 2;
//                 HEXtoBCD(ADC_Right_Det);
//                 adc_line(1,3);
//                 display_changed = TRUE;

                 ADCMCTL0 &= ~ADCINCH_3; // Last channel A3
                 ADCMCTL0 |= ADCINCH_5; // Next channel A5

//                 ADCMCTL0 = ADCINCH_2; // Next channel A2
//                 ADCCTL0 |= ADCSC; // Start next sample
             }break;
             case 0x02:{ // ADC_Left_Det
                 ADC_Thumb = ADCMEM0; // Channel A2
                 ADC_Thumb = ADC_Thumb >> 2;
//                 HEXtoBCD(ADC_Thumb);
//                 adc_line(3,3);
//                 display_changed = TRUE;
                 ADCMCTL0 &= ~ADCINCH_5; // Last channel A5
                 ADCMCTL0 |= ADCINCH_2; // Next channel A2
                 // Do not start the next sample
                 ADC_Channel = 0;
             }break;
             default:
                 break;
         }
         ADCCTL0 |= ADCENC; // Enable Conversions
         break;
     default:
         break;
 }
}


//-----------------------------------------------------------------
// Hex to BCD Conversion
// Convert a Hex number to a BCD for display on an LCD or monitor
//
//-----------------------------------------------------------------
void HEXtoBCD(int hex_value){
    int value;
    int i;
    for(i=0; i < 4; i++) {
        adc_char[i] = '0';
    }
    value = 0;
    while (hex_value > 999){
        hex_value = hex_value - 1000;
        value = value + 1;
        adc_char[0] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 99){
        hex_value = hex_value - 100;
        value = value + 1;
        adc_char[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){
        hex_value = hex_value - 10;
        value = value + 1;
        adc_char[2] = 0x30 + value;
    }
    adc_char[3] = 0x30 + hex_value;
}
//-----------------------------------------------------------------
//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void adc_line(char line, char location){
//-------------------------------------------------------------
    int i;
    unsigned int real_line;
    real_line = line - 1;
    for(i=0; i < 4; i++) {
        display_line[real_line][i+location] = adc_char[i];
        update_display = TRUE;
    }
}
//-
