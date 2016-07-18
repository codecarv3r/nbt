/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  coder.h
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

#ifndef coder_h
#define coder_h

#include <stdio.h>

#include "byte_order.h"

__BEGIN_DECLS

typedef struct _nbt_coder nbt_coder_t;

nbt_coder_t* nbt_coder_create();
void nbt_coder_destroy(nbt_coder_t* coder);

const char* nbt_coder_reorder_data(nbt_coder_t* coder, size_t* length);

/* File System */
nbt_coder_t* nbt_coder_read_file(const char* path);
void nbt_coder_write_file(nbt_coder_t* coder, const char* path);

/* Choose one or the other */
void nbt_coder_initialize_encoder(nbt_coder_t* coder);
void nbt_coder_initialize_decoder(nbt_coder_t* coder, const char* data, size_t length);
void nbt_coder_force_encoder(nbt_coder_t* coder);
void nbt_coder_force_decoder(nbt_coder_t* coder);

/* Encoder */
void nbt_coder_encode_byte(nbt_coder_t* coder, int8_t item);
void nbt_coder_encode_short(nbt_coder_t* coder, int16_t item, nbt_byte_order_t order);
void nbt_coder_encode_int(nbt_coder_t* coder, int32_t item, nbt_byte_order_t order);
void nbt_coder_encode_long(nbt_coder_t* coder, int64_t item, nbt_byte_order_t order);
void nbt_coder_encode_float(nbt_coder_t* coder, float item, nbt_byte_order_t order);
void nbt_coder_encode_double(nbt_coder_t* coder, double item, nbt_byte_order_t order);
void nbt_coder_encode_data(nbt_coder_t* coder, const char* data, size_t length);

/* Decoder */
int8_t nbt_coder_decode_byte(nbt_coder_t* coder);
int16_t nbt_coder_decode_short(nbt_coder_t* coder, nbt_byte_order_t order);
int32_t nbt_coder_decode_int(nbt_coder_t* coder, nbt_byte_order_t order);
int64_t nbt_coder_decode_long(nbt_coder_t* coder, nbt_byte_order_t order);
float nbt_coder_decode_float(nbt_coder_t* coder, nbt_byte_order_t order);
double nbt_coder_decode_double(nbt_coder_t* coder, nbt_byte_order_t order);
void nbt_coder_decode_data(nbt_coder_t* coder, char* buffer, size_t length);

/* Compression */
typedef enum {
	NBT_COMPRESSION_GZIP,	/* gzip header -- compress like a level.dat */
	NBT_COMPRESSION_INFLATE	/* zlib header -- compress like a chunk */
} nbt_compression_strategy_t;
nbt_coder_t* nbt_coder_compress(nbt_coder_t* coder, nbt_compression_strategy_t compression_strategy);
nbt_coder_t* nbt_coder_decompress(nbt_coder_t* coder);

__END_DECLS

#endif /* coder_h */
