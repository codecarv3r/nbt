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

#ifndef USE_READLINE
#define USE_READLINE 1
#endif /* !defined(USE_READLINE) */

#include "commands.h"

#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#if USE_READLINE
#include <editline/readline.h>
#endif /* USE_READLINE */

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

char* command_token(const char* command, uintptr_t* index);
char* prompt_command(const char* prompt);
void edit_tag(nbt_t* tag, const char* path, bool* exit, bool* no_output);

bool edit_exit_main(size_t argc, const char* argv[], bool* no_output);

#define EXPECTED_MAX_TOKEN 16

int edit_main(int argc, const char* argv[]) {
	int option;
	char* path = NULL;
	char* output = NULL;
	char* endian = NULL;
	int option_index = 0;
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
	optind = 1;
	
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
	
	bool no_output = false;
	bool exit = false;
	edit_tag(tag, "/", &exit, &no_output);
	
	if (!no_output) {
		printf("Outputting...\n");
	}
	
	nbt_release(tag);
	free(output);
	return 0;
}

void edit_tag(nbt_t* tag, const char* path, bool* exit, bool* no_output) {
	while (true) {
		char* prompt = nbt_printf(XLRED "[" XLYELLOW " %s " XLRED "] " XLGREEN "> " RESET, path);
		char* command = prompt_command(prompt);
		
		char** tokens = malloc(EXPECTED_MAX_TOKEN * sizeof(*tokens));
		size_t token_count = 0;
		size_t token_reserved = EXPECTED_MAX_TOKEN;
		char* token;
		uintptr_t index = 0;
		token = command_token(command, &index);
		if (token) {
			tokens[0] = token;
			token_count++;
		} else {
			continue;
		}
		do {
			if (token_count == token_reserved) {
				token_reserved <<= 1;
				tokens = realloc(tokens, token_reserved * sizeof(*tokens));
			}
			tokens[token_count] = token;
			token_count++;
		} while ((token = command_token(command, &index)));
		if (token_count == token_reserved) {
			token_reserved <<= 1;
			tokens = realloc(tokens, token_reserved * sizeof(*tokens));
		}
		tokens[token_count] = NULL;
		
		bool exit = false;
		if (!strcmp(tokens[0], "exit")) {
			exit = edit_exit_main(token_count, (const char **)tokens, no_output);
		}
		
		for (size_t i = 0; i < token_count; i++) {
			free(tokens[i]);
		}
		free(tokens);
		
#if USE_READLINE
		add_history(command);
#endif /* USE_READLINE */
		
		free(prompt);
		free(command);
		
		if (exit) {
			break;
		}
	}
}

char* command_token(const char* command, uintptr_t* index) {
	enum {
		IN_NONE,
		IN_QUOTE_STRING,
		IN_QUOTE_CHAR,
		IN_WORD
	} state = IN_NONE;
	intptr_t word_start = -1;
	while (command[*index]) {
		unsigned char next = command[*index];
		switch (state) {
			case IN_NONE:
				if (isspace(next)) {
					break;
				} else if (next == '"') {
					state = IN_QUOTE_STRING;
				} else if (next == '\'') {
					state = IN_QUOTE_CHAR;
				} else {
					state = IN_WORD;
				}
				word_start = *index;
				break;
			case IN_QUOTE_STRING:
				if (next == '"') {
					(*index)++;
					return strndup(command + word_start + 1, (*index) - word_start - 2);
				}
				break;
			case IN_QUOTE_CHAR:
				if (next == '\'') {
					(*index)++;
					return strndup(command + word_start + 1, (*index) - word_start - 2);
				}
				break;
			case IN_WORD:
				if (isspace(next)) {
					(*index)++;
					return strndup(command + word_start, (*index) - word_start - 1);
				} else if (next == '"' || next == '\'') {
					(*index)++;
					return strndup(command + word_start, (*index) - word_start - 1);
				}
				break;
		}
		(*index)++;
	}
	if (word_start != -1) {
		return strdup(command + word_start);
	}
	return NULL;
}

#if USE_READLINE
char* prompt_command(const char* prompt) {
	return readline(prompt);
}
#else
char* prompt_command(const char* prompt) {
	printf("%s", prompt);
	char ret[1024];
	memset(ret, 0, sizeof(ret));
	fgets(ret, sizeof(ret), stdin);
	return strdup(ret);
}
#endif /* USE_READLINE */

static const struct option exit_options[] = {
	{ "no_output", no_argument, NULL, 'n' },
	{ NULL, 0, NULL, 0 }
};

bool edit_exit_main(size_t argc, const char* argv[], bool* no_output) {
	*no_output = false;
	int option;
	int option_index = 0;
	while ((option = getopt_long((int)argc, (char*const*)argv, "n", exit_options, &option_index)) != -1) {
		switch (option) {
			case 'n':
				*no_output = true;
				break;
			case '?':
				break;
		}
	}
	return true;
}
