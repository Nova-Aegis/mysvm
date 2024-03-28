#ifndef __VARRAY_H__
#include "varray.h"
#define __VARRAY_H_
#endif
#ifndef __PAIR_H__
#include "pair.h"
#define __PAIR_H_
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

/**
 * void incr_size_varray(varray_t*array);
 * internal function :
 * doubles the size of the varray
 * returns 0 on success
 */
int incr_size_varray(varray_t*array) {
	// creating new array
	var_data_t*new = (var_data_t*)malloc(sizeof(var_data_t) * array->size*2);
	if (!new) {
		fprintf(stderr, "error [incr_size_varray] malloc error\n");
		return 1;
	}
	// copying data
	int i = 0;
	for (; i<array->top; i++) {
		new[i].data = array->data[i].data;
		new[i].type = array->data[i].type;
	}
	// modifying structure
	free(array->data);
	array->data = new;
	array->size = array->size*2;
	// initialising empty space
	for (; i<array->size; i++) {
		new[i].type = -1;
	}
	return 0;
}

/**
 * void incr_size_varray_code(varray_code_t*array);
 * internal function :
 * doubles the size of the varray
 * returns 0 on success
 */
/* UNUSED
int incr_size_varray_code(varray_code_t*array) {
	// create new array
	int*new = (int*)malloc(sizeof(int) * array->size*2);
	if (!new) return 1;
	// copying data
	int i = 0;
	for (; i<array->size; i++) {
		new[i] = array->code[i];
	}
	//modifying structure
	array->size = array->size*2;
	free(array->code);
	array->code = new;
	// initialising empty space
	for (; i<array->size; i++) {
		new[i].code = -1;
	}
}
*/


/**
 * Get the data structure of position `pos` from the `array`
 * if `pos` is not initialised type = -1 is returned
 * returns 0 on success
 **/
int gfetch(varray_t*array, intptr_t pos, intptr_t*type, intptr_t*data) {
	// testing pos
	if ((pos < 0) || (pos > array->size)) {
		fprintf(stderr, "error [fetch] position is not mapped\n");
		return 1;
	}
	// returning
	*data = array->data[pos].data;
	*type = array->data[pos].type;
	return 0;
}

/**
 * store the data structure in position `pos` in the `array`
 * if `pos` is not acceptable exit is called
 * returns 0 on success
 **/
int gstore(varray_t*array, intptr_t pos, intptr_t type, intptr_t data) {
	// testing pos
	if (pos < 0) {
		fprintf(stderr, "error [store] pos < 0\n");
		return 1;
	}
	// allocating space as needed
	while (pos >= array->size && incr_size_varray(array)) {
		fprintf(stderr, "error [gstore] - could not increase size\n");
		return 1;
	}
	// storing data
	array->data[pos].data = data;
	array->data[pos].type = type;
	return 0;
}




/**
 * Get the data structure of position `pos` from the `array`
 * if `pos` is not initialised type = -1 is returned
 * returns 0 on success
 **/
int fetch(varray_t*array, intptr_t pos, intptr_t*type, intptr_t*data) {
	// testing pos
	if ((array->top-pos-1 < 0) || (pos < 0)) {
		fprintf(stderr, "{top : %"PRIdPTR" | size : %"PRIdPTR" | pos : %"PRIdPTR"}", array->top, array->size, pos);
		fprintf(stderr, "error [fetch] position %"PRIdPTR" is not mapped\n", pos);
		return 1;
	}
	// returning
	*data = array->data[array->top-pos-1].data;
	*type = array->data[array->top-pos-1].type;
	return 0;
}

/**
 * store the data structure in position `pos` in the `array`
 * if `pos` is not acceptable exit is called
 * returns 0 on success
 **/
int store(varray_t*array, intptr_t pos, intptr_t type, intptr_t data) {
	// testing pos
	if (array->top-pos-1 < 0) {
		fprintf(stderr, "error [store] pos < 0\n");
		return 1;
	}
	// allocating space as needed
	while (pos >= array->size) {
		if (incr_size_varray(array)) {
			fprintf(stderr, "error [store] - could not increase size\n");
			return 1;
		}
	}
	// storing data
	array->data[array->top-pos-1].data = data;
	array->data[array->top-pos-1].type = type;
	return 0;
}

/**
 * Get the next code of the code structure and advance
 * returns 0 on success
 **/
int next_code(varray_code_t*array, intptr_t*result) {
	// setting pc to next byte
	array->pc++;
	// testing size
	if (array->pc == array->size+1) {
		fprintf(stderr, "error [next_code] cp is out of bounds\n");
		return 1;
	}
	// returning
  *result = array->code[array->pc];
	return 0;
}

/**
 * Goto the code label
 * returns 0 on success
 **/
