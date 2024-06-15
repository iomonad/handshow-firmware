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

#pragma once

typedef struct Settings_struct {
  uint32_t magic;
  uint32_t len;
  uint8_t temp_C_or_F;
  uint8_t advertising_temp_C_or_F;
  uint8_t blinking_smiley;
  uint8_t comfort_smiley;
  uint8_t show_batt_enabled;
  uint8_t advertising_interval; // advise new values - multiply by 10 for value
  uint8_t measure_interval; // time = loop interval * factor (def: about 7 * X)
  int8_t temp_offset;
  uint8_t temp_alarm_point; // divide by ten for value
  uint8_t refresh_timer;

  /* WARNING */
  // Needs to be at the last position otherwise the settings can
  // not be validated on next boot!!!!
  uint8_t crc;
} settings_struct;

void init_flash(void);
void reset_settings_to_default(void);
void save_settings_to_flash(void);
uint8_t get_crc(void);
