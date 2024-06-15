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

RAM bool i2c_sending;

void init_i2c(void) {
  i2c_gpio_set(I2C_GPIO_GROUP_C0C1);
  i2c_master_init(0x78, (uint8_t)(CLOCK_SYS_CLOCK_HZ / (4 * 400000)));
}

void send_i2c(uint8_t device_id, uint8_t *buffer, int dataLen) {
  if (i2c_sending)
    return;
  i2c_sending = true;
  i2c_set_id(device_id);
  i2c_write_series(0, 0, (uint8_t *)buffer, dataLen);
  i2c_sending = false;
}

uint8_t test_i2c_device(uint8_t address) {
  reg_i2c_id = address << 1;
  reg_i2c_ctrl = FLD_I2C_CMD_START | FLD_I2C_CMD_ID;
  while (reg_i2c_status & FLD_I2C_CMD_BUSY)
    ;
  reg_i2c_ctrl = FLD_I2C_CMD_STOP;
  while (reg_i2c_status & FLD_I2C_CMD_BUSY)
    ;

  return (reg_i2c_status & FLD_I2C_NAK) ? 0 : 1;
}
