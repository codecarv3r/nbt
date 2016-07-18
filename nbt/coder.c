/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  coder.c
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

#include "coder.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define NBT_CODER_DEFAULT_CHUNK 128

struct _nbt_coder {
	enum {
		ENCODER,
		DECODER
	} type;
	char* data;
	size_t length;
	size_t cursor;
	size_t reserved;
};

void _nbt_coder_reserve(nbt_coder_t* coder, size_t reserved);

nbt_coder_t* nbt_coder_create() {
	nbt_coder_t* coder = malloc(sizeof(*coder));
	memset(coder, 0, sizeof(*coder));
	return coder;
}

void nbt_coder_destroy(nbt_coder_t* coder) {
	if (coder) {
		free(coder->data);
		free(coder);
	}
}

nbt_coder_t* nbt_coder_read_file(const char* path) {
	FILE* fp = fopen(path, "r");
	assert(fp);
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	nbt_coder_t* coder = nbt_coder_create();
	_nbt_coder_reserve(coder, size);
	coder->length = size;
	fseek(fp, 0, SEEK_SET);
	fread(coder->data, size, 1, fp);
	fclose(fp);
	coder->type = DECODER;
	return coder;
}

void nbt_coder_write_file(nbt_coder_t* coder, const char* path) {
	FILE* fp = fopen(path, "w");
	assert(fp);
	fwrite(coder->data, coder->length, 1, fp);
	fclose(fp);
}

void nbt_coder_initialize_encoder(nbt_coder_t* coder) {
	coder->type = ENCODER;
	_nbt_coder_reserve(coder, 0);
	coder->length = 0;
	coder->cursor = 0;
}

void nbt_coder_initialize_decoder(nbt_coder_t* coder, const char* data, size_t length) {
	coder->type = DECODER;
	_nbt_coder_reserve(coder, length);
	coder->length = length;
	coder->cursor = 0;
	memcpy(coder->data, data, length);
}

void nbt_coder_force_encoder(nbt_coder_t* coder) {
	free(coder->data);
	memset(coder, 0, sizeof(*coder));
	nbt_coder_initialize_encoder(coder);
}

void nbt_coder_force_decoder(nbt_coder_t* coder) {
	coder->type = DECODER;
	coder->cursor = 0;
}

void nbt_coder_encode_byte(nbt_coder_t* coder, int8_t item) {
	assert(coder->type == ENCODER);
	_nbt_coder_reserve(coder, coder->length + sizeof(item));
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->length) = item;
	coder->length += sizeof(item);
}

void nbt_coder_encode_short(nbt_coder_t* coder, int16_t item, nbt_byte_order_t order) {
	assert(coder->type == ENCODER);
	_nbt_coder_reserve(coder, coder->length + sizeof(item));
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->length) = nbt_reorder_short(item, order);
	coder->length += sizeof(item);
}

void nbt_coder_encode_int(nbt_coder_t* coder, int32_t item, nbt_byte_order_t order) {
	assert(coder->type == ENCODER);
	_nbt_coder_reserve(coder, coder->length + sizeof(item));
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->length) = nbt_reorder_int(item, order);
	coder->length += sizeof(item);
}

void nbt_coder_encode_long(nbt_coder_t* coder, int64_t item, nbt_byte_order_t order) {
	assert(coder->type == ENCODER);
	_nbt_coder_reserve(coder, coder->length + sizeof(item));
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->length) = nbt_reorder_long(item, order);
	coder->length += sizeof(item);
}

void nbt_coder_encode_float(nbt_coder_t* coder, float item, nbt_byte_order_t order) {
	assert(coder->type == ENCODER);
	_nbt_coder_reserve(coder, coder->length + sizeof(item));
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->length) = nbt_reorder_float(item, order);
	coder->length += sizeof(item);
}

void nbt_coder_encode_double(nbt_coder_t* coder, double item, nbt_byte_order_t order) {
	assert(coder->type == ENCODER);
	_nbt_coder_reserve(coder, coder->length + sizeof(item));
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->length) = nbt_reorder_double(item, order);
	coder->length += sizeof(item);
}

void nbt_coder_encode_data(nbt_coder_t* coder, const char* data, size_t length) {
	assert(coder->type == ENCODER);
	_nbt_coder_reserve(coder, coder->length + length);
	memcpy((void*)((uintptr_t)coder->data + coder->length), data, length);
	coder->length += length;
}

