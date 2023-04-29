//-----------------------------------------------------------------------------
//
//  odb2.c
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

/******************** Include ***************************/
#include "odb2.h"

/******************** Functions **************************/
void odb2_init(){
    rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_CAN1);

    AFIO_MAPR |= AFIO_MAPR_CAN1_REMAP_PORTB;

    /* Configure CAN pin: RX (input pull-up). */
	gpio_set_mode(GPIO_BANK_CAN1_PB_RX, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_CAN1_PB_RX);
	gpio_set(GPIO_BANK_CAN1_PB_RX, GPIO_CAN1_PB_RX);

	/* Configure CAN pin: TX. */
	gpio_set_mode(GPIO_BANK_CAN1_PB_TX, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN1_PB_TX);

    //Set up NVIC
    nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
	nvic_set_priority(NVIC_USB_LP_CAN_RX0_IRQ, 1);

    //Reset CAN
    can_reset(CAN1);

    /* CAN cell init.
	 * Setting the bitrate to 500kBit. APB1 = 24MHz, 
	 * prescaler = 4 -> 6MHz time quanta frequency.
	 * 1tq sync + 8tq bit segment1 (TS1) + 1tq bit segment2 (TS2) = 
	 * 16time quanto per bit period, therefor 6MHz/12 = 500kHz
	 */
    can_init(CAN1, 
            false,  //TTCM: Time triggered comm mode?
            true,   //ABOM: Automatic bus-off management?
            false,  //AWUM: Automatic wakeup mode?
            true,   //NART: No automatic retransmission?
            false,  //RFLM: Receive FIFO locked mode?
            false,  //TXFP: Transmit FIFO priority?
            CAN_BTR_SJW_1TQ,
		    CAN_BTR_TS1_8TQ,
		    CAN_BTR_TS2_3TQ,
            4,
		    false,
            false
    );

    /* CAN filter 0 init. */
	can_filter_id_mask_32bit_init(
                0,     /* Filter ID */
                RESPONSE,     /* CAN ID */
                0,     /* CAN ID mask */
                0,     /* FIFO assignment (here: FIFO0) */
                true); /* Enable the filter. */

    // Enable CAN RX interrupt.
	can_enable_irq(CAN1, CAN_IER_FMPIE0);

    return;
}

uint16_t odb2_receive(void){
    //Wait for the response
    uint32_t id;
	bool ext, rtr;
	uint8_t fmi, length, resp_data[8];

	can_receive(CAN1, 0, false, &id, &ext, &rtr, &fmi, &length, resp_data, NULL);

    uint16_t value = 0x00;

    if(id == RESPONSE){
        //Check this was the requested data
        switch(resp_data[2]){
            case VEHICLE_SPEED:
                //We have the data, convert to mph
                value =  (uint16_t)resp_data[3]*0.621371;
                break;
            case FUEL_LEVEL:
                //TODO
                value = 0x04;
                break;
            case THROTTLE_POS:
                //TODO
                value = 0x03;
                break;
            case ODOMETER:
                //TODO
                value = 0x02;
                break;
            default:
                value = 0x01; //Given unknown code
        }
    }
    else{
        //Error, incorrect response id/send zero
        value = 0x00;
    }

    can_fifo_release(CAN1, 0);

    return value;
}

void odb2_request(uint8_t code){
    static uint8_t data[8] = {0x02, SHOW_CURRENT, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
    data[2] = code;

    //Request the given resource
    can_transmit(CAN1,
			 REQUEST,     /* (EX/ST)ID: CAN ID */
			 false, /* IDE: CAN ID extended? */
			 false, /* RTR: Request transmit? */
			 8,     /* DLC: Data length */
			 data);
    return;
}
