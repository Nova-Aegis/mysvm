#define __VARRAY_H__
#define INITIAL_SIZE 128

#include <stdint.h>

/*
	Data structure : contains the data with type of data
	type :
	(NOT_ASSIGNED -1)
	(UNIT    0)
	(INT     1)
  (PRIM    2)
  (FUN     3)
  (BOOL    4)
  (PAIR    5)
 */
typedef struct var_data_s{
  intptr_t data;
	intptr_t type;
} var_data_t;

/*
	Data array contains data structure.
	size is the length of the array
	top is the next available slot in the array
 */
typedef struct varray_s{
	var_data_t*data;
	intptr_t size;
	intptr_t top;
} varray_t;


/*
	Code array contains bytecode
*/
typedef struct varray_code_s{
	intptr_t*code;
	intptr_t size;
	intptr_t pc;
} varray_code_t;


/**
 * Get the data structure of position `pos` from the `array`
 * if `pos` is not accessible type = -1 is returned
 **/
int gfetch(varray_t*array, intptr_t pos, intptr_t*type, intptr_t*data);
/**
 * store the data structure in position `pos` in the `array`
 * if `pos` is not acceptable exit is called
 **/
int gstore(varray_t*array, intptr_t pos, intptr_t type, intptr_t data);


/**
 * Get the data structure of position `pos` from the `array`
 * if `pos` is not accessible type = -1 is returned
 **/
int fetch(varray_t*array, intptr_t pos, intptr_t*type, intptr_t*data);
/**
 * store the data structure in position `pos` in the `array`
 * if `pos` is not acceptable exit is called
 **/
int store(varray_t*array, intptr_t pos, intptr_t type, intptr_t data);


/**
 * Get the next code of the code structure and advance
 **/
int next_code(varray_code_t*array, intptr_t*result);
/**
 * Goto the code label
 **/
int goto_code(varray_code_t*array, intptr_t pos);
/**
 * varray_code_t*create_varray_code(int*tab, int size);
 * creates the code array from passed `tab`
 * returns 0 on success
 **/
int create_varray_code(const char*filename,varray_code_t**result);


/**
 * push a `data` of `type` to the `array`
 **/
int push(varray_t*array, intptr_t type, intptr_t data);
/**
 * pops a data_structure from the `array`
 * if `array` is empty then return an error
 * - get data from passed argument pointer `data`
 * - get type from passed argument pointer `type`
 **/
int pop(varray_t*array, intptr_t*type, intptr_t*data);

/**
 * create a varray of INITIAL_SIZE
 * return 0 on success
 **/
int create_varray(varray_t**result);


void varray_free(varray_t*array);
void varray_code_free(varray_code_t*array);


/** DEBUG **/
void varray_print(varray_t*array);