int8_t nbt_coder_decode_byte(nbt_coder_t* coder) {
	assert(coder->type == DECODER);
	assert(coder->cursor + sizeof(int8_t) <= coder->length);
	int8_t item = *(int8_t*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(int8_t);
	return item;
}

int16_t nbt_coder_decode_short(nbt_coder_t* coder, nbt_byte_order_t order) {
	assert(coder->type == DECODER);
	assert(coder->cursor + sizeof(int16_t) <= coder->length);
	int16_t item = *(int16_t*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(int16_t);
	return nbt_reorder_short(item, order);
}

int32_t nbt_coder_decode_int(nbt_coder_t* coder, nbt_byte_order_t order) {
	assert(coder->type == DECODER);
	assert(coder->cursor + sizeof(int32_t) <= coder->length);
	int32_t item = *(int32_t*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(int32_t);
	return nbt_reorder_int(item, order);
}

int64_t nbt_coder_decode_long(nbt_coder_t* coder, nbt_byte_order_t order) {
	assert(coder->type == DECODER);
	assert(coder->cursor + sizeof(int64_t) <= coder->length);
	int64_t item = *(int64_t*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(int64_t);
	return nbt_reorder_long(item, order);
}

float nbt_coder_decode_float(nbt_coder_t* coder, nbt_byte_order_t order) {
	assert(coder->type == DECODER);
	assert(coder->cursor + sizeof(float) <= coder->length);
	float item = *(float*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(float);
	return nbt_reorder_float(item, order);
}

double nbt_coder_decode_double(nbt_coder_t* coder, nbt_byte_order_t order) {
	assert(coder->type == DECODER);
	assert(coder->cursor + sizeof(double) <= coder->length);
	double item = *(double*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(double);
	return nbt_reorder_double(item, order);
}

void nbt_coder_decode_data(nbt_coder_t* coder, char* buffer, size_t length) {
	assert(coder->type == DECODER);
	assert(coder->cursor + length <= coder->length);
	memcpy(buffer, (void*)((uintptr_t)coder->data + coder->cursor), length);
	coder->cursor += length;
}

void _nbt_coder_reserve(nbt_coder_t* coder, size_t reserved) {
	if (!coder->data) {
		coder->data = malloc(NBT_CODER_DEFAULT_CHUNK);
		coder->reserved = NBT_CODER_DEFAULT_CHUNK;
	}
	if (coder->reserved < reserved) {
		do {
			coder->reserved <<= 1;
		} while (coder->reserved < reserved);
		coder->data = realloc(coder->data, coder->reserved);
	}
}

nbt_coder_t* nbt_coder_compress(nbt_coder_t* coder, nbt_compression_strategy_t compression_strategy) {
	nbt_coder_t* ret_coder = nbt_coder_create();
	nbt_coder_initialize_encoder(ret_coder);
	
	z_stream stream = {
		.zalloc		= Z_NULL,
		.zfree		= Z_NULL,
		.opaque		= Z_NULL,
		.next_in	= (void*)coder->data,
		.avail_in	= (uInt)coder->length
	};
	
	/* Should be from 8..15 */
	int window_bits = 15;
	
	/* Or add 16 if we are using a gzip header */
	if (compression_strategy == NBT_COMPRESSION_GZIP) {
		window_bits += 16;
	}
	
	/* Start the compression */
	assert(!deflateInit2(&stream,
						 Z_DEFAULT_COMPRESSION,
						 Z_DEFLATED,
						 15,
						 8,
						 Z_DEFAULT_STRATEGY));
	
	do {
		_nbt_coder_reserve(ret_coder, ret_coder->length + NBT_CODER_DEFAULT_CHUNK);
		
		stream.next_out = (Bytef*)ret_coder->data + ret_coder->length;
		stream.avail_out = NBT_CODER_DEFAULT_CHUNK;
		
		assert(deflate(&stream, Z_FINISH) != Z_STREAM_ERROR);
		
		ret_coder->length += NBT_CODER_DEFAULT_CHUNK - stream.avail_out;
	} while (stream.avail_out == 0);
	
	deflateEnd(&stream);
	return ret_coder;
}

nbt_coder_t* nbt_coder_decompress(nbt_coder_t* coder) {
	nbt_coder_t* ret_coder = nbt_coder_create();
	nbt_coder_initialize_encoder(ret_coder);
	
	z_stream stream = {
		.zalloc		= Z_NULL,
		.zfree		= Z_NULL,
		.opaque		= Z_NULL,
		.next_in	= (void*)coder->data,
		.avail_in	= (uInt)coder->length
	};
	
	/* automatic header detection */
	assert(inflateInit2(&stream, 15 + 32) == Z_OK);
	
	int zlib_ret;
	do {
		_nbt_coder_reserve(ret_coder, ret_coder->length + NBT_CODER_DEFAULT_CHUNK);
		
		stream.avail_out = NBT_CODER_DEFAULT_CHUNK;
		stream.next_out = (Bytef*)ret_coder->data + ret_coder->length;
		switch ((zlib_ret = inflate(&stream, Z_NO_FLUSH))) {
			case Z_MEM_ERROR:
			case Z_DATA_ERROR:
			case Z_NEED_DICT:
				assert(0);
			default:
				ret_coder->length += NBT_CODER_DEFAULT_CHUNK - stream.avail_out;
		}
	} while (stream.avail_out == 0);
	
	assert(zlib_ret == Z_STREAM_END);
	inflateEnd(&stream);
	return ret_coder;
}
