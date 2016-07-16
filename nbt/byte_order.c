/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  byte_order.c
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

#include "byte_order.h"

#include <string.h>
#include <sys/param.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
nbt_byte_order_t nbt_native_byte_order = NBT_LITTLE_ENDIAN;
#elif __BYTE_ORDER == __BIG_ENDIAN
nbt_byte_order_t nbt_native_byte_order = NBT_BIG_ENDIAN;
#else
#error You seem to be compiling for an unknown byte order.
#endif

void nbt_swap(void* data, size_t length) {
	int8_t* original = (int8_t*)data;
	int8_t temp[length];
	for (size_t i = 0; i < length; i++) {
		temp[i] = original[length - i - 1];
	}
	memcpy(original, temp, length);
}

int16_t nbt_swap_short(int16_t value) {
	return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

int32_t nbt_swap_int(int32_t value) {
	return (((value & 0x00000000FF000000) >> 24) |
			((value & 0x0000000000FF0000) >> 8)  |
			((value & 0x000000000000FF00) << 8)  |
			((value & 0x00000000000000FF) << 24));
}

int64_t nbt_swap_long(int64_t value) {
	return (((value & 0xFF00000000000000) >> 56) |
			((value & 0x00FF000000000000) >> 40) |
			((value & 0x0000FF0000000000) >> 24) |
			((value & 0x000000FF00000000) >> 8)  |
			((value & 0x00000000FF000000) << 8)  |
			((value & 0x0000000000FF0000) << 24) |
			((value & 0x000000000000FF00) << 40) |
			((value & 0x00000000000000FF) << 56));
}

float nbt_swap_float(float value) {
	int32_t temp = nbt_swap_int(*(int32_t*)&value);
	return *(float*)&temp;
}

double nbt_swap_double(double value) {
	int64_t temp = nbt_swap_long(*(int64_t*)&value);
	return *(double*)&temp;
}

int16_t nbt_reorder_short(int16_t value, nbt_byte_order_t byte_order) {
	if (byte_order != nbt_native_byte_order) {
		return nbt_swap_short(value);
	} else {
		return value;
	}
}

int32_t nbt_reorder_int(int32_t value, nbt_byte_order_t byte_order) {
	if (byte_order != nbt_native_byte_order) {
		return nbt_swap_int(value);
	} else {
		return value;
	}
}

int64_t nbt_reorder_long(int64_t value, nbt_byte_order_t byte_order) {
	if (byte_order != nbt_native_byte_order) {
		return nbt_swap_long(value);
	} else {
		return value;
	}
}

float nbt_reorder_float(float value, nbt_byte_order_t byte_order) {
	if (byte_order != nbt_native_byte_order) {
		return nbt_swap_float(value);
	} else {
		return value;
	}
}

double nbt_reorder_double(double value, nbt_byte_order_t byte_order) {
	if (byte_order != nbt_native_byte_order) {
		return nbt_swap_double(value);
	} else {
		return value;
	}
}
