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
	char* data;
	size_t size;
	size_t cursor;
	size_t reserved;
};

void _nbt_coder_reserve(nbt_coder_t* coder, size_t reserved);

nbt_coder_t* nbt_coder_create() {
	nbt_coder_t* coder = malloc(sizeof(*coder));
	coder->data = malloc(NBT_CODER_DEFAULT_CHUNK);
	coder->size = 0;
	coder->cursor = 0;
	coder->reserved = NBT_CODER_DEFAULT_CHUNK;
	return coder;
}

nbt_coder_t* nbt_coder_create_file(const char* path) {
	nbt_coder_t* coder = nbt_coder_create();
	FILE* fp = fopen(path, "r");
	assert(fp);
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	_nbt_coder_reserve(coder, size);
	coder->size = size;
	fseek(fp, 0, SEEK_SET);
	fread(coder->data, coder->size, 1, fp);
	fclose(fp);
	coder->cursor = 0;
	return coder;
}

nbt_coder_t* nbt_coder_create_data(const char* data, size_t size) {
	nbt_coder_t* coder = nbt_coder_create();
	nbt_coder_encode_data(coder, data, size);
	coder->cursor = 0;
	return coder;
}

void nbt_coder_release(nbt_coder_t* coder) {
	if (coder) {
		free(coder->data);
		free(coder);
	}
}

void nbt_coder_write_file(nbt_coder_t* coder, const char* path) {
	FILE* fp = fopen(path, "w");
	assert(fp);
	fwrite(coder->data, coder->size, 1, fp);
	fclose(fp);
}

void nbt_coder_encode_byte(nbt_coder_t* coder, int8_t item) {
	_nbt_coder_reserve(coder, coder->size + sizeof(item));
	if (coder->cursor != coder->size) {
		for (size_t i = 0; i < sizeof(item); i++) {
			((int8_t*)((uintptr_t)coder->data + coder->cursor + sizeof(item)))[i] = ((int8_t*)((uintptr_t)coder->data + coder->cursor))[i];
		}
	}
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor) = item;
	coder->size += sizeof(item);
	coder->cursor += sizeof(item);
}

void nbt_coder_encode_short(nbt_coder_t* coder, int16_t item, nbt_byte_order_t order) {
	_nbt_coder_reserve(coder, coder->size + sizeof(item));
	if (coder->cursor != coder->size) {
		for (size_t i = 0; i < sizeof(item); i++) {
			((int8_t*)((uintptr_t)coder->data + coder->cursor + sizeof(item)))[i] = ((int8_t*)((uintptr_t)coder->data + coder->cursor))[i];
		}
	}
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor) = nbt_reorder_short(item, order);
	coder->size += sizeof(item);
	coder->cursor += sizeof(item);
}

void nbt_coder_encode_int(nbt_coder_t* coder, int32_t item, nbt_byte_order_t order) {
	_nbt_coder_reserve(coder, coder->size + sizeof(item));
	if (coder->cursor != coder->size) {
		for (size_t i = 0; i < sizeof(item); i++) {
			((int8_t*)((uintptr_t)coder->data + coder->cursor + sizeof(item)))[i] = ((int8_t*)((uintptr_t)coder->data + coder->cursor))[i];
		}
	}
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor) = nbt_reorder_int(item, order);
	coder->size += sizeof(item);
	coder->cursor += sizeof(item);
}

void nbt_coder_encode_long(nbt_coder_t* coder, int64_t item, nbt_byte_order_t order) {
	_nbt_coder_reserve(coder, coder->size + sizeof(item));
	if (coder->cursor != coder->size) {
		for (size_t i = 0; i < sizeof(item); i++) {
			((int8_t*)((uintptr_t)coder->data + coder->cursor + sizeof(item)))[i] = ((int8_t*)((uintptr_t)coder->data + coder->cursor))[i];
		}
	}
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor) = nbt_reorder_long(item, order);
	coder->size += sizeof(item);
	coder->cursor += sizeof(item);
}

void nbt_coder_encode_float(nbt_coder_t* coder, float item, nbt_byte_order_t order) {
	_nbt_coder_reserve(coder, coder->size + sizeof(item));
	if (coder->cursor != coder->size) {
		for (size_t i = 0; i < sizeof(item); i++) {
			((int8_t*)((uintptr_t)coder->data + coder->cursor + sizeof(item)))[i] = ((int8_t*)((uintptr_t)coder->data + coder->cursor))[i];
		}
	}
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor) = nbt_reorder_float(item, order);
	coder->size += sizeof(item);
	coder->cursor += sizeof(item);
}

void nbt_coder_encode_double(nbt_coder_t* coder, double item, nbt_byte_order_t order) {
	_nbt_coder_reserve(coder, coder->size + sizeof(item));
	if (coder->cursor != coder->size) {
		for (size_t i = 0; i < sizeof(item); i++) {
			((int8_t*)((uintptr_t)coder->data + coder->cursor + sizeof(item)))[i] = ((int8_t*)((uintptr_t)coder->data + coder->cursor))[i];
		}
	}
	*(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor) = nbt_reorder_double(item, order);
	coder->size += sizeof(item);
	coder->cursor += sizeof(item);
}

