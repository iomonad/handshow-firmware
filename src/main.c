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
#include "vendor/common/user_config.h"
#include "drivers/8258/gpio_8258.h"
#include "app_config.h"
#include "main.h"
#include "app.h"
#include "battery.h"
#include "ble.h"
#include "cmd_parser.h"
#include "epd.h"
#include "flash.h"
#include "i2c.h"
#include "led.h"
#include "nfc.h"
#include "ota.h"
#include "uart.h"

_attribute_ram_code_ __attribute__((optimize("-Os"))) void irq_handler(void) {
  irq_blt_sdk_handler();
}

_attribute_ram_code_ int main(void) // must run in ramcode
{
  blc_pm_select_internal_32k_crystal();
  cpu_wakeup_init();
  int deepRetWakeUp =
      pm_is_MCU_deepRetentionWakeup(); // MCU deep retention wakeUp
  rf_drv_init(RF_MODE_BLE_1M);
  gpio_init(!deepRetWakeUp); // analog resistance will keep available in
                             // deepSleep mode, so no need initialize again
#if (CLOCK_SYS_CLOCK_HZ == 16000000)
  clock_init(SYS_CLK_16M_Crystal);
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
  clock_init(SYS_CLK_24M_Crystal);
#endif
  blc_app_loadCustomizedParameters();

  init_led();
  init_uart();
  init_i2c();

  if (deepRetWakeUp) {
    user_init_deepRetn();
    printf("\r\n\r\n\r\nBooting deep\r\n\r\n\r\n\r\n");
  } else {
    printf("\r\n\r\n\r\nBooting\r\n\r\n\r\n\r\n");
    user_init_normal();
  }
  irq_enable();
  while (1) {
    main_loop();
  }
}
