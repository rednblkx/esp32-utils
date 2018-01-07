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

#include "esp32-utils/mpi.h"
#include "errno.h"

mbedtls_mpi *utils_mpi_new(void) {
    mbedtls_mpi *mpi = NULL;
    if (!(mpi = (mbedtls_mpi *)malloc(sizeof(mbedtls_mpi)))) {
        return NULL;
    }
    mbedtls_mpi_init(mpi);
    return mpi;
}

void utils_mpi_free(void *m) {
    mbedtls_mpi *mpi = (mbedtls_mpi *)m;
    if (mpi) {
   	  mbedtls_mpi_free(mpi);
   	  free(mpi);
    }
}

