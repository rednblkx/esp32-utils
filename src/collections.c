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

#include "esp32-utils/collections.h"

/***********************************************************************************************************
 * Array
 ***********************************************************************************************************/
struct _array {
    int count;
    void **elements;
    element_free *free_callback;
};

array_t array_new(element_free *free_callback) {
    array_t array;
    if (!(array = (array_t)malloc(sizeof(struct _array)))) {
        goto cleanup;
    }
    memset(array, 0, sizeof(struct _array));
    array->free_callback = free_callback;
    return array;
cleanup:
    errno = UTILS_ERR_ALLOC_FAILED;
    return NULL;
}

void array_free(void *a) {
    array_t array = (array_t)a;
    if (array) {
        if (array->free_callback) {
            for (int i = 0; i < array->count; i++) {
                array->free_callback(array->elements[i]);
            }
        }
        free(a);
    }
}

int array_count(array_t array) {
    return array->count;
}

int array_push(array_t array, void *element) {
    if (!array->elements) {
        if (!(array->elements = (void **)malloc(sizeof(void *)))) {
            return UTILS_ERR_ALLOC_FAILED;
        }
    }
    else {
        if (!(array->elements = (void **)realloc(array->elements, sizeof(void *) * (array->count + 1)))) {
            return UTILS_ERR_ALLOC_FAILED;
        }
    }
    array->elements[array->count++] = element;
    return UTILS_ERR_OK;
}

void *array_pop(array_t array) {
    return array_remove_at(array, array->count - 1);
}

void *array_remove_at(array_t array, int index) {
    if (index < array->count) {
        void *element = array->elements[index];
        for (int i = index; i < array->count - 1; i++) {
            array->elements[i] = array->elements[i + 1];
        }
        if ((array->elements = (void **)realloc(array->elements, sizeof(void *) * (--array->count)))) {
            return element;
        }
    }
    return NULL;
}

void *array_replace(array_t array, int index, void *element) {
    if (index < array->count) {
        void *old_element = array->elements[index];
        array->elements[index] = element;
        return old_element;
    }
    return NULL;
}

void *array_at(array_t array, int index) {
    if (index < array->count) {
        return array->elements[index];
    }
    return NULL;
}

/***********************************************************************************************************
 * Map
 ***********************************************************************************************************/
struct _map {
    array_t keys;
    array_t values;
};

map_t map_new(element_free *free_callback) {
    map_t map;
    if (!(map = (map_t)malloc(sizeof(struct _map)))) {
        goto cleanup;
    }
    memset(map, 0, sizeof(struct _map));
    if (!(map->keys = array_new(free))) {
        goto cleanup;
    }
    if (!(map->values = array_new(free_callback))) {
        goto cleanup;
    }
    return map;
cleanup:
    map_free(map);
    errno = UTILS_ERR_ALLOC_FAILED;
    return NULL;
}

void map_free(void *m) {
    map_t map = (map_t)m;
    if (map) {
        array_free(map->keys);
        array_free(map->values);
        free(m);
    }
}

array_t map_keys(map_t map) {
    return map->keys;
}

array_t map_values(map_t map) {
    return map->values;
}

void *map_value_for_key(map_t map, const char *key) {
    for (int i = 0; i < array_count(map->keys); i++) {
        if (!strcmp(array_at(map->keys, i), key)) {
            return array_at(map->values, i);
        }
    }
    return NULL;
}

void *map_set_value_for_key(map_t map, const char *key, void *value) {
    for (int i = 0; i < array_count(map->keys); i++) {
        if (!strcmp(array_at(map->keys, i), key)) {
            void *old_value = array_replace(map->values, i, value);
            return old_value;
        }
    }
    array_push(map->keys, (void *)key);
    array_push(map->values, value);
    return value;
}

void *map_remove_key(map_t map, const char *key, void *value) {
    for (int i = 0; i < array_count(map->keys); i++) {
        if (!strcmp(array_at(map->keys, i), key)) {
            array_remove_at(map->keys, i);
            void *old_value = array_remove_at(map->values, i);
            return old_value;
        }
    }
    return NULL;
}

/***********************************************************************************************************
 * Buffer
 ***********************************************************************************************************/
struct _buffer {
    int size;
    int pos;
    unsigned char *data;
};

/***********************************************************************************************************
 * Public interface
 ***********************************************************************************************************/
buffer_t buffer_new(int size) {
    buffer_t buffer;
    if (!(buffer = (buffer_t)malloc(sizeof(struct _buffer)))) {
        return NULL;
    }
    memset(buffer, 0, sizeof(struct _buffer));
    if (!(buffer->data = malloc(size + 1))) {
        goto cleanup;
    }
    memset(buffer->data, 0, size + 1);
    buffer->size = size;
    return buffer;
cleanup:
    buffer_free(buffer);
    return NULL;
}

int buffer_get_length(buffer_t buffer) {
    return buffer->pos;
}

const unsigned char *buffer_get_data(buffer_t buffer) {
    return buffer->data;
}

// If max_size <= 0 the buffer will not resize automatically
// If max_size > current size, the buffer will resize automatically up to max_size.
// Return number of bytes written or UTILS_ERR_OUT_OF_MEMORY if realloc failed
int buffer_append(buffer_t buffer, const unsigned char *data, int len) {
    int ret;
    if (!(ret = buffer_ensure_available(buffer, len))) {
        memcpy(buffer->data + buffer->pos, data, len);
        buffer->pos+= len;
    }
    return ret;
}

int buffer_append_string(buffer_t buffer, const char *data) {
    return buffer_append(buffer, (const unsigned char *)data, strlen(data));
}

int buffer_append_buffer(buffer_t buffer, const buffer_t data) {
    return buffer_append(buffer, buffer_get_data(data), buffer_get_length(data));
}

int buffer_append_mpi(buffer_t buffer, mbedtls_mpi *data) {
    int ret;
    int mpi_size = mbedtls_mpi_size(data);
    if (!(ret = buffer_ensure_available(buffer, mpi_size))) {
        mbedtls_mpi_write_binary(data, buffer->data + buffer->pos, mpi_size);
        buffer->pos+= mpi_size;
    }
    return ret;
}

int buffer_ensure_available(buffer_t buffer, int len) {
    int available = buffer->size - buffer->pos;
    // Buffer was created with no resizing
    if (len > available) {
        int new_size = buffer->pos + len;
        if (buffer_resize(buffer, new_size) != UTILS_ERR_OK) {
            return UTILS_ERR_OUT_OF_MEMORY;
        }
    };
    return UTILS_ERR_OK;
}

int buffer_resize(buffer_t buffer, int new_size) {
    if (new_size < 0) {
        return UTILS_ERR_OUT_OF_MEMORY;        
    }
    if (!(buffer->data = realloc(buffer->data, new_size + 1))) {
        return UTILS_ERR_OUT_OF_MEMORY;
    }
    bool is_shrunk = (new_size < buffer->size);
    int zero_len = is_shrunk ? 1 : new_size - buffer->size + 1;
    int zero_offset = is_shrunk ? new_size : buffer->size;
    memset(buffer->data + zero_offset, 0, zero_len);
    buffer->size = new_size;
    buffer->pos = (buffer->pos > new_size) ? new_size : buffer->pos;
    return UTILS_ERR_OK;
}

void buffer_reset(buffer_t buffer) {
    buffer->pos = 0;
    memset(buffer->data, 0, buffer->size);
}

void buffer_free(void *p) {
    buffer_t buffer = (buffer_t)p;
    if (buffer) {
        free(buffer->data);
        free(p);
    }
}
