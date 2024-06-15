/*
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * (C) Original Author: atc1441 <https://github.com/atc1441/ATC_TLSR_Paper>
 * (C) Author: iomonad <iomonad@riseup.net>
 */

#include <stdint.h>
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "drivers/8258/flash.h"
#include "uart.h"
#include "main.h"

void init_uart(void) {
  gpio_set_func(TXD, AS_GPIO);
  gpio_set_output_en(TXD, 1);
  gpio_write(TXD, 0);
  gpio_set_func(RXD, AS_GPIO);
  gpio_set_input_en(RXD, 1);
  gpio_set_output_en(RXD, 0);

  uart_gpio_set(UART_TX_PB1, UART_RX_PA0);
  uart_reset();
  uart_init(12, 15, PARITY_NONE, STOP_BIT_ONE); // baud rate: 115200
  uart_dma_enable(0, 0);
  dma_chn_irq_enable(0, 0);
  uart_irq_enable(0, 0);
  uart_ndma_irq_triglevel(0, 0);
}

_attribute_ram_code_ void puts(const char *str) {
  while (*str != '\0') {
    putchar_custom((char)*str);
    str++;
  }
}

int putchar_custom(int c) {
  uart_ndma_send_byte((char)c);
  while (uart_tx_is_busy()) {
    sleep_us(10);
  };
  return 0;
}
