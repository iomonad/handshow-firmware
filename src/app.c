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
#include "app.h"
#include "main.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "vendor/common/blt_common.h"

#include "battery.h"
#include "ble.h"
#include "flash.h"
#include "ota.h"
#include "epd.h"
#include "time.h"
#include "bart_tif.h"

RAM uint8_t battery_level;
RAM uint16_t battery_mv;
RAM int16_t temperature;

RAM uint8_t hour_refresh = 100;
RAM uint8_t minute_refresh = 100;

// Settings
extern settings_struct settings;

_attribute_ram_code_ void
user_init_normal(void) {   // this will get executed one time after power up
  random_generator_init(); // must
  init_time();
  init_ble();
  init_flash();
  init_nfc();
}

_attribute_ram_code_ void
user_init_deepRetn(void) { // after sleep this will get executed
  blc_ll_initBasicMCU();
  rf_set_power_level_index(RF_POWER_P3p01dBm);
  blc_ll_recoverDeepRetention();
}

/* Main loop computed */
_attribute_ram_code_ void main_loop(void) {
  /* Stack documentation:
     -
     https://wiki.telink-semi.cn/doc/an/AN_17092700-E_Telink%20826x%20BLE%20SDK%20Developer%20Handbook.pdf
   */
  blt_sdk_main_loop();
  handler_time();

  /* State updaters */
  if (time_reached_period(Timer_CH_1, 30)) {
    battery_mv = get_battery_mv();
    battery_level = get_battery_level(battery_mv);
    temperature = get_temperature_c();
    set_adv_data(EPD_read_temp() * 10, battery_level, battery_mv);
    ble_send_battery(battery_level);
    ble_send_temp(EPD_read_temp() * 10);
  }

  /* Screen Write Dispatcher */
  uint8_t current_minute = (get_time() / 60) % 60;
  if (current_minute != minute_refresh) {
    minute_refresh = current_minute;
    uint8_t current_hour = ((get_time() / 60) / 60) % 24;
    if (current_hour != hour_refresh) {
      hour_refresh = current_hour;
      epd_display(1);
    } else {
      epd_display(0);
    }
  }

  if (time_reached_period(Timer_CH_0, settings.refresh_timer)) {
    if (ble_get_connected()) {
      set_led_color(3);
    } else {
      set_led_color(2);
    }

    WaitMs(1);
    set_led_color(0);
  }

  // if epd_update is ongoing enable gpio wakeup to put
  // the display to sleep as fast as possible
  if (epd_state_handler()) {
    cpu_set_gpio_wakeup(EPD_BUSY, 1, 1);
    bls_pm_setWakeupSource(PM_WAKEUP_PAD);
    bls_pm_setSuspendMask(SUSPEND_DISABLE);
  } else {
    blt_pm_proc();
  }
}
