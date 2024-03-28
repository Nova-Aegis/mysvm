#ifndef __VARRAY_H__
#include "varray.h"
#define __VARRAY_H_
#endif
#ifndef __PRIMS_H__
#include "prims.h"
#define __PRIMS_H_
#endif
#ifndef __PAIR_H__
#include "pair.h"
#define __PAIR_H_
#endif


#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

/**
 * Primitive functions :
 * - addition
 * - subtraction
 * - multiplication
 * - division
 * - equality
 * - list
 * - cons
 * - car
 * - cdr
 * - zerop
 **/

int addition(intptr_t params, varray_t*stack) {
	intptr_t res = 0;
	intptr_t data, type;
	intptr_t i = 0;
	//add all 
	for (; i < params; i++) {
		if (pop(stack, &type, &data)) return 1;
		if (type != 1) {
			fprintf(stderr, "error [addition] cannot add non-int\n");
			return 1;
		}
		res += data;
	}
	if (push(stack, 1, res)) return 1;
	return 0;
}

int subtraction(intptr_t params, varray_t*stack) {
	// test parameter given
	if (params < 1) {
		fprintf(stderr, "error [subtraction] arrity missmatch (1 argument minimum expected 0 given)\n");
		return 1;
	}
	intptr_t res = 0;
	intptr_t type, data;
	intptr_t i = 0;
	// test if 1 or more parameter given
	if (params > 1) {
		if (pop(stack, &type, &data)) return 1;
		if (type != 1) {
			fprintf(stderr, "error [subtraction] cannot sub non-int\n");
			return 1;
		}
		res = data;
		i = 1;
	}
	// sub all 
	for (; i < params; i++) {
		if (pop(stack, &type, &data)) return 1;
		if (type != 1) {
			fprintf(stderr, "error [subtraction] cannot sub non-int\n");
			return 1;
		}
		res -= data;
	}
	/*
	// adding last number
	if (pop(stack, &type, &data)) return 1;
	if (type != 1) {
		fprintf(stderr, "error [subtraction] cannot sub non-int\n");
		return 1;
	}
	res += data;
	*/
	fprintf(stdout, "-[%"PRIdPTR"]", res);
	// pushing
	if (push(stack, 1, res)) return 1;
	return 0;
}

int multiplication(intptr_t params, varray_t*stack) {
	intptr_t res = 1;
	intptr_t data, type;
	intptr_t i = 0;
	// mult all
	for (; i < params; i++) {
		if (pop(stack, &type, &data)) return 1;
		if (type != 1) {
			fprintf(stderr, "error [multiplication] cannot mult non-int\n");
			return 1;
		}
		res *= data;
	}
	if (push(stack, 1, res)) return 1;
	return 0;
}

int division(intptr_t params, varray_t*stack) {
	fprintf(stderr, "error [prim division] cannot divide due to lack of float handling\n");
	// test parameter given
	if (params < 1) {
		fprintf(stderr, "error [subtraction] arrity missmatch (1 argument minimum expected 0 given)\n");
		return 1;
	}
	intptr_t res = 1;
	intptr_t type, data;
	intptr_t i = 0;
	//test if 1 or more parameter given
	if (params > 1) {
		if (pop(stack, &type, &data)) return 1;
		if (type != 1) {
			fprintf(stderr, "error [division] cannot div non-int\n");
			return 1;
		}
		res = data;
		i = 1;
	}
	//div all 
	for (; i < params; i++) {
		if (pop(stack, &type, &data)) return 1;
		if (type != 1) {
			fprintf(stderr, "error [division] cannot div non-int\n");
			return 1;
		}
		res /= data;
	}
	if (push(stack, 1, res)) return 1;
	return 0;
}

int equality(intptr_t params, varray_t*stack) { // using eq?
	// test parameter number
	if (params != 2) {
		fprintf(stderr, "error [equality] 2 argument needed\n");
		return 1;
	}
	// get 2 data
	intptr_t type1, type2, data1, data2;
	if (pop(stack, &type1, &data1)) return 1;
	if (pop(stack, &type2, &data2)) return 1;
	// push result
	intptr_t bool = ((type1 == type2) && (data1 == data2));
	if (push(stack, 4, bool)) return 1;
	return 0;
}

int list(intptr_t params, varray_t*stack) {
	// test params
	if (params < 0) {
		fprintf(stderr, "negative amount of parameters\n");
		return 1;
	}
	if (params == 0) { // case empty list
		if (push(stack, 5, 0)) return 1; // PUSH empty list
		return 0;
	}
	// set initial value
	intptr_t data2 = 0;
	intptr_t type2 = 5;
	intptr_t data1, type1;
	intptr_t i = 0;
	for (; i < params; i++) {
		// get next item
		if (pop(stack, &type1, &data1)) return 1;
		// create pair
		if (create_pair((pair_t**)&data2, type1, data1, type2, data2)) return 1;
	}
	// push list
	return push(stack, 5, data2);
}

int cons(intptr_t params, varray_t*stack) {
	// test params
	if (params != 2) {
		fprintf(stderr, "error [prim cons] argument mismatch, expected 2 given %"PRIdPTR"\n", params);
		return 1;
	}
	// get values
	intptr_t type1, data1, type2, data2;
	intptr_t res;
	if (pop(stack, &type1, &data1)) return 1;
	if (pop(stack, &type2, &data2)) return 1;
	// create pair
	if (create_pair((pair_t**)&res, type1, data1, type2, data2)) return 1;
	// push pair
	if (push(stack, 5, res)) return 1;
	return 0;
	
}

int car(intptr_t params, varray_t*stack) {
	// test params
	if (params != 1) {
		fprintf(stderr, "error [prim car] argument mismatch, expected 1 given %"PRIdPTR"\n", params);
		return 1;
	}
	// get value
	intptr_t pair, type, data;
	if (pop(stack, &type, &pair)) return 1;
	if (pair_car((pair_t*)pair, &type, &data)) return 1;
	// push result
	if (push(stack, type, data)) return 1;
	return 0;
}

int cdr(intptr_t params, varray_t*stack) {
	// test params
	if (params != 1) {
		fprintf(stderr, "error [prim car] argument mismatch, expected 1 given %"PRIdPTR"\n", params);
		return 1;
	}
	// get value
	intptr_t pair, type, data;
	if (pop(stack, &type, &pair)) return 1;
	if (pair_cdr((pair_t*)pair, &type, &data)) return 1;
	// push result
	if (push(stack, type, data)) return 1;
	return 0;
}

int zerop(intptr_t params, varray_t*stack) {
	if (params != 1) {
		fprintf(stderr, "error [prim zerop] argument mismatch, expected 1 given %"PRIdPTR"\n", params);
		return 1;
	}
	intptr_t type, data;
	if (pop(stack, &type, &data)) return 1;
	if (type != 1 && type != 5) { 
		fprintf(stderr, "error [prim zerop] wrong argument type, expected INT or PAIR given %s\n", TYPES[type]);
		return 1;
	}
	fprintf(stdout, "{zero?[%"PRIdPTR"][%"PRIdPTR"]", type, data);
	// bool
	if (data == 0) data = 1;
	else data = 0;
	type = 4;
	fprintf(stdout, "[%"PRIdPTR"][%"PRIdPTR"]}", type, data);
	// push
	if (push(stack, type, data)) return 1;
	return 0;
}
