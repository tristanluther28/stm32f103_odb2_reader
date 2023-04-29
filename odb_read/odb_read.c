//-----------------------------------------------------------------------------
//
//  clock.c
//
//  Swallowtail Clock Demo Firmware
//  STM32F103 Clock Demo Firmware
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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <stdio.h>
#include <errno.h>

#define F_CPU 48000000

#include "ltc4627.h"
#include "odb2.h"

volatile uint16_t speed = 0x00;

static void clock_setup(void)
{
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_48MHZ]);

	/* Enable TIM2 Clock */
	rcc_periph_clock_enable(RCC_TIM2);

	/* Enable GPIOA, GPIOB, GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_AFIO);

	/* Enable SPI1 Periph and gpio clocks */
	rcc_periph_clock_enable(RCC_SPI1);

}

static void systick_setup(void){
	/* 48MHz / 8 => 6000000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	/* 6000000/18000000 = 0.33 overflows per second - every 2s one interrupt */
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(17999999);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();
}

void sys_tick_handler(void){
	__asm__("nop");
	odb2_request(VEHICLE_SPEED);
}

void usb_lp_can_rx0_isr(void)
{
	speed = odb2_receive();	
}

int main(void) {
	//Init all peripherals
  	clock_setup();
	systick_setup();
	ltc4627_init();
	odb2_init();

	while(1){
		//Hold & Wait
		__asm__("nop");
		//Send to the display
		ltc4627_set_number((uint16_t)speed);

	}

	return 0;
}
