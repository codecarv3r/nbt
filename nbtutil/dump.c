/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  dump.c
 *  This file is part of nbt.
 *
 *  Created by Silas Schwarz on 7/16/16.
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

#include "commands.h"

#include <assert.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "nbt.h"

static const struct option options[] = {
	{ "path", required_argument, NULL, 'p' },
	{ "style", required_argument, NULL, 's' },
	{ "endian", required_argument, NULL, 'e' },
	{ "order", required_argument, NULL, 'e' },
	{ "byte_order", required_argument, NULL, 'e' },
	{ "uncompressed", no_argument, NULL, 'u' },
	{ NULL, 0, NULL, 0 }
};

int dump_main(int argc, const char* argv[]) {
	int option;
	char* path = NULL;
	char* style = NULL;
	char* endian = NULL;
	int option_index;
	bool compressed = true;
	while ((option = getopt_long(argc - 1, (char*const*)&argv[1], "p:s:e:u", options, &option_index)) != -1) {
		switch (option) {
			case 'p':
				path = strdup(optarg);
				break;
			case 's':
				style = strdup(optarg);
				break;
			case 'e':
				endian = strdup(optarg);
				break;
			case 'u':
				compressed = false;
				break;
			case '?':
				return 1;
		}
	}
	optind = 1;
	
	if (!path) {
		printf("You forgot to give a path to dump the data from\n");
		free(path);
		free(style);
		free(endian);
		return 1;
	}
	nbt_print_style_t print_style = NBT_STYLE_ORIGINAL;
	if (style) {
		if (!strcmp(style, "original")) {
			
		} else if (!strcmp(style, "pipe")) {
			print_style = NBT_STYLE_PIPE;
		} else if (!strcmp(style, "color")) {
			print_style = NBT_STYLE_COLOR;
		} else {
			printf("Unknown dump style: %s\n", style);
		}
		free(style);
	}
	nbt_byte_order_t order = nbt_native_byte_order;
	if (endian) {
		if (!strcmp(endian, "native")) {
			
		} else if (!strcmp(endian, "little")) {
			order = NBT_LITTLE_ENDIAN;
		} else if (!strcmp(endian, "big")) {
			order = NBT_BIG_ENDIAN;
		} else {
			printf("Unknown byte order: %s\n", endian);
		}
		free(endian);
	}
	nbt_coder_t* coder = nbt_coder_create_file(path);
	free(path);
	nbt_status_t error = NBT_SUCCESS;
	nbt_t* tag = nbt_parse_coder(coder, order, compressed, &error);
	nbt_coder_release(coder);
	assert(!error);
	char* dump = nbt_print(tag, print_style);
	printf("%s", dump);
	free(dump);
	nbt_release(tag);
	return 0;
}
