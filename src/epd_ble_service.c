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

#include "epd_ble_service.h"

#include <stdint.h>
#include "tl_common.h"
#include "stack/ble/ble.h"

#include "epd.h"
#include "ble.h"

extern uint8_t *epd_temp;

#define ASSERT_MIN_LEN(val, min_len)                                           \
  if (val < min_len) {                                                         \
    return 0;                                                                  \
  }

extern unsigned char epd_buffer[epd_buffer_size];
unsigned int byte_pos = 0;

int epd_ble_handle_write(void *p) {
  rf_packet_att_write_t *req = (rf_packet_att_write_t *)p;
  uint8_t *payload = &req->value;
  unsigned int payload_len = req->l2capLen - 3;

  ASSERT_MIN_LEN(payload_len, 1);

  switch (payload[0]) {
  // Clear EPD display.
  case 0x00:
    ASSERT_MIN_LEN(payload_len, 2);
    memset(epd_buffer, payload[1], sizeof(epd_buffer));
    ble_set_connection_speed(40);
    return 0;
  // Push buffer to display.
  case 0x01:
    ble_set_connection_speed(200);
    EPD_Display(epd_buffer, epd_buffer_size, 1);
    return 0;
  // Set byte_pos.
  case 0x02:
    ASSERT_MIN_LEN(payload_len, 3);
    byte_pos = payload[1] << 8 | payload[2];
    return 0;
  // Write data to image buffer.
  case 0x03:
    if (byte_pos + payload_len - 1 >= sizeof(epd_buffer) + 1) {
      return 0;
    }
    memcpy(epd_buffer + byte_pos, payload + 1, payload_len - 1);
    byte_pos += payload_len - 1;
    return 0;
  case 0x04: // decode & display a TIFF image
    epd_display_tiff(epd_buffer, byte_pos);
    return 0;
  default:
    return 0;
  }

  return 0;
}
