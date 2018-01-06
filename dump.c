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

#include "freertos/FreeRTOS.h"
#include <string.h>
#include "mbedtls/bignum.h"
#include "esp32-utils/utils.h"

void dump_data(const void* data, size_t size, const char *description) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	if (description) {
		printf("%s\n", description);
	}
	if (!data || !size) {
		printf("NULL\n");
		return;
	}
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
	if (description) {
		printf("End\n");
	}
}

void dump_string(const char *string, const char *description) {
	int size = 0;
	if (string) {
		size = strlen(string);
	}
	dump_data(string, size, description);
}

void dump_big_number(const void *big_number, const char *description) {
	if (!big_number) {
		dump_data(NULL, 0, description);
		return;
	}
	unsigned char *bytes_bn; int len_bn;
	len_bn   = mbedtls_mpi_size((mbedtls_mpi *)big_number);
	bytes_bn = malloc(len_bn);
	mbedtls_mpi_write_binary(big_number, (unsigned char *)bytes_bn, len_bn);
	dump_data(bytes_bn, len_bn, description);
	free(bytes_bn);
}

