/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  main.c
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

#include <stdio.h>
#include <string.h>

#include "commands.h"

void usage(const char* command_call);

int main(int argc, const char * argv[]) {
	if (argc < 2) {
		usage(argv[0]);
		return 0;
	}
	if (!strcmp(argv[1], "dump")) {
		return dump_main(argc, argv);
	} else if (!strcmp(argv[1], "help")) {
		return help_main(argc, argv);
	} else if (!strcmp(argv[1], "edit")) {
		return edit_main(argc, argv);
	} else {
		printf("Unknown command: %s\n", argv[1]);
		usage(argv[0]);
		return 1;
	}
    return 0;
}

void usage(const char* command_call) {
	printf(
		   "Usage: %s <command> <options>...\n"
		   "\tCommands:\n"
		   "\t\thelp [-c <command>]\t\tshow this message, or help for a specific command\n"
		   "\t\tedit -p <path> \t\t\tenter an interactive mode for editing nbt data\n"
		   "\t\tdump -p <path> [-s <style]\tdump a readable form of the nbt data at <path>\n",
		   command_call);
}
