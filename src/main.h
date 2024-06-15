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
#include <stdint.h>
#include "app_config.h"
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

#define LED_BLUE GPIO_PA7
#define LED_RED GPIO_PD2
#define LED_GREEN GPIO_PD3

#define RXD GPIO_PA0
#define TXD GPIO_PB1

#define EPD_RESET GPIO_PD4
#define EPD_DC GPIO_PD7
#define EPD_BUSY GPIO_PA1
#define EPD_CS GPIO_PB4
#define EPD_CLK GPIO_PB5
#define EPD_MOSI GPIO_PB6
#define EPD_ENABLE GPIO_PC5

#define NFC_SDA GPIO_PC0
#define NFC_SCL GPIO_PC1
#define NFC_CS GPIO_PC6
#define NFC_IRQ GPIO_PC4
