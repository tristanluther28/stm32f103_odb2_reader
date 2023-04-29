//-----------------------------------------------------------------------------
//
//  ltc4627.h
//
//  Swallowtail LTC-4627 Firmware
//  Quad Seven Segment Display Firmware
//
//  Copyright (c) 2023 Swallowtail Electronics
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sub-license,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//
//  Web:    http://swallowtailelectronics.com
//  Email:  tluther@swallowtailelectronics.com
//
//-----------------------------------------------------------------------------

/******************** Macros *****************************/
#ifndef F_CPU
    #define F_CPU 48000000
#endif

/******************** Includes ***************************/
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "pwm.h"
#include "spi.h"

/******************* Global Defines *****************************/

#define LTC4627_CHAR_BLANK       0xFF

//MUX Interface GPIO
#define MUX_PORT GPIOA
#define A GPIO9
#define B GPIO10
#define C GPIO0
#define PWM GPIO1

#define LTC4627_DIGIT0            0x08
#define LTC4627_DIGIT1            0x05
#define LTC4627_DIGIT2            0x01
#define LTC4627_DIGIT3            0x00
#define LTC4627_L				  0x04

//Seven Segment Connections
#define A_L1 PD0
#define B_L2 PD1
#define C_L3 PD2
#define D PD3
#define E PD4
#define F PD5
#define G PD6
#define DP PD7
#define LTC4627_ONE 0xF9
#define LTC4627_TWO 0xA4
#define LTC4627_THREE 0xB0
#define LTC4627_FOUR 0x99
#define LTC4627_FIVE 0x92
#define LTC4627_SIX 0x82
#define LTC4627_SEVEN 0xF8
#define LTC4627_EIGHT 0x80
#define LTC4627_NINE 0x90
#define LTC4627_ZERO 0xC0
#define LTC4627_C 0xC6
#define LTC4627_F 0x8E
#define LTC4627_K 0x85
#define LTC4627_L3 0x03

enum tempunits {KELVIN, CELSUIS, FAHRENHEIT};

/******************** Functions Definitions **************************/
void ltc4627_init(void);
void ltc4627_set_digit(uint8_t value, uint8_t char_num);
// void ltc4627_set_time(uint8_t hours, uint8_t minute);
//void ltc4627_set_temp(float temp_val, enum tempunits units);
void ltc4627_set_number(uint16_t value);
void ltc4627_set_brightness(uint8_t value);