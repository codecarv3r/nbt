/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  nbt.h
 *  This file is part of nbt.
 *
 *  Created by Silas Schwarz on 7/10/16.
 *  Copyright (c) 2016 ZCodeMT LLC.
 *
 *  nbt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nbt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with nbt.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef nbt_h
#define nbt_h

#include <stdio.h>
#include <stdbool.h>

#include "byte_order.h"
#include "coder.h"

__BEGIN_DECLS

typedef struct _nbt nbt_t;

typedef enum {
	NBT_SUCCESS			= 0,
	NBT_ERROR_UNKNOWN	= 1,
	NBT_ERROR_MEMORY	= 2,
	NBT_ERROR_IO		= 3,
	NBT_ERROR_ZLIB		= 4
} nbt_status_t;

typedef enum {
	NBT_END			= 0,
	NBT_BYTE		= 1,
	NBT_SHORT		= 2,
	NBT_INT			= 3,
	NBT_LONG		= 4,
	NBT_FLOAT		= 5,
	NBT_DOUBLE		= 6,
	NBT_BYTE_ARRAY	= 7,
	NBT_STRING		= 8,
	NBT_LIST		= 9,
	NBT_COMPOUND	= 10,
	NBT_INT_ARRAY	= 11
} nbt_type_t;

/* Create a node */
nbt_t* nbt_create();

/* Create simple nodes */
nbt_t* nbt_create_byte(const char* name, int8_t payload);
nbt_t* nbt_create_short(const char* name, int16_t payload);
nbt_t* nbt_create_int(const char* name, int32_t payload);
nbt_t* nbt_create_long(const char* name, int64_t payload);
nbt_t* nbt_create_float(const char* name, float payload);
nbt_t* nbt_create_double(const char* name, double payload);

nbt_t* nbt_create_string(const char* name, const char* payload);

/* Create array types */
nbt_t* nbt_create_byte_array(const char* name, const int8_t* bytes, int32_t length);
nbt_t* nbt_create_int_array(const char* name, const int32_t* ints, int32_t length);

/* Create list node. */
nbt_t* nbt_create_list(const char* name, nbt_type_t type);

/* Create compound node */
nbt_t* nbt_create_compound(const char* name);

/* Don't leak memory, mkay? */
void nbt_release(nbt_t* tag);

/* Parsing */
nbt_t* nbt_parse_data(const char* bytes, size_t length, nbt_byte_order_t order, bool compressed, nbt_status_t* errorp);
nbt_t* nbt_parse_coder(nbt_coder_t* coder, nbt_byte_order_t order, bool compressed, nbt_status_t* errorp);

/* Writing */
nbt_coder_t* nbt_write_data(nbt_t* tag, nbt_byte_order_t order);

/* Get the value of simple types */
int8_t nbt_byte(nbt_t* tag);
int16_t nbt_short(nbt_t* tag);
int32_t nbt_int(nbt_t* tag);
int64_t nbt_long(nbt_t* tag);
float nbt_float(nbt_t* tag);
double nbt_double(nbt_t* tag);

const int8_t* nbt_byte_array(nbt_t* tag);
const int32_t* nbt_int_array(nbt_t* tag);

const char* nbt_string(nbt_t* tag);

/* Working with lists */
int32_t nbt_list_count(nbt_t* list);
nbt_t* nbt_list_index(nbt_t* list, int32_t index);
void nbt_list_add(nbt_t* list, nbt_t* item);
void nbt_list_remove(nbt_t* list, int32_t index);

/* Working with compounds */
nbt_t* nbt_compound_name(nbt_t* compound, const char* name);
void nbt_compound_set(nbt_t* compound, nbt_t* item);
void nbt_compound_remove(nbt_t* compound, const char* name);

/* Printing */
typedef enum {
	NBT_STYLE_ORIGINAL,
	NBT_STYLE_PIPE,
	NBT_STYLE_COLOR
} nbt_print_style_t;

char* nbt_print(nbt_t* tag, nbt_print_style_t style);

__END_DECLS

#endif /* nbt_h */
