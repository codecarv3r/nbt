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
 *  Created by Silas Schwarz on 7/11/16.
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

#include <stdlib.h>
#include <stdio.h>

#include "nbt.h"

int main(int argc, const char * argv[]) {
	FILE* fp = fopen("/Users/silas/Code/nbt/test_data/level_unzipped.nbt", "r");
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* data = malloc(size);
	fread(data, size, 1, fp);
	fclose(fp);
	nbt_status_t status;
	nbt_t* tag = nbt_parse_data(data, size, NBT_BIG_ENDIAN, &status);
	free(data);
	char* print = nbt_print(tag, NBT_STYLE_PIPE);
	printf("%s\n", print);
	nbt_destroy(tag);
	return 0;
}
