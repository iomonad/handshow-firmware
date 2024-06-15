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
#include "led.h"
#include "main.h"

_attribute_ram_code_ void init_led(void) {
  gpio_setup_up_down_resistor(LED_BLUE, PM_PIN_PULLUP_1M);
  gpio_write(LED_BLUE, 1);
  gpio_set_func(LED_BLUE, AS_GPIO);
  gpio_set_output_en(LED_BLUE, 1);
  gpio_set_input_en(LED_BLUE, 0);

  gpio_write(LED_RED, 1);
  gpio_setup_up_down_resistor(LED_RED, PM_PIN_PULLUP_1M);
  gpio_set_func(LED_RED, AS_GPIO);
  gpio_set_output_en(LED_RED, 1);
  gpio_set_input_en(LED_RED, 0);

  gpio_setup_up_down_resistor(LED_GREEN, PM_PIN_PULLUP_1M);
  gpio_write(LED_GREEN, 1);
  gpio_set_func(LED_GREEN, AS_GPIO);
  gpio_set_output_en(LED_GREEN, 1);
  gpio_set_input_en(LED_GREEN, 0);
}

_attribute_ram_code_ void set_led_color(uint8_t color) {
  switch (color) {
  case 1:
    gpio_write(LED_BLUE, 1);
    gpio_write(LED_RED, 0);
    gpio_write(LED_GREEN, 1);
    break;
  case 2:
    gpio_write(LED_BLUE, 1);
    gpio_write(LED_RED, 1);
    gpio_write(LED_GREEN, 0);
    break;
  case 3:
    gpio_write(LED_BLUE, 0);
    gpio_write(LED_RED, 1);
    gpio_write(LED_GREEN, 1);
    break;
  default:
    gpio_write(LED_BLUE, 1);
    gpio_write(LED_RED, 1);
    gpio_write(LED_GREEN, 1);
    break;
  }
}
