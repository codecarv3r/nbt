/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  nbt.c
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

nbt_t* nbt_create() {
	nbt_t* tag = malloc(sizeof(*tag));
	memset(tag, 0, sizeof(*tag));
	return tag;
}

nbt_t* nbt_create_byte(const char* name, int8_t payload) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_byte = payload;
	return tag;
}

nbt_t* nbt_create_short(const char* name, int16_t payload) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_short = payload;
	return tag;
}

nbt_t* nbt_create_int(const char* name, int32_t payload) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_int = payload;
	return tag;
}

nbt_t* nbt_create_long(const char* name, int64_t payload) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_long = payload;
	return tag;
}

nbt_t* nbt_create_float(const char* name, float payload) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_float = payload;
	return tag;
}

nbt_t* nbt_create_double(const char* name, double payload) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_double = payload;
	return tag;
}

nbt_t* nbt_create_byte_array(const char* name, const int8_t* bytes, int32_t length) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_byte_array.length = length;
	tag->payload.tag_byte_array.byte_array = malloc(sizeof(int8_t) * length);
	memcpy(tag->payload.tag_byte_array.byte_array, bytes, sizeof(int8_t) * length);
	return tag;
}

nbt_t* nbt_create_int_array(const char* name, const int32_t* ints, int32_t length) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_int_array.length = length;
	tag->payload.tag_int_array.int_array = malloc(sizeof(int32_t) * length);
	memcpy(tag->payload.tag_int_array.int_array, ints, sizeof(int32_t) * length);
	return tag;
}

nbt_t* nbt_create_list(const char* name, nbt_type_t type) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_list.type = type;
	return tag;
}

nbt_t* nbt_create_compound(const char* name) {
	nbt_t* tag = nbt_create();
	if (name) {
		tag->name = strdup(name);
	}
	return tag;
}

void nbt_destroy(nbt_t* tag) {
	if (tag) {
		switch (tag->type) {
			case NBT_BYTE_ARRAY:
				free(tag->payload.tag_byte_array.byte_array);
				break;
			case NBT_STRING:
				free(tag->payload.tag_string);
				break;
			case NBT_LIST:
				nbt_destroy(tag->payload.tag_list.tree);
				break;
			case NBT_COMPOUND:
				nbt_destroy(tag->payload.tag_compound);
				break;
			case NBT_INT_ARRAY:
				free(tag->payload.tag_int_array.int_array);
				break;
			default:
				break;
		}
		free(tag);
	}
}
