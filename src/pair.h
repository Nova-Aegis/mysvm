#include <stdio.h>
#include <stdint.h>

#define __PAIR_H__

extern const char*OPERATIONS[];
extern const char*TYPES[];
extern const char*PRIMS[];


typedef struct pair_s{
	intptr_t data[2];
	int type[2];
} pair_t;

int create_pair(pair_t**result,intptr_t type1, intptr_t data1, intptr_t type2, intptr_t data2);
int free_pair(pair_t*pair);
int copy_pair(pair_t**pair);
int pair_car(pair_t*pair, intptr_t*type, intptr_t*data);
int pair_cdr(pair_t*pair, intptr_t*type, intptr_t*data);
void print_pair(FILE*output, pair_t*pair);
