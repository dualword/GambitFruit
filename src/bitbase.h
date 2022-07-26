// bitbase.h

#ifndef BIT_H
#define BIT_H

// includes
#include "util.h"

#define MAX_STR            256
#define max(a, b)        (((a) > (b)) ? (a) : (b))
#define f_distance(x,y)  abs(sfile(x)-sfile(y))
#define r_distance(x,y)  abs(srank(x)-srank(y))
#define sdistance(x,y)    max(f_distance(x,y),r_distance(x,y))

#define sfile(x)          ((x) &  7)
#define srank(x)          ((x) >> 4)
#define srank64(x)        ((x) >> 3)
#define sSQ(x,y)          (((x) << 4) | (y))
#define sSQ64(x,y)        (((x) << 3) | (y))
#define sSQ8864(x)        sSQ64(srank(x),sfile(x))
#define sSQ6488(x)        sSQ(srank64(x),sfile(x))

static uint16 KK_index[4096];
static uint16 KK1_index[640];
static uint16 PP_index[1152];
static uint8 K_TR[64] = {
	0, 1, 2, 3, 3, 2, 1, 0,
	1, 4, 5, 6, 6, 5, 4, 1,
	2, 5, 7, 8, 8, 7, 5, 2,
	3, 6, 8, 9, 9, 8, 6, 3,
	3, 6, 8, 9, 9, 8, 6, 3,
	2, 5, 7, 8, 8, 7, 5, 2,
	1, 4, 5, 6, 6, 5, 4, 1,
	0, 1, 2, 3, 3, 2, 1, 0
};

struct BITBASE {
	char* name;
	bool is_loaded;   
	uint32  size;
    	uint8*  table;
};

//total size is 18MB
static BITBASE bitbases[] = {
	{"kpk", false, 21672, NULL},
    {"kppk", false, 1040256, NULL},
	{"kpkp", false, 1040256, NULL},
	{"knkp", false, 1387008, NULL},
    {"kbkp", false, 1387008, NULL},
	{"knpk", false, 1387008, NULL},
	{"kbpk", false, 1387008, NULL},
	{"krkp", false, 2774016, NULL},
    {"kqkp", false, 2774016, NULL},
	{"krkn", false, 655360, NULL},
	{"krkb", false, 655360, NULL},
	{"kqkn", false, 655360, NULL},
	{"kqkb", false, 655360, NULL},
    {"krkr", false, 655360, NULL},
	{"kqkr", false, 1310720, NULL},
    {"kqkq", false, 655360, NULL},
	{"krk", false, 10240, NULL},
    {"kqk", false, 10240, NULL},
	{NULL,false,0,NULL}
};

void init_bitbases();

#endif
