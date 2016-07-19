/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  edit.c
 *  This file is part of nbt.
 *
 *  Created by Silas Schwarz on 7/18/16.
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
#include <editline/readline.h>

#include "colors.h"
#include "nbt.h"

static const struct option options[] = {
	{ "path", required_argument, NULL, 'p' },
	{ "endian", required_argument, NULL, 'e' },
	{ "order", required_argument, NULL, 'e' },
	{ "byte_order", required_argument, NULL, 'e' },
	{ "uncompressed", no_argument, NULL, 'u' },
	{ "output", required_argument, NULL, 'o' },
	{ NULL, 0, NULL, 0 }
};

void edit_tag(nbt_t* tag, const char* path);

int edit_main(int argc, const char* argv[]) {
	int option;
	char* path = NULL;
	char* output = NULL;
	char* endian = NULL;
	int option_index;
	bool compressed = true;
	while ((option = getopt_long(argc - 1, (char*const*)&argv[1], "p:e:uo:", options, &option_index)) != -1) {
		switch (option) {
			case 'p':
				path = strdup(optarg);
				break;
			case 'e':
				endian = strdup(optarg);
				break;
			case 'u':
				compressed = false;
				break;
			case 'o':
				output = strdup(optarg);
				break;
			case '?':
				return 1;
		}
	}
	if (!path) {
		printf("You forgot to give a path to get data to manipulate\n");
		free(path);
		free(endian);
		return 1;
	}
	if (!output) {
		printf("No output path was given, so your manipulations will be written to the input file\n");
		output = strdup(path);
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
	edit_tag(tag, XLGREEN "/" RESET);
	nbt_release(tag);
	return 0;
}

void edit_tag(nbt_t* tag, const char* path) {
	while (true) {
		char* prompt = nbt_printf(XLRED "[" XLYELLOW " %s " XLRED "] " XLGREEN "> " RESET, path);
		char* command = readline(prompt);
		free(prompt);
		free(command);
	}
}
