#ifndef __PAIR_H__
#include "pair.h"
#define __PAIR_H__
#endif

 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

const char*OPERATIONS[] = {"GALLOC", "PUSH", "GSTORE", "POP", "JUMP", "GFETCH", "CALL", "RETURN", "FETCH", "JFALSE", "STORE"};
const char*TYPES[] = {"UNIT", "INT", "PRIM", "FUN", "BOOL", "PAIR"};
const char*PRIMS[] = {"ADD", "SUB", "MUL", "DIV", "EQ", "LIST", "CONS", "CAR", "CDR", "ZEROP"};


/**
 * Create a new pair with passed data
 * return 0 on success
 **/
int create_pair(pair_t**result,intptr_t type1, intptr_t data1, intptr_t type2, intptr_t data2) {
	// allocate space
	pair_t*new = (pair_t*) malloc(sizeof(pair_t));
	if (!new) {
		fprintf(stderr, "error [create_pair] allocation failed\n");
		return 1;
	}
	// copy data
	new->data[0] = data1;
	new->data[1] = data2;
	new->type[0] = type1;
	new->type[1] = type2;
	// give pointer
	*result = new;
	return 0;
}

/**
 * free one pair
 * return 0 on success
 **/
int free_pair(pair_t*pair) {
	free(pair);
	return 0;
}

/**
 * free list and all sub list
 * return 0 on success
 **/
int free_pair_cascade(pair_t*pair) {
	// cascade free first pair
	if (pair->type[0] == 5 && pair->data[0]) free_pair_cascade((pair_t*)pair->data[0]);
	// cascade free second pair
	if (pair->type[1] == 5 && pair->data[1]) free_pair_cascade((pair_t*)pair->data[1]);
	// free current
	free(pair);
	return 0;
}

/**
 * copy pair and and sub pairs
 * return 0 on success
 **/
int copy_pair(pair_t**pair) {
	if (*pair == 0) return 0;
	// allocae space
	pair_t*new = (pair_t*) malloc(sizeof(pair_t));
	if (!pair) {
		fprintf(stderr, "error [copy_pair] allocation failed\n");
		return 1;
	}
	// copy data
	new->data[0] = (*pair)->data[0];
	new->type[0] = (*pair)->type[0];
	new->data[1] = (*pair)->data[1];
	new->type[1] = (*pair)->type[1];
	// if sub-pairs present then copy sub-pairs
	if ((new->type[0] == 5) && (copy_pair((pair_t**)&(new->data[0])))) {
		fprintf(stderr, "error [copy_pair] recursive error\n");
		free(new);
		return 1;
	}
	if ((new->type[1] == 5) && (copy_pair((pair_t**)&(new->data[1])))) {
		fprintf(stderr, "error [copy_pair] recursive error\n");
		free(new);
		return 1;
	}
	// send back pointer
	*pair = new;
	return 0;
}

/**
 * gives the first data of a pair
 * return 0 on success
 **/
int pair_car(pair_t*pair, intptr_t*type, intptr_t*data) {
	*type = pair->type[0];
	*data = pair->data[0];
	return 0;
}

/**
 * gives the second data of a pair
 * return 0 on success
 **/
int pair_cdr(pair_t*pair, intptr_t*type, intptr_t*data) {
	*type = pair->type[1];
	*data = pair->data[1];
	return 0;
}

/**
 * print pair as a list
 **/
void print_list(FILE*output, pair_t*pair) {
	if ((intptr_t) pair == 0) return;
	fprintf(output, " ");
	switch (pair->type[0]) {
	case 0: // UNIT
		fprintf(output, "<unit>");
		break;
	case 1: // INT
		fprintf(output, "%"PRIdPTR"", pair->data[0]);
		break;
	case 2: // PRIM
		fprintf(output, "<prim@%s>", PRIMS[pair->data[0]]);
		break;
	case 3: // FUN
		fprintf(output, "<label@%"PRIdPTR">", pair->data[0]);
		break;
	case 4: // BOOL
		fprintf(output, "#%c", (pair->data[0]?'t':'f'));
		break;
	case 5: // PAIR
		if (pair->data[0]) print_pair(output, (pair_t*)pair->data[0]); // non null
		else fprintf(output, "'()");
		break;
	}
	switch (pair->type[1]) {
	case 0: // UNIT
		fprintf(output, " . <unit>");
		break;
	case 1: // INT
		fprintf(output, " . %"PRIdPTR"", pair->data[1]);
		break;
	case 2: // PRIM
		fprintf(output, " . <prim@%s>", PRIMS[pair->data[1]]);
		break;
	case 3: // FUN
		fprintf(output, " . <label@%"PRIdPTR">", pair->data[1]);
		break;
	case 4: // BOOL
		fprintf(output, " . #%c", (pair->data[0]?'t':'f'));
		break;
	case 5: // PAIR
		if (pair->data[0])
			print_list(output, (pair_t*)pair->data[1]); // non null
	}
	return;
}

/**
 * prints pair and cascade
 * return 0 on success
 **/
void print_pair(FILE*output, pair_t*pair) {
	if ((intptr_t) pair == 0) {
		fprintf(output, "(list)");
		return;
	}
	if (pair->type[1] == 5) { // PRINT LIST
		fprintf(output,"(list");
		print_list(output,pair);
		fprintf(output, ")");
	} else {
		fprintf(output, "( ");
		// first data
		switch (pair->type[0]) {
		case 0: // UNIT
			fprintf(output, "<unit>");
			break;
		case 1: // INT
			fprintf(output, "%"PRIdPTR"", pair->data[0]);
			break;
		case 2: // PRIM
			fprintf(output, "<prim@%s>", PRIMS[pair->data[0]]);
			break;
		case 3: // FUN
			fprintf(output, "<label@%"PRIdPTR">", pair->data[0]);
			break;
		case 4: // BOOL
			fprintf(output, "#%c", (pair->data[0]?'t':'f'));
			break;
		case 5: // PAIR
			if (pair->data[0]) print_pair(output, (pair_t*)pair->data[0]); // non null
			else fprintf(output, "'()");
			break;
		}
		fprintf(output, " . ");
		// second data 
		switch (pair->type[1]) {
		case 0: // UNIT
			fprintf(output, "<unit>");
			break;
		case 1: // INT
			fprintf(output, "%"PRIdPTR"", pair->data[1]);
			break;
		case 2: // PRIM
			fprintf(output, "<prim@%s>", PRIMS[pair->data[1]]);
			break;
		case 3: // FUN
			fprintf(output, "<label@%"PRIdPTR">", pair->data[1]);
			break;
		case 4: // BOOL
			fprintf(output, "#%c", (pair->data[1]?'t':'f'));
			break;
		case 5: // PAIR
			if (pair->data[1]) print_pair(output, (pair_t*)pair->data[1]); // non null
			else fprintf(output, "'()");
			break;
		}
		fprintf(output, " )");
	}
	return;
}
