/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  writing.h
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

#include "internal.h"

#include <string.h>

void _nbt_write_data(nbt_t* tag, nbt_coder_t* coder, nbt_byte_order_t order);
void _nbt_write_payload(nbt_t* tag, nbt_coder_t* coder, nbt_byte_order_t order);

nbt_coder_t* nbt_write_data(nbt_t* tag, nbt_byte_order_t order) {
	nbt_coder_t* coder = nbt_coder_create();
	nbt_coder_initialize_encoder(coder);
	_nbt_write_data(tag, coder, order);
	return coder;
}

void _nbt_write_data(nbt_t* tag, nbt_coder_t* coder, nbt_byte_order_t order) {
	nbt_coder_encode_byte(coder, tag->type);
	nbt_coder_encode_short(coder, strlen(tag->name), order);
	nbt_coder_encode_data(coder, tag->name, strlen(tag->name));
	_nbt_write_payload(tag, coder, order);
}

void _nbt_write_payload(nbt_t* tag, nbt_coder_t* coder, nbt_byte_order_t order) {
	if (!tag) {
		return;
	}
	switch (tag->type) {
		case NBT_END:
			break;
		case NBT_BYTE:
			nbt_coder_encode_byte(coder, tag->payload.tag_byte);
			break;
		case NBT_SHORT:
			nbt_coder_encode_short(coder, tag->payload.tag_short, order);
			break;
		case NBT_INT:
			nbt_coder_encode_int(coder, tag->payload.tag_int, order);
			break;
		case NBT_LONG:
			nbt_coder_encode_long(coder, tag->payload.tag_long, order);
			break;
		case NBT_FLOAT:
			nbt_coder_encode_float(coder, tag->payload.tag_float, order);
			break;
		case NBT_DOUBLE:
			nbt_coder_encode_double(coder, tag->payload.tag_double, order);
			break;
		case NBT_BYTE_ARRAY:
			nbt_coder_encode_int(coder, tag->payload.tag_byte_array.length, order);
			nbt_coder_encode_data(coder, (const char*)tag->payload.tag_byte_array.byte_array, tag->payload.tag_byte_array.length);
			break;
		case NBT_INT_ARRAY:
			nbt_coder_encode_int(coder, tag->payload.tag_int_array.length, order);
			for (int32_t i = 0; i < tag->payload.tag_int_array.length; i++) {
				nbt_coder_encode_int(coder, tag->payload.tag_int_array.int_array[i], order);
			}
			break;
		case NBT_STRING:
			nbt_coder_encode_short(coder, strlen(tag->payload.tag_string), order);
			nbt_coder_encode_data(coder, tag->payload.tag_string, strlen(tag->payload.tag_string));
			break;
		case NBT_LIST: {
			nbt_coder_encode_byte(coder, tag->payload.tag_list.type);
			nbt_coder_encode_int(coder, _nbt_tree_count(tag->payload.tag_list.tree), order);
			nbt_t* next = tag->payload.tag_list.tree;
			do {
				_nbt_write_payload(next, coder, order);
			} while ((next = next->tree_right));
			break;
		}
		case NBT_COMPOUND: {
			nbt_t* next = tag->payload.tag_compound;
			do {
				_nbt_write_data(next, coder, order);
			} while ((next = next->tree_right));
			nbt_coder_encode_byte(coder, 0);
			break;
		}
	}
}
