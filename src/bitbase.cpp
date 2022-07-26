#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "bitbase.h"

void init_bitbases() {

	FILE* fd;
	int index,i,j;
	BITBASE* pbbase;
	char bb_name[MAX_STR];

	//initialize legal KK index table,max index 3612
	index = 0;
	for( i = 0;i < 64;i++) {
		for( j = 0; j < 64;j++) {
			if(sdistance(sSQ6488(i),sSQ6488(j)) <= 1)
				continue;
			KK_index[i * 64 + j] = index++;
		}
	}

	//init KK1 index table used when there are no pawns, max index 640
	index = 0;
	for( i = 0;i < 10;i++) {
		for( j = 0; j < 64;j++) {
			KK1_index[i * 64 + j] = index++;
		}
	}

	//initialize PP index table,max index is 1152
	index = 0;
	for(i = 0;i < 24;i++) {
		for(j = 0; j < 48;j++) {
			PP_index[i * 48 + j] = index++;
		}
	}

	//now load available bitbases
	index = 0;
	while((pbbase = &bitbases[index])->name) {
        strcpy(bb_name,"egbb/");
        strcat(bb_name,pbbase->name);
		strcat(bb_name,".dat");
        if(fd = fopen(bb_name,"rb")) {
			pbbase->table = new uint8[pbbase->size];
			for(uint32 k = 0; k < pbbase->size; k++) {
				pbbase->table[k] = fgetc(fd);
			}
			pbbase->is_loaded = true;
			fclose(fd);
		} else {
			printf("%s bitbase not loaded\n",bb_name);
		}

		index++;
	}

}
