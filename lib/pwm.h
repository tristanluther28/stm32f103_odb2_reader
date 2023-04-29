//-----------------------------------------------------------------------------
//
//  pwm.h
//
//  Swallowtail PWM Firmware
//  STM32F103 PWM Firmware
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
#include <libopencm3/stm32/timer.h>

/******************* Global Defines *****************************/
static volatile int period;

/******************** Functions Definitions **************************/
void pwm_setup_timer(uint32_t peripheral, uint32_t freq, uint32_t prescaler);
void pwm_setup_output(uint32_t peripheral, enum tim_oc_id oc_id, uint32_t rcc_timer_clk, uint32_t rcc_gpio_clk, uint32_t gpio_port, uint16_t gpio_pin);
void pwm_start_timer(uint32_t peripheral);
void pwm_set_duty(uint32_t peripheral, enum tim_oc_id oc_id, float duty);