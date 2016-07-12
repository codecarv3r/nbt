/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  printing.c
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

#ifndef __printflike
# define __printflike(a, b)
#endif

const char* _nbt_type_names[] = {
	"TAG_End",
	"TAG_Byte",
	"TAG_Short",
	"TAG_Int",
	"TAG_Long",
	"TAG_Float",
	"TAG_Double",
	"TAG_Byte_Array",
	"TAG_String",
	"TAG_List",
	"TAG_Compound",
	"TAG_Int_Array"
};

char* _nbt_printf(const char* format, ...) __printflike(1, 2);
char* _nbt_vprintf(const char* format, va_list ap) __printflike(1, 0);
char* _nbt_tabs(int count);

char* _nbt_print_original(nbt_t* tag);
char* _nbt_print_original_tab(nbt_t* tag, int tab_count);

char* nbt_print(nbt_t* tag, nbt_print_style_t style) {
	switch (style) {
		case NBT_STYLE_ORIGINAL:
			return _nbt_print_original(tag);
			break;
		case NBT_STYLE_FANCY:
			return strdup("");
			break;
	}
}

char* _nbt_printf(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	char* print = _nbt_vprintf(format, ap);
	va_end(ap);
	return print;
}

char* _nbt_vprintf(const char* format, va_list ap) {
	char* print;
	vasprintf(&print, format, ap);
	return print;
}

char* _nbt_tabs(int count) {
	char* print = malloc(count + 1);
	memset(print, '\t', count);
	print[count] = '\0';
	return print;
}

char* _nbt_print_original(nbt_t* tag) {
	return _nbt_print_original_tab(tag, 0);
}

char* _nbt_print_original_tab(nbt_t* tag, int tab_count) {
	char* start;
	if (tag->name) {
		start = _nbt_printf("%s(\"%s\")", _nbt_type_names[tag->type], tag->name);
	} else {
		start = _nbt_printf("%s", _nbt_type_names[tag->type]);
	}
	char* tabs = _nbt_tabs(tab_count);
	char* print;
	switch (tag->type) {
		case NBT_END:
			print = strdup("");
			break;
		case NBT_BYTE:
			print = _nbt_printf("%s%s: %d\n", tabs, start, tag->payload.tag_byte);
			break;
		case NBT_SHORT:
			print = _nbt_printf("%s%s: %d\n", tabs, start, tag->payload.tag_short);
			break;
		case NBT_INT:
			print = _nbt_printf("%s%s: %d\n", tabs, start, tag->payload.tag_int);
			break;
		case NBT_LONG:
			print = _nbt_printf("%s%s: %lld\n", tabs, start, tag->payload.tag_long);
			break;
		case NBT_FLOAT:
			print = _nbt_printf("%s%s: %f\n", tabs, start, tag->payload.tag_float);
			break;
		case NBT_DOUBLE:
			print = _nbt_printf("%s%s: %lf\n", tabs, start, tag->payload.tag_double);
			break;
		case NBT_STRING:
			print = _nbt_printf("%s%s: %s\n", tabs, start, tag->payload.tag_string);
			break;
		case NBT_BYTE_ARRAY:
			print = _nbt_printf("%s%s: [%d bytes]\n", tabs, start, tag->payload.tag_byte_array.length);
			break;
		case NBT_INT_ARRAY:
			print = _nbt_printf("%s%s: [%d ints]\n", tabs, start, tag->payload.tag_int_array.length);
			break;
		case NBT_LIST:
		{
			print = _nbt_printf("%s%s: %d entries of type %s\n%s{\n", tabs, start, nbt_list_count(tag), _nbt_type_names[tag->payload.tag_list.type], tabs);
			nbt_t* item = tag->payload.tag_list.tree;
			char* temp;
			do {
				char* next = _nbt_print_original_tab(item, tab_count + 1);
				temp = _nbt_printf("%s%s", print, next);
				free(print);
				free(next);
				print = temp;
			} while ((item = item->tree_right));
			temp = _nbt_printf("%s%s}\n", print, tabs);
			free(print);
			print = temp;
			break;
		}
		case NBT_COMPOUND:
			print = _nbt_printf("%s%s: %d entries\n%s{\n", tabs, start, _nbt_tree_count(tag->payload.tag_compound), tabs);
			nbt_t* item = tag->payload.tag_compound;
			char* temp;
			do {
				char* next = _nbt_print_original_tab(item, tab_count + 1);
				temp = _nbt_printf("%s%s", print, next);
				free(print);
				free(next);
				print = temp;
			} while ((item = item->tree_right));
			temp = _nbt_printf("%s%s}\n", print, tabs);
			free(print);
			print = temp;
			break;
	}
	return print;
}
