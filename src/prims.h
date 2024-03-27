#ifndef __VARRAY_H__
#include "varray.h"
#define __VARRAY_H_
#endif

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
int addition(intptr_t params, varray_t*stack);
int subtraction(intptr_t params, varray_t*stack);
int multiplication(intptr_t params, varray_t*stack);
int division(intptr_t params, varray_t*stack);
int equality(intptr_t params, varray_t*stack);
int list(intptr_t params, varray_t*stack);
int cons(intptr_t params, varray_t*stack);
int car(intptr_t params, varray_t*stack);
int cdr(intptr_t params, varray_t*stack);
int zerop(intptr_t params, varray_t*stack);
