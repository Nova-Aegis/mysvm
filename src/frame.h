#include "varray.h"
#include "pair.h"
#include <stdint.h>
#define __FRAME_H_

typedef struct frame_s {
	intptr_t pc;
	intptr_t localEnv_size;
} frame_t;


typedef struct varray_frame_s {
	frame_t*frames;
	int size;
	int top;
} varray_frame_t;

/**
 * create varray_frame_t with INITIAL_SIZE
 * return 0 on success
 **/
int create_varray_frame(varray_frame_t**result);

void free_varray_frame(varray_frame_t*array);

int varray_frame_add(varray_frame_t*array,intptr_t nb_args);

int varray_frame_set_pc(varray_frame_t*array, int pos, int pc);
int varray_frame_get_pc(varray_frame_t*array, int pos, int*pc);
