//-----------------------------------------------------------------------------
//
//  pwm.c
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

/******************** Include ***************************/
#include "pwm.h"

/******************** Functions **************************/
void pwm_setup_timer(uint32_t peripheral, uint32_t freq, uint32_t prescaler){
    if (prescaler > 0){
        period = (F_CPU/(prescaler*2))/freq;
    }
    else{
        period = (F_CPU)/freq;
    }
    //For a given timer, have no clock divide, center alignment, count up
    timer_set_mode(peripheral, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
    //1MHz prescaler for a 48MHz clock 
    timer_set_prescaler(peripheral, prescaler/2);
    timer_enable_preload(peripheral);
    //Set the clock timer period, requires prescaler to be set correctly
    timer_set_period(peripheral, period);
    //Allow the timer to run continously
    timer_continuous_mode(peripheral);
    timer_set_repetition_counter(peripheral, 0);
    return;
}

void pwm_setup_output(uint32_t peripheral, enum tim_oc_id oc_id, uint32_t rcc_timer_clk, uint32_t rcc_gpio_clk, uint32_t gpio_port, uint16_t gpio_pin){
    //Enable clock
    rcc_periph_clock_enable(rcc_timer_clk);
    //Enable GPIO, Alternate function clocks
    rcc_periph_clock_enable(rcc_gpio_clk);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_set_mode(gpio_port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, gpio_pin);

    timer_disable_oc_output(peripheral, oc_id);
    //Output active when counter is lt compare register
    timer_set_oc_mode(peripheral, oc_id, TIM_OCM_PWM1);
    //Set the duty cycle of the output
    timer_set_oc_value(peripheral, oc_id, 0);
    //Enable timer output compare
    timer_enable_oc_output(peripheral, oc_id);
    return;
}

void pwm_start_timer(uint32_t peripheral){
    //Turn the counter perhipheral on, last step!
    timer_enable_counter(peripheral);
    return;
}

void pwm_set_duty(uint32_t peripheral, enum tim_oc_id oc_id, float duty){
    //Set the duty cycle of the output
    timer_set_oc_value(peripheral, oc_id, period * (duty/100));
    return;
}

/******************** Interrupt Service Routines *********/