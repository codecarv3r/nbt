//
//  main.c
//  nbt_dump
//
//  Created by Silas Schwarz on 7/11/16.
//  Copyright © 2016 ZCodeMT. All rights reserved.
//

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
	char* print = nbt_print(tag, NBT_STYLE_ORIGINAL);
	printf("%s\n", print);
	nbt_destroy(tag);
    return 0;
}
