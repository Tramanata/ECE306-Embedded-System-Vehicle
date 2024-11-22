/*
 * macros.h
 *
 *  Created on: Sep 16, 2024
 *      Author: trama
 */

#ifndef MACROS_H_
#define MACROS_H_
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define FALSE                (0x00)
#define TB0CCR0_INTERVAL (50000) // 8,000,000 / 2 / 8 / (1 / 5msec)
#define TB0CCR1_INTERVAL (50000) // 8,000,000 / 2 / 8 / (1 / 5msec)
#define TB0CCR2_INTERVAL (50000) // 8,000,000 / 2 / 8 / (1 / 5msec)
#define OFF (0)
#define ON (0)
#define BEGINNING (0)

#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)

//#define RIGHT_FORWARD_SPEED     (TB3CCR2)
//#define LEFT_FORWARD_SPEED      (TB3CCR3)
//#define RIGHT_REVERSE_SPEED     (TB3CCR4)
//#define LEFT_REVERSE_SPEED      (TB3CCR5)
//#define WHEEL_OFF               (0)
#define SLOWER                    (6000)
#define SLOWEST                    (5000)
#define SLOW                    (7000)
#define SLOWISH             (7500)
#define FAST                    (50000)
//#define WHEEL_PERIOD           (50005)
#define PERCENT_100             (50000)
#define PERCENT_80              (45000)


#define START (1)
#define GO (2)
#define TURNING (3)
#define STOP (4)

#define ENABLE_IOT (0)
#define STOP_SAVE (1)
#define UPDATE_CONNECTIONS (2)
#define SERVER_PORT (3)
#define SSID (4)
#define GET_IP (5)

#define CARROT (0x5E)
#define CAP_F  (0x46)
#define LOW_F  (0x66)
#define CAP_R  (0x52)
#define CAP_T  (0x54)
#define LOW_T  (0x74)



#endif /* MACROS_H_ */
