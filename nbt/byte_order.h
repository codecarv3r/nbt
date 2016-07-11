/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  byte_order.h
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

#ifndef byte_order_h
#define byte_order_h

#include <stdio.h>

__BEGIN_DECLS

typedef enum {
	NBT_BIG_ENDIAN,
	NBT_LITTLE_ENDIAN
} nbt_byte_order_t;

extern nbt_byte_order_t nbt_native_byte_order;

void nbt_swap(void* data, size_t length);
int16_t nbt_swap_short(int16_t value);
int32_t nbt_swap_int(int32_t value);
int64_t nbt_swap_long(int64_t value);
float nbt_swap_float(float value);
double nbt_swap_double(double value);

int16_t nbt_reorder_short(int16_t value, nbt_byte_order_t byte_order);
int32_t nbt_reorder_int(int32_t value, nbt_byte_order_t byte_order);
int64_t nbt_reorder_long(int64_t value, nbt_byte_order_t byte_order);
float nbt_reorder_float(float value, nbt_byte_order_t byte_order);
double nbt_reorder_double(double value, nbt_byte_order_t byte_order);

__END_DECLS

#endif /* byte_order_h */
