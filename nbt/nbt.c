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

nbt_t* _nbt_tree_index(nbt_t* node, int32_t index);
nbt_t* _nbt_tree_end(nbt_t* node);
nbt_t* _nbt_tree_name(nbt_t* node, const char* name);
void _nbt_tree_remove(nbt_t* node);
void _nbt_tree_replace(nbt_t* current, nbt_t* replacement);

nbt_t* nbt_create() {
	nbt_t* tag = malloc(sizeof(*tag));
	memset(tag, 0, sizeof(*tag));
	return tag;
}

nbt_t* nbt_create_byte(const char* name, int8_t payload) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_BYTE;
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_byte = payload;
	return tag;
}

nbt_t* nbt_create_short(const char* name, int16_t payload) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_SHORT;
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_short = payload;
	return tag;
}

nbt_t* nbt_create_int(const char* name, int32_t payload) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_INT;
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_int = payload;
	return tag;
}

nbt_t* nbt_create_long(const char* name, int64_t payload) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_LONG;
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_long = payload;
	return tag;
}

nbt_t* nbt_create_float(const char* name, float payload) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_FLOAT;
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_float = payload;
	return tag;
}

nbt_t* nbt_create_double(const char* name, double payload) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_DOUBLE;
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_double = payload;
	return tag;
}

nbt_t* nbt_create_string(const char* name, const char* payload) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_STRING;
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_string = strdup(payload);
	return tag;
}

nbt_t* nbt_create_byte_array(const char* name, const int8_t* bytes, int32_t length) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_BYTE_ARRAY;
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
	tag->type = NBT_INT_ARRAY;
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
	tag->type = NBT_LIST;
	if (name) {
		tag->name = strdup(name);
	}
	tag->payload.tag_list.type = type;
	return tag;
}

nbt_t* nbt_create_compound(const char* name) {
	nbt_t* tag = nbt_create();
	tag->type = NBT_COMPOUND;
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

int8_t nbt_byte(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_BYTE);
	return tag->payload.tag_byte;
}

int16_t nbt_short(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_SHORT);
	return tag->payload.tag_short;
}

int32_t nbt_int(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_INT);
	return tag->payload.tag_int;
}

int64_t nbt_long(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_LONG);
	return tag->payload.tag_long;
}

float nbt_float(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_FLOAT);
	return tag->payload.tag_float;
}

double nbt_double(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_DOUBLE);
	return tag->payload.tag_double;
}

const int8_t* nbt_byte_array(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_BYTE_ARRAY);
	return tag->payload.tag_byte_array.byte_array;
}

const int32_t* nbt_int_array(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_INT_ARRAY);
	return tag->payload.tag_int_array.int_array;
}

const char* nbt_string(nbt_t* tag) {
	assert(tag);
	assert(tag->type == NBT_STRING);
	return tag->payload.tag_string;
}

/* Working with lists */
int32_t nbt_list_count(nbt_t* list) {
	assert(list);
	assert(list->type == NBT_LIST);
	return _nbt_tree_count(list->payload.tag_list.tree);
}

int32_t _nbt_tree_count(nbt_t* node) {
	if (node) {
		return _nbt_tree_count(node->tree_right) + 1;
	} else {
		return 0;
	}
}

nbt_t* nbt_list_index(nbt_t* list, int32_t index) {
	assert(list);
	assert(list->type == NBT_LIST);
	return _nbt_tree_index(list->payload.tag_list.tree, index);
}

nbt_t* _nbt_tree_index(nbt_t* node, int32_t index) {
	if (node) {
		if (index) {
			return _nbt_tree_index(node->tree_right, index - 1);
		}
		return node;
	}
	return NULL;
}

void nbt_list_add(nbt_t* list, nbt_t* item) {
	assert(list);
	assert(list->type == NBT_LIST);
	assert(list->payload.tag_list.type == item->type);
	if (list->payload.tag_list.tree) {
		nbt_t* node = _nbt_tree_end(list->payload.tag_list.tree);
		node->tree_right = item;
		item->tree_left = node;
	} else {
		list->payload.tag_list.tree = item;
	}
}

nbt_t* _nbt_tree_end(nbt_t* node) {
	if (node->tree_right) {
		return _nbt_tree_end(node->tree_right);
	}
	return node;
}

void nbt_list_remove(nbt_t* list, int32_t index) {
	assert(list);
	assert(list->type == NBT_LIST);
	nbt_t* node = _nbt_tree_index(list->payload.tag_list.tree, index);
	assert(node);
	_nbt_tree_remove(node);
}

void _nbt_tree_remove(nbt_t* node) {
	if (node) {
		if (node->tree_left) {
			node->tree_left->tree_right = node->tree_right;
		}
		if (node->tree_right) {
			node->tree_right->tree_left = node->tree_left;
		}
		nbt_destroy(node);
	}
}

/* Working with compounds */
nbt_t* nbt_compound_name(nbt_t* compound, const char* name) {
	assert(compound);
	assert(compound->type == NBT_COMPOUND);
	return _nbt_tree_name(compound->payload.tag_compound, name);
}

nbt_t* _nbt_tree_name(nbt_t* node, const char* name) {
	if (node) {
		if (strcmp(node->name, name)) {
			return _nbt_tree_name(node->tree_right, name);
		}
		return node;
	}
	return NULL;
}

void nbt_compound_set(nbt_t* compound, nbt_t* item) {
	assert(compound);
	assert(compound->type == NBT_COMPOUND);
	assert(item);
	nbt_t* current = nbt_compound_name(compound, item->name);
	if (current) {
		_nbt_tree_replace(current, item);
	} else {
		if (compound->payload.tag_compound) {
			current = _nbt_tree_end(compound->payload.tag_compound);
			current->tree_right = item;
			item->tree_left = current;
		} else {
			compound->payload.tag_compound = item;
		}
	}
}

void _nbt_tree_replace(nbt_t* current, nbt_t* replacement) {
	if (current->tree_left) {
		current->tree_left->tree_right = replacement;
	}
	if (current->tree_right) {
		current->tree_right->tree_left = replacement;
	}
	nbt_destroy(current);
}

void nbt_compound_remove(nbt_t* compound, const char* name) {
	assert(compound);
	assert(compound->type == NBT_COMPOUND);
	_nbt_tree_remove(nbt_compound_name(compound, name));
}
