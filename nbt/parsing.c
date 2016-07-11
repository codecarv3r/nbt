/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  parsing.c
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
#include "coder.h"

nbt_t* _nbt_parse_coder(nbt_coder_t* coder, nbt_byte_order_t order, nbt_status_t* errorp);
nbt_t* _nbt_parse_payload(nbt_type_t type, const char* name, nbt_coder_t* coder, nbt_byte_order_t order, nbt_status_t* errorp);

nbt_t* nbt_parse_data(const char* bytes, size_t length, nbt_byte_order_t order, nbt_status_t* errorp) {
	nbt_coder_t* coder = nbt_coder_create();
	nbt_coder_initialize_decoder(coder, bytes, length);
	nbt_t* tag = _nbt_parse_coder(coder, order, errorp);
	nbt_coder_destroy(coder);
	return tag;
}

nbt_t* _nbt_parse_coder(nbt_coder_t* coder, nbt_byte_order_t order, nbt_status_t* errorp) {
	nbt_type_t type = nbt_coder_decode_byte(coder);
	if (!type) {
		return NULL;
	}
	int16_t name_length = nbt_coder_decode_short(coder, order);
	char* name = malloc(name_length + 1);
	nbt_coder_decode_data(coder, name, name_length);
	name[name_length] = '\0';
	nbt_t* tag = _nbt_parse_payload(type, name, coder, order, errorp);
	free(name);
	return tag;
}

nbt_t* _nbt_parse_payload(nbt_type_t type, const char* name, nbt_coder_t* coder, nbt_byte_order_t order, nbt_status_t* errorp) {
	switch (type) {
		case NBT_END:
			return NULL;
		case NBT_BYTE:
			return nbt_create_byte(name, nbt_coder_decode_byte(coder));
		case NBT_SHORT:
			return nbt_create_short(name, nbt_coder_decode_short(coder, order));
		case NBT_INT:
			return nbt_create_int(name, nbt_coder_decode_int(coder, order));
		case NBT_LONG:
			return nbt_create_long(name, nbt_coder_decode_long(coder, order));
		case NBT_FLOAT:
			return nbt_create_float(name, nbt_coder_decode_float(coder, order));
		case NBT_DOUBLE:
			return nbt_create_double(name, nbt_coder_decode_double(coder, order));
		case NBT_BYTE_ARRAY: {
			int32_t length = nbt_coder_decode_int(coder, order);
			int8_t array[length];
			nbt_coder_decode_data(coder, (char*)array, length);
			nbt_t* tag = nbt_create_byte_array(name, array, length);
			return tag;
		}
		case NBT_INT_ARRAY: {
			int32_t length = nbt_coder_decode_int(coder, order);
			int32_t array[length * sizeof(int32_t)];
			for (int32_t i = 0; i < length; i++) {
				array[i] = nbt_coder_decode_int(coder, order);
			}
			nbt_t* tag = nbt_create_int_array(name, array, length);
			return tag;
		}
		case NBT_STRING: {
			int16_t length = nbt_coder_decode_short(coder, order);
			char* string = malloc(length + 1);
			nbt_coder_decode_data(coder, string, length);
			string[length] = '\0';
		}
		case NBT_LIST: {
			nbt_type_t list_type = nbt_coder_decode_byte(coder);
			nbt_t* tag = nbt_create_list(name, list_type);
			int32_t count = nbt_coder_decode_int(coder, order);
			for (int32_t i = 0; i < count; i++) {
#warning NBT_List is not implemented
			}
			return tag;
		}
		case NBT_COMPOUND: {
			nbt_t* tag = nbt_create_compound(name);
			nbt_t* next = NULL;
			while ((next = _nbt_parse_coder(coder, order, errorp))) {
#warning NBT_Compound is not implemented
			}
			return tag;
		}
	}
}
