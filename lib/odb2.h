//-----------------------------------------------------------------------------
//
//  odb2.h
//
//  Swallowtail ODB2 Firmware
//  STM32F103 ODB2 Firmware
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
#include <stddef.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/cm3/nvic.h>

/******************* Global Defines *****************************/


struct can_tx_msg {
	uint32_t std_id;
	uint32_t ext_id;
	uint8_t ide;
	uint8_t rtr;
	uint8_t dlc;
	uint8_t data[8];
};

struct can_rx_msg {
	uint32_t std_id;
	uint32_t ext_id;
	uint8_t ide;
	uint8_t rtr;
	uint8_t dlc;
	uint8_t data[8];
	uint8_t fmi;
};

//CAN Interface GPIO
#define CAN_PORT GPIOB
#define CAN_H GPIO8
#define CAN_L GPIO9
#define DUMMY 0x55

//Frame Identifiers
#define REQUEST 0x7DF
#define RESPONSE 0x7E8

//Modes
#define SHOW_CURRENT 0x01
#define SHOW_FREEZE 0x02
#define SHOW_STORED_CODES 0x03
#define CLEAR 0x04
#define VEHICLE_INFO 0x09

//PIDs
#define VEHICLE_SPEED 0x0D
#define THROTTLE_POS 0x11
#define FUEL_LEVEL 0x2F
#define ODOMETER 0xA6


/******************** Functions Definitions **************************/
void odb2_init(void);
void odb2_request(uint8_t code);
uint16_t odb2_receive(void);
