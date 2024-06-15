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
#include "epd.h"
#include "epd_spi.h"
#include "drivers.h"
#include "stack/ble/ble.h"

_attribute_ram_code_ void EPD_init(void) {
  gpio_set_func(EPD_RESET, AS_GPIO);
  gpio_set_output_en(EPD_RESET, 1);
  gpio_setup_up_down_resistor(EPD_RESET, PM_PIN_PULLUP_1M);

  gpio_set_func(EPD_DC, AS_GPIO);
  gpio_set_output_en(EPD_DC, 1);
  gpio_setup_up_down_resistor(EPD_DC, PM_PIN_PULLUP_1M);

  gpio_set_func(EPD_BUSY, AS_GPIO);
  gpio_set_output_en(EPD_BUSY, 0);
  gpio_set_input_en(EPD_BUSY, 1);
  gpio_setup_up_down_resistor(EPD_BUSY, PM_PIN_PULLUP_1M);

  gpio_set_func(EPD_CS, AS_GPIO);
  gpio_set_output_en(EPD_CS, 1);
  gpio_setup_up_down_resistor(EPD_CS, PM_PIN_PULLUP_1M);

  gpio_set_func(EPD_CLK, AS_GPIO);
  gpio_set_output_en(EPD_CLK, 1);
  gpio_setup_up_down_resistor(EPD_CLK, PM_PIN_PULLUP_1M);

  gpio_set_func(EPD_MOSI, AS_GPIO);
  gpio_set_output_en(EPD_MOSI, 1);
  gpio_setup_up_down_resistor(EPD_MOSI, PM_PIN_PULLUP_1M);

  gpio_set_output_en(EPD_ENABLE, 0);
  gpio_set_input_en(EPD_ENABLE, 1);
  gpio_setup_up_down_resistor(EPD_ENABLE, PM_PIN_PULLUP_1M);
}

_attribute_ram_code_ void EPD_SPI_Write(unsigned char value) {
  unsigned char i;

  WaitUs(10);
  for (i = 0; i < 8; i++) {
    gpio_write(EPD_CLK, 0);
    if (value & 0x80) {
      gpio_write(EPD_MOSI, 1);
    } else {
      gpio_write(EPD_MOSI, 0);
    }
    value = (value << 1);
    gpio_write(EPD_CLK, 1);
  }
}

_attribute_ram_code_ uint8_t EPD_SPI_read(void) {
  unsigned char i;
  uint8_t value = 0;

  gpio_shutdown(EPD_MOSI);
  gpio_set_output_en(EPD_MOSI, 0);
  gpio_set_input_en(EPD_MOSI, 1);
  gpio_write(EPD_CS, 0);
  EPD_ENABLE_WRITE_DATA();
  WaitUs(10);
  for (i = 0; i < 8; i++) {
    gpio_write(EPD_CLK, 0);
    gpio_write(EPD_CLK, 1);
    value <<= 1;
    if (gpio_read(EPD_MOSI) != 0) {
      value |= 1;
    }
  }
  gpio_set_output_en(EPD_MOSI, 1);
  gpio_set_input_en(EPD_MOSI, 0);
  gpio_write(EPD_CS, 1);
  return value;
}

_attribute_ram_code_ void EPD_WriteCmd(unsigned char cmd) {
  gpio_write(EPD_CS, 0);
  EPD_ENABLE_WRITE_CMD();
  EPD_SPI_Write(cmd);
  gpio_write(EPD_CS, 1);
}

_attribute_ram_code_ void EPD_WriteData(unsigned char data) {
  gpio_write(EPD_CS, 0);
  EPD_ENABLE_WRITE_DATA();
  EPD_SPI_Write(data);
  gpio_write(EPD_CS, 1);
}

_attribute_ram_code_ void EPD_CheckStatus(int max_ms) {
  unsigned long timeout_start = clock_time();
  unsigned long timeout_ticks = max_ms * CLOCK_16M_SYS_TIMER_CLK_1MS;
  WaitMs(1);
  while (EPD_IS_BUSY()) {
    if (clock_time() - timeout_start >= timeout_ticks)
      return; // Here we had a timeout
  }
}

_attribute_ram_code_ void EPD_CheckStatus_inverted(int max_ms) {
  unsigned long timeout_start = clock_time();
  unsigned long timeout_ticks = max_ms * CLOCK_16M_SYS_TIMER_CLK_1MS;
  WaitMs(1);
  while (!EPD_IS_BUSY()) {
    if (clock_time() - timeout_start >= timeout_ticks)
      return; // Here we had a timeout
  }
}

_attribute_ram_code_ void EPD_send_lut(uint8_t lut[], int len) {
  EPD_WriteCmd(lut[0]);
  for (int r = 1; r <= len; r++) {
    EPD_WriteData(lut[r]);
  }
}

_attribute_ram_code_ void EPD_send_empty_lut(uint8_t lut, int len) {
  EPD_WriteCmd(lut);
  for (int r = 0; r <= len; r++)
    EPD_WriteData(0x00);
}

_attribute_ram_code_ void EPD_LoadImage(unsigned char *image, int size,
                                        uint8_t cmd) {
  int i;
  EPD_WriteCmd(cmd);
  for (i = 0; i < size; i++) {
    EPD_WriteData(image[i]);
  }
  WaitMs(2);
}
