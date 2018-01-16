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

#ifndef _UTILS_COLLECTIONS_H_
#define _UTILS_COLLECTIONS_H_

#include "esp32-utils/utils.h"

#define UTILS_BUFFER_NO_LIMIT       -1

/***********************************************************************************************************
 * Array
 ***********************************************************************************************************/
typedef void(element_free)(void *element);
typedef struct _array *array_t;

array_t array_new(element_free *free_callback);
// Make compatible with free()
void array_free(void *array);
int array_count(array_t array);
int array_push(array_t array, void *element);
void *array_pop(array_t array);
void *array_remove_at(array_t array, int index);
void *array_replace(array_t array, int index, void *element);
void *array_at(array_t array, int index);

/***********************************************************************************************************
 * Map
 ***********************************************************************************************************/
typedef struct _map *map_t;

map_t map_new(element_free *free_callback);
void map_free(void *m);
void *map_value_for_key(map_t map, const char *key);
void *map_set_value_for_key(map_t map, const char *key, void *value);
void *map_remove_key(map_t map, const char *key, void *value);
array_t map_keys(map_t map);
array_t map_values(map_t map);

/***********************************************************************************************************
 * Buffer
 ***********************************************************************************************************/
typedef struct _buffer *buffer_t;

buffer_t buffer_new(int size);
buffer_t buffer_new_from_data(unsigned char *data, int size);
buffer_t buffer_new_from_string(char *string);
buffer_t clone(buffer_t buffer);
// Size of the useful data. Doesn't include a string \0 terminator
int buffer_get_length(buffer_t buffer);
// If data is a string, it is \0 terminated
const unsigned char *buffer_get_data(buffer_t buffer);
// Detach the data buffer form the buffer object. Caller is responsible for freeing
unsigned char *buffer_detach_data(buffer_t buffer);
int buffer_append(buffer_t buffer, const unsigned char *data, int len);
int buffer_append_string(buffer_t buffer, const char *data);
int buffer_append_buffer(buffer_t buffer, const buffer_t data);
int buffer_append_mpi(buffer_t buffer, mbedtls_mpi *data);
int buffer_ensure_available(buffer_t buffer, int len);
int buffer_resize(buffer_t buffer, int new_len);
void buffer_reset(buffer_t buffer);
void buffer_free(void *p);

#endif
#ifdef __cplusplus
}
#endif

