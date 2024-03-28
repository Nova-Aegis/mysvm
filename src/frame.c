#include "frame.h"
#include "pair.h"
#include "varray.h"

int incr_varray_frame(varray_frame_t*array) {
	array->size *= 2;
	frame_t*new = (frame_t*) malloc(sizeof(frame_t) * INITIAL_SIZE);
	if (!new) {
		fprintf(stderr, "error [incr_varray_frame] malloc error frame_t\n");
		return 1;
	}
	for (int i = 0; i< array->top; i++) {
		new[i].pc = array->frames[i].pc;
		new[i].localEnv_size = array->frames[i].localEnv_size;
	}
	free(array->frames);
	array->frames = new;
	return 0;
}

/**
 * create varray_frame_t with INITIAL_SIZE
 * return 0 on success
 **/
int create_varray_frame(varray_frame_t**result) {
	// create varray for frame
	varray_frame_t*new = (varray_frame_t*) malloc(sizeof(varray_frame_t));
	if (!new) {
		fprintf(stderr, "error [create_varray_frame] malloc error varray_frame_t\n");
		return 1;
	}
	// create starting array of frame
	new->frames = (frame_t*) malloc(sizeof(frame_t) * INITIAL_SIZE);
	if (!new->frames) {
		fprintf(stderr, "error [create_varray_frame] malloc error frame_t\n");
		free(new);
		return 1;
	}
	// initialise values
	new->size = INITIAL_SIZE;
	new->top = 0;
	// return value
	*result = new;
	return 0;
}

void free_varray_frame(varray_frame_t*array) {
	free(array->frames);
	free(array);
}

int varray_frame_add(varray_frame_t*array,intptr_t nb_args,int*result) {
	if (array->top == size) {
		return 1;
	}
	array->frame[array->top].localEnv_size = nb_args;
	if (int) *int = array->top;
	array->top++;
	return 0;
}

int varray_frame_set_pc(varray_frame_t*array, int pos, int pc) {
	if (pos < 0 || pos > array->top) {
		fprintf(stderr, "error [varray_frame_set_pc] given pos is out of bounds\n");
		return 1;
	}
	array->frames[pos].pc = pc;
	return 0;
}

int varray_frame_get_pc(varray_frame_t*array, int pos, int*pc) {
	if (pos < 0 || pos > array->top) {
		fprintf(stderr, "error [varray_frame_set_pc] given pos is out of bounds\n");
		return 1;
	}
	array->frames[pos].pc = pc;
	return 0;
}
