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
#include "epd_bw_213_ice.h"
#include "drivers.h"
#include "stack/ble/ble.h"

// SSD1675 mixed with SSD1680 EPD Controller

#define BW_213_ice_Len 50
uint8_t LUT_BW_213_ice_part[] = {

    0x40,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    BW_213_ice_Len, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

_attribute_ram_code_ uint8_t EPD_BW_213_ice_detect(void) {
  // SW Reset
  EPD_WriteCmd(0x12);
  WaitMs(10);

  EPD_WriteCmd(0x2F);
  if (EPD_SPI_read() != 0x01)
    return 0;
  return 1;
}

_attribute_ram_code_ uint8_t EPD_BW_213_ice_read_temp(void) {
  uint8_t epd_temperature = 0;

  // SW Reset
  EPD_WriteCmd(0x12);

  EPD_CheckStatus_inverted(100);

  // Set Analog Block control
  EPD_WriteCmd(0x74);
  EPD_WriteData(0x54);
  // Set Digital Block control
  EPD_WriteCmd(0x7E);
  EPD_WriteData(0x3B);

  // ACVCOM Setting
  EPD_WriteCmd(0x2B);
  EPD_WriteData(0x04);
  EPD_WriteData(0x63);

  // Booster soft start
  EPD_WriteCmd(0x0C);
  EPD_WriteData(0x8B);
  EPD_WriteData(0x9C);
  EPD_WriteData(0x96);
  EPD_WriteData(0x0F);

  // Driver output control
  EPD_WriteCmd(0x01);
  EPD_WriteData(0x28);
  EPD_WriteData(0x01);
  EPD_WriteData(0x01);

  // Data entry mode setting
  EPD_WriteCmd(0x11);
  EPD_WriteData(0x01);

  // Temperature sensor control
  EPD_WriteCmd(0x18);
  EPD_WriteData(0x80);

  // Set RAM X- Address Start/End
  EPD_WriteCmd(0x44);
  EPD_WriteData(0x00);
  // EPD_WriteData(0x0C);
  EPD_WriteData(0x0f); // modif auchan

  // Set RAM Y- Address Start/End
  EPD_WriteCmd(0x45);
  EPD_WriteData(0x28);
  EPD_WriteData(0x01);
  // EPD_WriteData(0x54);
  EPD_WriteData(0x2f); // modif auchan
  EPD_WriteData(0x00);

  // Border waveform control
  EPD_WriteCmd(0x3C);
  EPD_WriteData(0x01);

  // Display update control
  EPD_WriteCmd(0x22);
  EPD_WriteData(0xA1);

  // Master Activation
  EPD_WriteCmd(0x20);

  EPD_CheckStatus_inverted(100);

  // Temperature sensor read from register
  EPD_WriteCmd(0x1B);
  epd_temperature = EPD_SPI_read();
  EPD_SPI_read();

  WaitMs(5);

  // Display update control
  EPD_WriteCmd(0x22);
  EPD_WriteData(0xB1);

  // Master Activation
  EPD_WriteCmd(0x20);

  EPD_CheckStatus_inverted(100);

  // Display update control
  EPD_WriteCmd(0x21);
  EPD_WriteData(0x03);

  // deep sleep
  EPD_WriteCmd(0x10);
  EPD_WriteData(0x01);

  return epd_temperature;
}

_attribute_ram_code_ uint8_t EPD_BW_213_ice_Display(unsigned char *image,
                                                    int size,
                                                    uint8_t full_or_partial) {
  uint8_t epd_temperature = 0;

  // SW Reset
  EPD_WriteCmd(0x12);

  EPD_CheckStatus_inverted(100);

  // Set Analog Block control
  EPD_WriteCmd(0x74);
  EPD_WriteData(0x54);
  // Set Digital Block control
  EPD_WriteCmd(0x7E);
  EPD_WriteData(0x3B);

  // ACVCOM Setting
  EPD_WriteCmd(0x2B);
  EPD_WriteData(0x04);
  EPD_WriteData(0x63);

  // Booster soft start
  EPD_WriteCmd(0x0C);
  EPD_WriteData(0x8B);
  EPD_WriteData(0x9C);
  EPD_WriteData(0x96);
  EPD_WriteData(0x0F);

  // Driver output control
  EPD_WriteCmd(0x01);
  EPD_WriteData(0x28);
  EPD_WriteData(0x01);
  EPD_WriteData(0x01);

  // Data entry mode setting
  EPD_WriteCmd(0x11);
  EPD_WriteData(0x01);

  // Temperature sensor control
  EPD_WriteCmd(0x18);
  EPD_WriteData(0x80);

  // Set RAM X- Address Start/End
  EPD_WriteCmd(0x44);
  EPD_WriteData(0x00);
  // EPD_WriteData(0x0C);
  EPD_WriteData(0x0f); // modif auchan

  // Set RAM Y- Address Start/End
  EPD_WriteCmd(0x45);
  EPD_WriteData(0x28);
  EPD_WriteData(0x01);
  // EPD_WriteData(0x54);
  EPD_WriteData(0x2e); // modif auchan
  EPD_WriteData(0x00);

  // Border waveform control
  EPD_WriteCmd(0x3C);
  EPD_WriteData(0x01);

  // Display update control
  EPD_WriteCmd(0x22);
  EPD_WriteData(0xA1);

  // Master Activation
  EPD_WriteCmd(0x20);

  EPD_CheckStatus_inverted(100);

  // Temperature sensor read from register
  EPD_WriteCmd(0x1B);
  epd_temperature = EPD_SPI_read();
  EPD_SPI_read();

  WaitMs(5);

  // Display update control
  EPD_WriteCmd(0x22);
  EPD_WriteData(0xB1);

  // Master Activation
  EPD_WriteCmd(0x20);

  EPD_CheckStatus_inverted(100);

  // Display update control
  EPD_WriteCmd(0x21);
  EPD_WriteData(0x03);

  // Set RAM X address
  EPD_WriteCmd(0x4E);
  EPD_WriteData(0x00);

  // Set RAM Y address
  EPD_WriteCmd(0x4F);
  EPD_WriteData(0x28);
  EPD_WriteData(0x01);

  EPD_LoadImage(image, size, 0x24);

  // Display update control
  EPD_WriteCmd(0x22);
  EPD_WriteData(0x40);

  int i;
  if (!full_or_partial) {
    EPD_WriteCmd(0x32);
    for (i = 0; i < sizeof(LUT_BW_213_ice_part); i++) {
      EPD_WriteData(LUT_BW_213_ice_part[i]);
    }
  }

  // Display update control
  EPD_WriteCmd(0x22);
  EPD_WriteData(0xC7);

  // Master Activation
  EPD_WriteCmd(0x20);

  return epd_temperature;
}

_attribute_ram_code_ void EPD_BW_213_ice_set_sleep(void) {
  // deep sleep
  EPD_WriteCmd(0x10);
  EPD_WriteData(0x01);
}
