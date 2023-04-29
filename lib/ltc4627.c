//-----------------------------------------------------------------------------
//
//  ltc4627.c
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

/******************** Include ***************************/
#include "ltc4627.h"

const enum tim_oc_id oc_id = TIM_OC2; //Output compare channel designator

/******************** Functions **************************/
void ltc4627_init(){
    //Init the GPIO
    gpio_set_mode(MUX_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, A | B | C | PWM);

    //Init SPI
    spi_setup();

    //Init the PWM
    pwm_setup_output(TIM2, oc_id, RCC_TIM2, RCC_GPIOA, GPIOA, GPIO_TIM2_CH2);
    pwm_setup_timer(TIM2, 400, 48);
    pwm_start_timer(TIM2);
    pwm_set_duty(TIM2, oc_id, 0); //0% Duty Cycle
}

void ltc4627_set_digit(uint8_t value, uint8_t char_num){
    //Push the data to the seven segment port
    spi_cs_enable();
	switch(value){
		case 0:
			spi_transfer(LTC4627_ZERO);
			break;
		case 1:
			spi_transfer(LTC4627_ONE);
			break;
		case 2:
			spi_transfer(LTC4627_TWO);
			break;
		case 3:
			spi_transfer(LTC4627_THREE);
			break;
		case 4:
			spi_transfer(LTC4627_FOUR);
			break;
		case 5:
			spi_transfer(LTC4627_FIVE);
			break;
		case 6:
			spi_transfer(LTC4627_SIX);
			break;
		case 7:
			spi_transfer(LTC4627_SEVEN);
			break;
		case 8:
			spi_transfer(LTC4627_EIGHT);
			break;
		case 9:
			spi_transfer(LTC4627_NINE);
			break;
		case LTC4627_CHAR_BLANK:
			spi_transfer(LTC4627_CHAR_BLANK);
		    break;
		case 10:
			spi_transfer(LTC4627_L3);
		    break;
		case 70:
			spi_transfer(LTC4627_F);
		    break;
		case 43:
			spi_transfer(LTC4627_C);
		    break;
		case 75:
			spi_transfer(LTC4627_K);
		    break;
		default:
			spi_transfer(LTC4627_ZERO);
			break;
	}
    spi_cs_disable();

    //Push the digit info to the 74HC138
	switch(char_num){
		case 0:
            gpio_clear(MUX_PORT, A);
            gpio_clear(MUX_PORT, B);
            gpio_clear(MUX_PORT, C);
			break;
		case 1:
			gpio_set(MUX_PORT, A);
            gpio_clear(MUX_PORT, B);
            gpio_clear(MUX_PORT, C);
			break;
		case 2:
			gpio_set(MUX_PORT, A);
            gpio_set(MUX_PORT, B);
            gpio_clear(MUX_PORT, C);
			break;
		case 3:
			gpio_clear(MUX_PORT, A);
            gpio_clear(MUX_PORT, B);
            gpio_set(MUX_PORT, C);
			break;
		default:
			gpio_clear(MUX_PORT, A);
            gpio_set(MUX_PORT, B);
            gpio_clear(MUX_PORT, C);
			break;
	}

    //Delay for a second so it renders
    for (int j = 0; j < 5000; j++) {
		__asm__("nop");
	}
}

// void ltc4627_set_time(uint8_t hours, uint8_t minute){
    
//     return;
// }

// void ltc4627_set_temp(float temp_val, enum tempunits units){

//     return;
// }

void ltc4627_set_number(uint16_t value){
    //Get the thousands place
	uint8_t thousands = ((value / 1000) % 10);
	//Get the hundreds place if applicable
	uint8_t hundreds = ((value / 100) % 10);
	//Get the tens place
	uint8_t tens = ((value / 10) % 10);
	//Get the ones place
	uint8_t ones = ((value / 1) % 10);
	//Push those numbers to their respective places on the display
	if(thousands == 0){
		//If the hundreds is zero then print a blank
		ltc4627_set_digit(LTC4627_CHAR_BLANK, 3);
	}
	else{
		//Otherwise show it
        ltc4627_set_digit(thousands, 3);
	}
	if(hundreds == 0 && thousands == 0){
		//If the tens is zero as well as the hundreds then print a blank
		ltc4627_set_digit(LTC4627_CHAR_BLANK, 2);
	}
	else{
		//Otherwise show it
		ltc4627_set_digit(hundreds, 2);
	}
	if(tens == 0 && hundreds == 0 && thousands == 0){
		//If the tens is zero as well as the hundreds then print a blank
		ltc4627_set_digit(LTC4627_CHAR_BLANK, 1);
	}
	else{
		//Otherwise show it
		ltc4627_set_digit(tens, 1);
	}
	ltc4627_set_digit(ones, 0);
	
	return; //Return to call point
}

void ltc4627_set_brightness(uint8_t value){
    pwm_set_duty(TIM2, oc_id, value); //Change Duty Cycle
    return;
}