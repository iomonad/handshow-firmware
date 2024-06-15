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
#include "nfc.h"
#include "main.h"

uint8_t nfc_reset[] = {0x03, 0xb5, 0xa0};

_attribute_ram_code_ void init_nfc(void) {
  gpio_write(NFC_CS, 1);
  gpio_set_func(NFC_CS, AS_GPIO);
  gpio_set_output_en(NFC_CS, 1);
  gpio_set_input_en(NFC_CS, 0);
  gpio_setup_up_down_resistor(NFC_CS, PM_PIN_PULLUP_10K);

  gpio_set_func(NFC_IRQ, AS_GPIO);
  gpio_set_output_en(NFC_IRQ, 0);
  gpio_set_input_en(NFC_IRQ, 1);
  gpio_setup_up_down_resistor(NFC_IRQ, PM_PIN_PULLUP_10K);

  gpio_write(NFC_CS, 0);
  sleep_us(500);
  send_i2c(0xae, nfc_reset, sizeof(nfc_reset));
  gpio_write(NFC_CS, 1);
}
