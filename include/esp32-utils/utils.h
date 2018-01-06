/*
 * A collection of utilities for ESP32
 *
 * Copyright (c) 2018 Emmanuel Merali
 * https://github.com/ifullgaz/esp32-utils
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ESP32_UTILS_H_
#define _ESP32_UTILS_H_

#define UTILS_ERR_OK                        0
#define UTILS_ERR_ALLOC_FAILED              -0x1004
#define UTILS_ERR_OUT_OF_MEMORY             UTILS_ERR_ALLOC_FAILED

#include "esp32-utils/dump.h"
#include "esp32-utils/collections.h"

#endif
#ifdef __cplusplus
}
#endif