int goto_code(varray_code_t*array, intptr_t pos) {
	// testing pos
	if (pos < 0 || pos >= array->size) {
		fprintf(stderr, "error [goto_code] given position is out of bounds\n");
		return 1;
	}
	// set new pc
	array->pc = pos;
	return 0;
}


/**
 * push a `data` of `type` to the `array`
 * returns 0 on success
 **/
int push(varray_t*array, intptr_t type, intptr_t data) {
	// increase size if necessary
	if (array->top + 1 == array->size && incr_size_varray(array)) {
		fprintf(stderr, "error [push] - could not increase size\n");
		return 1;
	}
	// storing last data
	array->data[array->top].data = data;
	array->data[array->top].type = type;
	array->top++;
	return 0;
}

/**
 * pops a data_structure from the `array`
 * if `array` is empty then return an error
 * - get data from passed argument pointer `data`
 * - get type from passed argument pointer `type`
 * returns 0 on success
 **/
int pop(varray_t*array, intptr_t*type, intptr_t*data) {
	// testing top
	if (array->top == 0){
		fprintf(stderr, "error [varray.c] [pop()] varray is empty\n");
		return 1;
	}
	// returning
	array->top--;
	*data = array->data[array->top].data;
	*type = array->data[array->top].type;
	// delete last
	array->data[array->top].type = -1;
	return 0;
}


/**
 * varray_code_t*create_varray_code(int*tab, int size);
 * creates the code array from passed `tab`
 * returns 0 on success
 **/
int create_varray_code(const char*filename, varray_code_t**result) {
	// open file
	FILE*bytecode = fopen(filename,"r");
	if (!bytecode) {
		fprintf(stderr, "input file [%s] could not be opened\n", filename);
		return 1;
	}
	// creating array
	varray_code_t*new = (varray_code_t*)malloc(sizeof(varray_code_t));
	new->pc = -1;
	intptr_t begining;
	// get starting information
	if (fscanf(bytecode, "%"SCNdPTR" %"SCNdPTR" ", &begining, &(new->size))!=2 || begining!=424242) {
		fprintf(stdout, "input file [%s] is not a recognized bytecode file\n", filename);
		fprintf(stdout, "existing . . .\n");
	  free(new);
		return 1;
	}
	new->code = (intptr_t*) malloc(sizeof(intptr_t) * new->size+1);
	// load code
	intptr_t tmp;
	int i;
	for (i = 0; i<new->size; i++) {
		if (fscanf(bytecode, "%"SCNdPTR" ", &tmp) != 1) {
			fprintf(stdout, "file format not acceptable (int n# %d)\nexiting . . .\n", i+2);
		  free(new->code);
			free(new);
			return 1;
		}
		new->code[i] = tmp;
	}
	// setting exit
	new->code[i] = -1;
	*result = new;
	return 0;
}


/**
 * create a varray of INITIAL_SIZE
 * return 0 on success
 **/
int create_varray(varray_t**result){
	// creating structure and list
	varray_t*new = (varray_t*) malloc(sizeof(varray_t));
	if (!new) return 1;
	new->data = (var_data_t*) malloc(sizeof(var_data_t) * INITIAL_SIZE);
	if (!new->data) {
		free(new);
		return 1;
	}
	// initialising data
	new->size = INITIAL_SIZE;
	new->top = 0;
	for (int i = 0; i < INITIAL_SIZE; i++) {
		new->data[i].type = -1;
	}
	// returning data
	*result = new;
	return 0;
}


void free_varray(varray_t*array) {
	// fprintf(stdout, "array size %d\n", array->size);
	for (int i = 0; i < array->size; i++) {
		if (array->data[i].type == -1) break;
		if (array->data[i].type == 5) free_pair((pair_t*)array->data[i].data);
	}
	free(array->data);
	free(array);
}
void free_varray_code(varray_code_t*array) {
	free(array->code);
	free(array);
}



/** DEBUG **/
void varray_print(varray_t*array){
	for (int i = 0; i < array->top; i++) {
		fprintf(stdout, "{%d[", i);
		intptr_t type = array->data[i].type;
		intptr_t data = array->data[i].data;
		switch (type) {
		case 0: // UNIT
			fprintf(stdout, "<unit>");
			break;
		case 1: // INT
			fprintf(stdout, "%"PRIdPTR"", data);
			break;
		case 2: // PRIM
			fprintf(stdout, "<prim@%s>", PRIMS[data]);
			break;
		case 3: // FUN
			fprintf(stdout, "<label@%"PRIdPTR">", data);
			break;
		case 4: // BOOL
			fprintf(stdout, "#%c", (data?'t':'f'));
			break;
		case 5: // PAIR
			print_pair(stdout, (pair_t*) data);
			break;
		}
		fprintf(stdout, "]} ");
	}
}
