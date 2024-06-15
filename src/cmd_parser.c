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
#include "main.h"
#include "epd.h"
#include "tl_common.h"
#include "stack/ble/ble.h"
#include "vendor/common/blt_common.h"

#include "time.h"
#include "flash.h"
#include "lime.h"

extern settings_struct settings;

#define testPin GPIO_PD3

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//                    BLE COMMAND DISPATCHER
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void cmd_parser(void *p) {

  rf_packet_att_data_t *req = (rf_packet_att_data_t *)p;
  uint8_t command = req->dat[0];

  switch (command) {

  case 0xFF: {
    /* Set test pins*/
    gpio_set_func(testPin, AS_GPIO);
    gpio_set_output_en(testPin, 1);
    gpio_set_input_en(testPin, 0);
    sleep_ms(500);

    break;
  }

  case 0xCC: {

    /* Set test pin */
    gpio_set_func(GPIO_PD2, AS_GPIO);
    gpio_set_output_en(GPIO_PD2, 1);
    gpio_set_input_en(GPIO_PD2, 0);
    sleep_ms(500);

    break;
  }

  case 0xDE: {
    // Save settings in flash to default
    reset_settings_to_default();
    save_settings_to_flash();
  }

  case 0xDF: {
    // Save current settings in flash
    save_settings_to_flash();

    break;
  }

  case 0xE0: {
    // force set an EPD model, if it wasnt detect
    // automatically correct
    set_EPD_model(req->dat[1]);
    break;
  }

  case 0xF1: {
    lime_set_battery_level(req->dat[1]);
    epd_display(0);

    break;
  }

  case 0xF2: {
    lime_set_speed(req->dat[1]);
    epd_display(0);

    break;
  }

  default:
    break;
  }
}
