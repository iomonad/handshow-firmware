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
#include "main.h"
#include "drivers.h"
#include "vendor/common/user_config.h"
#include "app_config.h"
#include "drivers/8258/gpio_8258.h"

#include "flash.h"

#define MAGIC_WORD 0xABCFF123

RAM settings_struct settings;

void init_flash(void) {
  flash_read_page(0x78100, sizeof(settings), (uint8_t *)&settings);

  if ((settings.magic != MAGIC_WORD) | (settings.crc != get_crc()) |
      (settings.len != sizeof(settings))) {
    reset_settings_to_default();
    save_settings_to_flash();
  }
}

void reset_settings_to_default(void) {
  settings.magic = MAGIC_WORD;
  settings.len = sizeof(settings_struct);

  settings.temp_C_or_F = false;
  settings.advertising_temp_C_or_F = false;
  settings.blinking_smiley = false;
  settings.comfort_smiley = true;
  settings.show_batt_enabled = true;
  settings.advertising_interval = 6;
  settings.measure_interval = 10;
  settings.temp_offset = 0;
  settings.temp_alarm_point = 5;
  settings.refresh_timer = 10;
}

void save_settings_to_flash(void) {
  settings.crc = get_crc();
  flash_erase_sector(0x78100);
  flash_write_page(0x78100, sizeof(settings_struct), (uint8_t *)&settings);
}

uint8_t get_crc(void) {
  uint8_t temp_crc = 0x00;

  for (int i = 0; i < sizeof(settings_struct) - 1;
       i++) // Iterate over everything expect the last value as it is CRC itself
  {
    temp_crc = temp_crc ^ ((uint8_t *)&settings)[i];
  }
  return temp_crc;
}
