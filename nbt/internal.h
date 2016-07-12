/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  internal.h
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

#ifndef internal_h
#define internal_h

#include "nbt.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

struct _nbt {
	nbt_type_t type;
	char* name;
	
	union nbt_payload {
		int8_t tag_byte;
		int16_t tag_short;
		int32_t tag_int;
		int64_t tag_long;
		float tag_float;
		double tag_double;
		
		struct nbt_byte_array {
			int32_t length;
			int8_t* byte_array;
		} tag_byte_array;
		
		char* tag_string;
		
		struct nbt_list {
			int8_t type;
			nbt_t* tree;
		} tag_list;
		
		nbt_t* tag_compound;
		
		struct nbt_int_array {
			int32_t length;
			int32_t* int_array;
		} tag_int_array;
	} payload;
	
	nbt_t* tree_left;
	nbt_t* tree_right;
};

int32_t _nbt_tree_count(nbt_t* node);

#endif /* internal_h */