void nbt_coder_encode_data(nbt_coder_t* coder, const char* data, size_t length) {
	_nbt_coder_reserve(coder, coder->size + length);
	if (coder->cursor != coder->size) {
		for (size_t i = 0; i < length; i++) {
			((int8_t*)((uintptr_t)coder->data + coder->cursor + length))[i] = ((int8_t*)((uintptr_t)coder->data + coder->cursor))[i];
		}
	}
	memcpy((void*)((uintptr_t)coder->data + coder->cursor), data, length);
	coder->size += length;
	coder->cursor += length;
}

int8_t nbt_coder_decode_byte(nbt_coder_t* coder) {
	int8_t item;
	assert(coder->cursor + sizeof(__typeof__(item)) <= coder->size);
	item = *(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(item);
	return item;
}

int16_t nbt_coder_decode_short(nbt_coder_t* coder, nbt_byte_order_t order) {
	int16_t item;
	assert(coder->cursor + sizeof(__typeof__(item)) <= coder->size);
	item = *(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(item);
	return nbt_reorder_short(item, order);
}

int32_t nbt_coder_decode_int(nbt_coder_t* coder, nbt_byte_order_t order) {
	int32_t item;
	assert(coder->cursor + sizeof(__typeof__(item)) <= coder->size);
	item = *(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(item);
	return nbt_reorder_int(item, order);
}

int64_t nbt_coder_decode_long(nbt_coder_t* coder, nbt_byte_order_t order) {
	int64_t item;
	assert(coder->cursor + sizeof(__typeof__(item)) <= coder->size);
	item = *(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(item);
	return nbt_reorder_long(item, order);
}

float nbt_coder_decode_float(nbt_coder_t* coder, nbt_byte_order_t order) {
	float item;
	assert(coder->cursor + sizeof(__typeof__(item)) <= coder->size);
	item = *(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(item);
	return nbt_reorder_float(item, order);
}

double nbt_coder_decode_double(nbt_coder_t* coder, nbt_byte_order_t order) {
	double item;
	assert(coder->cursor + sizeof(__typeof__(item)) <= coder->size);
	item = *(__typeof__(item)*)((uintptr_t)coder->data + coder->cursor);
	coder->cursor += sizeof(item);
	return nbt_reorder_double(item, order);
}

void nbt_coder_decode_data(nbt_coder_t* coder, char* buffer, size_t length) {
	assert(coder->cursor + length <= coder->size);
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
	
	z_stream stream = {
		.zalloc		= Z_NULL,
		.zfree		= Z_NULL,
		.opaque		= Z_NULL,
		.next_in	= (void*)coder->data,
		.avail_in	= (uInt)coder->size
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
						 window_bits,
						 8,
						 Z_DEFAULT_STRATEGY));
	
	do {
		_nbt_coder_reserve(ret_coder, ret_coder->size + NBT_CODER_DEFAULT_CHUNK);
		
		stream.next_out = (Bytef*)ret_coder->data + ret_coder->size;
		stream.avail_out = NBT_CODER_DEFAULT_CHUNK;
		
		assert(deflate(&stream, Z_FINISH) != Z_STREAM_ERROR);
		
		ret_coder->size += NBT_CODER_DEFAULT_CHUNK - stream.avail_out;
	} while (stream.avail_out == 0);
	
	deflateEnd(&stream);
	return ret_coder;
}

nbt_coder_t* nbt_coder_decompress(nbt_coder_t* coder) {
	nbt_coder_t* ret_coder = nbt_coder_create();
	
	z_stream stream = {
		.zalloc		= Z_NULL,
		.zfree		= Z_NULL,
		.opaque		= Z_NULL,
		.next_in	= (void*)coder->data,
		.avail_in	= (uInt)coder->size
	};
	
	/* automatic header detection */
	assert(inflateInit2(&stream, 15 + 32) == Z_OK);
	
	int zlib_ret;
	do {
		_nbt_coder_reserve(ret_coder, ret_coder->size + NBT_CODER_DEFAULT_CHUNK);
		
		stream.avail_out = NBT_CODER_DEFAULT_CHUNK;
		stream.next_out = (Bytef*)ret_coder->data + ret_coder->size;
		switch ((zlib_ret = inflate(&stream, Z_NO_FLUSH))) {
			case Z_MEM_ERROR:
			case Z_DATA_ERROR:
			case Z_NEED_DICT:
				assert(0);
			default:
				ret_coder->size += NBT_CODER_DEFAULT_CHUNK - stream.avail_out;
		}
	} while (stream.avail_out == 0);
	
	assert(zlib_ret == Z_STREAM_END);
	inflateEnd(&stream);
	return ret_coder;
}
