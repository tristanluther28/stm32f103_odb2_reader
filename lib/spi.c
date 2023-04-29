//-----------------------------------------------------------------------------
//
//  spi.c
//
//  Swallowtail SPI Firmware
//  STM32F103 SPI Firmware
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
#include "spi.h"

/******************** Functions **************************/
void spi_setup() {

  /* Configure GPIOs: SS=PA4, SCK=PA5, MISO=PA6 and MOSI=PA7 */
  gpio_set_mode(SPI_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, MOSI | SCK | CS );

  gpio_set_mode(SPI_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, MISO);

  /* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
  rcc_periph_reset_pulse(RST_SPI1);

  /* Set up SPI in Master mode with:
   * Clock baud rate: 1/64 of peripheral clock frequency
   * Clock polarity: Idle High
   * Clock phase: Data valid on 2nd clock pulse
   * Data frame format: 8-bit
   * Frame format: MSB First
   */
  spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

  /*
   * Set NSS management to software.
   *
   * Note:
   * Setting nss high is very important, even if we are controlling the GPIO
   * ourselves this bit needs to be at least set to 1, otherwise the spi
   * peripheral will not send any data out.
   */
  //spi_enable_ss_output(SPI1);
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);

  /* Enable SPI1 periph. */
  spi_enable(SPI1);

  return;
}

void spi_cs_enable(){
    spi_enable_ss_output(SPI1);
    gpio_clear(SPI_PORT, CS);
}

void spi_cs_disable(){
    spi_disable_ss_output(SPI1);
    gpio_set(SPI_PORT, CS);
}

uint8_t spi_transfer(uint8_t data){
    spi_send(SPI1, (uint8_t) data);
	// Read the byte that just came in (use a loopback between MISO and MOSI to get the same byte back)
    return spi_read(SPI1);
}