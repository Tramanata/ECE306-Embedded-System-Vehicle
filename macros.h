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
#define TB0CCR0_INTERVAL (2500) // 8,000,000 / 2 / 8 / (1 / 5msec)
#define TB0CCR1_INTERVAL (2500) // 8,000,000 / 2 / 8 / (1 / 5msec)
#define TB0CCR2_INTERVAL (2500) // 8,000,000 / 2 / 8 / (1 / 5msec)

//#define USE_GPIO (0x00)
//#define USE_SMCLK (0x01)


#endif /* MACROS_H_ */
