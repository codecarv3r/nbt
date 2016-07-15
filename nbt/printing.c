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
#include "colors.h"

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
char* _nbt_tabs(size_t count);
char* _nbt_spaces(size_t count);

char* _nbt_print_original(nbt_t* tag, int tab_count);
char* _nbt_print_pipe(nbt_t* tag, const char* start_string);
char* _nbt_print_color(nbt_t* tag, int tab_count);

char* nbt_print(nbt_t* tag, nbt_print_style_t style) {
	switch (style) {
		case NBT_STYLE_ORIGINAL:
			return _nbt_print_original(tag, 0);
			break;
		case NBT_STYLE_PIPE:
			return _nbt_print_pipe(tag, "");
			break;
		case NBT_STYLE_COLOR:
			return _nbt_print_color(tag, 0);
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

char* _nbt_tabs(size_t count) {
	char* print = malloc(count + 1);
	memset(print, '\t', count);
	print[count] = '\0';
	return print;
}

char* _nbt_spaces(size_t count) {
	char* print = malloc(count + 1);
	memset(print, ' ', count);
	print[count] = '\0';
	return print;
}

char* _nbt_print_original(nbt_t* tag, int tab_count) {
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
				char* next = _nbt_print_original(item, tab_count + 1);
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
				char* next = _nbt_print_original(item, tab_count + 1);
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
	free(tabs);
	return print;
}

char* _nbt_print_pipe(nbt_t* tag, const char* start_string) {
	char* start;
	if (tag->name) {
		start = _nbt_printf("%s(\"%s\")", _nbt_type_names[tag->type], tag->name);
	} else {
		start = _nbt_printf("%s", _nbt_type_names[tag->type]);
	}
	char* print;
	switch (tag->type) {
		case NBT_END:
			print = strdup("");
			break;
		case NBT_BYTE:
			print = _nbt_printf("%s: %d\n", start, tag->payload.tag_byte);
			break;
		case NBT_SHORT:
			print = _nbt_printf("%s: %d\n", start, tag->payload.tag_short);
			break;
		case NBT_INT:
			print = _nbt_printf("%s: %d\n", start, tag->payload.tag_int);
			break;
		case NBT_LONG:
			print = _nbt_printf("%s: %lld\n", start, tag->payload.tag_long);
			break;
		case NBT_FLOAT:
			print = _nbt_printf("%s: %f\n", start, tag->payload.tag_float);
			break;
		case NBT_DOUBLE:
			print = _nbt_printf("%s: %lf\n", start, tag->payload.tag_double);
			break;
		case NBT_STRING:
			print = _nbt_printf("%s: %s\n", start, tag->payload.tag_string);
			break;
		case NBT_BYTE_ARRAY:
			print = _nbt_printf("%s: [%d bytes]\n", start, tag->payload.tag_byte_array.length);
			break;
		case NBT_INT_ARRAY:
			print = _nbt_printf("%s: [%d ints]\n", start, tag->payload.tag_int_array.length);
			break;
		case NBT_LIST:
		{
			char* spaces = _nbt_spaces(strlen(start) + 2);
			char* new_start_string_pipe    = _nbt_printf("%s%s│  ", start_string, spaces);
			char* new_start_string_no_pipe = _nbt_printf("%s%s   ", start_string, spaces);
			char* new_start_string_before  = _nbt_printf("%s%s", start_string, spaces);
			free(spaces);
			char* temp;
			nbt_t* item = tag->payload.tag_list.tree;
			if (item->tree_right) {
				char* child_print = _nbt_print_pipe(item, new_start_string_pipe);
				print = _nbt_printf("%s ─┬─ %s", start, child_print);
				free(child_print);
				item = item->tree_right;
				while (item->tree_right) {
					child_print = _nbt_print_pipe(item, new_start_string_pipe);
					temp = _nbt_printf("%s%s├─ %s", print, new_start_string_before, child_print);
					free(print);
					print = temp;
					free(child_print);
					item = item->tree_right;
				}
				child_print = _nbt_print_pipe(item, new_start_string_no_pipe);
				temp = _nbt_printf("%s%s└─ %s", print, new_start_string_before, child_print);
				free(print);
				print = temp;
				free(child_print);
			} else {
				char* child_print = _nbt_print_pipe(item, new_start_string_no_pipe);
				print = _nbt_printf("%s ─── %s", start, child_print);
				free(child_print);
			}
			free(new_start_string_pipe);
			free(new_start_string_no_pipe);
			free(new_start_string_before);
			break;
		}
		case NBT_COMPOUND:
		{
			char* spaces = _nbt_spaces(strlen(start) + 2);
			char* new_start_string_pipe    = _nbt_printf("%s%s│  ", start_string, spaces);
			char* new_start_string_no_pipe = _nbt_printf("%s%s   ", start_string, spaces);
			char* new_start_string_before  = _nbt_printf("%s%s", start_string, spaces);
			free(spaces);
			char* temp;
			nbt_t* item = tag->payload.tag_compound;
			if (item->tree_right) {
				char* child_print = _nbt_print_pipe(item, new_start_string_pipe);
				print = _nbt_printf("%s ─┬─ %s", start, child_print);
				free(child_print);
				item = item->tree_right;
				while (item->tree_right) {
					child_print = _nbt_print_pipe(item, new_start_string_pipe);
					temp = _nbt_printf("%s%s├─ %s", print, new_start_string_before, child_print);
					free(print);
					print = temp;
					free(child_print);
					item = item->tree_right;
				}
				child_print = _nbt_print_pipe(item, new_start_string_no_pipe);
				temp = _nbt_printf("%s%s└─ %s", print, new_start_string_before, child_print);
				free(print);
				print = temp;
				free(child_print);
			} else {
				char* child_print = _nbt_print_pipe(item, new_start_string_no_pipe);
				print = _nbt_printf("%s ─── %s", start, child_print);
				free(child_print);
			}
			free(new_start_string_pipe);
			free(new_start_string_no_pipe);
			free(new_start_string_before);
			break;
		}
	}
	free(start);
	return print;
}

char* _nbt_print_color(nbt_t* tag, int tab_count) {
	char* start;
	if (tag->name) {
		start = _nbt_printf(XLBLUE "%s" XYELLOW "(" "\x1b[38;5;208m" "\"%s\"" XYELLOW ")" RESET, _nbt_type_names[tag->type], tag->name);
	} else {
		start = _nbt_printf(XLBLUE "%s" RESET, _nbt_type_names[tag->type]);
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
			print = _nbt_printf("%s%s: %d entries of type " XLBLUE "%s" XLBLUE "\n%s" XYELLOW "{" RESET "\n", tabs, start, nbt_list_count(tag), _nbt_type_names[tag->payload.tag_list.type], tabs);
			nbt_t* item = tag->payload.tag_list.tree;
			char* temp;
			do {
				char* next = _nbt_print_color(item, tab_count + 1);
				temp = _nbt_printf("%s%s", print, next);
				free(print);
				free(next);
				print = temp;
			} while ((item = item->tree_right));
			temp = _nbt_printf("%s%s" XYELLOW "}" RESET "\n", print, tabs);
			free(print);
			print = temp;
			break;
		}
		case NBT_COMPOUND:
			print = _nbt_printf("%s%s: %d entries\n%s" XYELLOW "{" RESET "\n", tabs, start, _nbt_tree_count(tag->payload.tag_compound), tabs);
			nbt_t* item = tag->payload.tag_compound;
			char* temp;
			do {
				char* next = _nbt_print_color(item, tab_count + 1);
				temp = _nbt_printf("%s%s", print, next);
				free(print);
				free(next);
				print = temp;
			} while ((item = item->tree_right));
			temp = _nbt_printf("%s%s" XYELLOW "}" RESET "\n", print, tabs);
			free(print);
			print = temp;
			break;
	}
	free(tabs);
	return print;
}
