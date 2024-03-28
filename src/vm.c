#ifndef __VARRAY_H__
#include "varray.h"
#define __VARRAY_H_
#endif
#ifndef __VM_H__
#include "vm.h"
#define __VM_H_
#endif
#ifndef __PRIMS_H__
#include "prims.h"
#define __PRIMS_H_
#endif
#ifndef __PAIR_H__
#include "pair.h"
#define __PAIR_H_
#endif
#ifndef __FRAME_H__
#include "frame.h"
#define __FRAME_H_
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define NB_OPER 10
#define NB_TYPE 5
#define NB_PRIM 9




int (*primitive_functions[]) (intptr_t params, varray_t*stack) = {*addition, *subtraction, *multiplication, *division, *equality, *list, *cons, *car, *cdr, *zerop};


/**
 * void vm_start(const char*outputFile, const char*codeFile, int debug)
 * arguments :
 * - `outputFile` is the filename of the output (const char* or NULL if none given)
 * - `codeFile` is the filename of the bytecode to execute (mandatory)
 * - `debug` if debug != 0 then asm code will be printed to stdout
 **/
void vm_start(const char*outputFile, const char*codeFile, int debug) {
	//variable initialisation
	int std = 0;
	FILE*output = stdout;
	if (outputFile) output = fopen(outputFile,"w");
	else std = 1;
	varray_code_t*tabCode = NULL;
	varray_t*globalEnv = NULL;
	varray_t*localEnv = NULL;
	varray_t*stack = NULL;
	varray_frame_t*execution_frame= NULL;
	int level = 0;
	intptr_t currentByte;
	intptr_t type = -1;
	intptr_t data = -1;
	intptr_t pos = -1;
  intptr_t ftype = -1;
	intptr_t fdata = -1;
	intptr_t params = -1;
	
	
	// check output
	if (!output) {
		fprintf(stdout, "output file [%s] could not be opened\ndefaulting to standard output\n", outputFile);
		output = stdout;
		std = 1;
	}
	
	// get code
	if (create_varray_code(codeFile, &tabCode)) {
		goto exit;
	}
	// create global env
	if (create_varray(&globalEnv)) {
		goto exit;
	}
	// create local env
	if (create_varray(&localEnv)) {
		goto exit;
	}
	// create stack
	if (create_varray(&stack)) {
		goto exit;
	}
	// create execution_frame
	if (create_varray_frame(&execution_frame)) {
		goto exit;
	}
	varray_frame_add(execution_frame, 0);
	// execute bytecode
	if (next_code(tabCode,&currentByte)) goto exit;
	while (currentByte != -1) {
		// print debug
		if (debug) {
			fprintf(stdout, "%s ", OPERATIONS[currentByte]);
			fflush(stdout);
		}
		///////////////////////////////sleep(1);
		// test operation
		switch (currentByte) {
			
		case 0: // GALLOC
			// no need for the moment
			//galloc(globalEnv);
			break;
			
		case 1: // PUSH
			// test UNIT
			if (next_code(tabCode,&currentByte)) goto exit;
			if (currentByte == -1) {
				fprintf(stderr, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
				goto exit;
			}
			type = currentByte;
		  if ((type < 0) || (type > NB_TYPE)) {
				fprintf(stderr, "\nTYPE %"PRIdPTR" not recognized at %"PRIdPTR"\n", type, tabCode->pc);
				goto exit;
			}
			// pring debug
			if (debug) {
				fprintf(stdout, "%s ", TYPES[type]);
				fflush(stdout);
			}
			if (type != 0) { // if not unit then 
				// test VALUE
				if (next_code(tabCode,&currentByte)) goto exit;
				if (currentByte == -1) {
					fprintf(stderr, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
					goto exit;
				}
				data = currentByte;
				// print debug
				if (debug) {
					fprintf(stdout, "%"PRIdPTR" ", data);
					fflush(stdout);
				}
				// test PRIM VALUE
				if ((type == 2) && ((data < 0) || (data > NB_PRIM))) {
					fprintf(stderr, "\nPRIM %"PRIdPTR" is not recognized at %"PRIdPTR"\n", data, tabCode->pc);
					goto exit;
				}
			} else {
				data = 0;
			}
			// PUSH TYPE DATA
			if (push(stack, type, data)) goto exit;
			break;
			
		case 2: // GSTORE
			// test pos
			if (next_code(tabCode,&currentByte)) goto exit;
			if (currentByte == -1) {
				fprintf(stdout, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
				goto exit;
			}
			pos = currentByte;
			// print debug
			if (debug) {
				fprintf(stdout, "%"PRIdPTR" ", pos);
				fflush(stdout);
			}
			// pop last gdata
			if (pop(stack, &type, &data)) goto exit;
			if (gstore(globalEnv, pos, type, data)) goto exit;
			break;
			
		case 3: // POP
			// pop last data
			if (pop(stack, &type, &data)) goto exit;
			// if at main
			if (!level) {
				// test type
				switch (type) {
				case 0: // UNIT
					fprintf(output, "\n> <unit>");
					break;
				case 1: // INT
					fprintf(output, "\n> %"PRIdPTR"", data);
					break;
				case 2: // PRIM
					fprintf(output, "\n> <prim@%s>", PRIMS[data]);
					break;
				case 3: // FUN
					fprintf(output, "\n> <label@%"PRIdPTR">", data);
					break;
				case 4: // BOOL
					fprintf(output, "\n> #%c", (data?'t':'f'));
					break;
				case 5: // PAIR
					fprintf(output, "\n> ");
					print_pair(output, (pair_t*) data);
					free_pair((pair_t*) data);
					break;
				}
			} else {
				//free allocated memory of pair
				if (type == 5) {
					free_pair((pair_t*)data);
				}
			}
			break;
			
		case 4: // JUMP
			// test pos
			if (next_code(tabCode,&currentByte)) goto exit;
			if (currentByte == -1) {
				fprintf(stderr, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
				goto exit;
			}
			pos = currentByte;
			// print debug
			if (debug) {
				fprintf(stdout, "%"PRIdPTR" ", pos);
				fflush(stdout);
			}
			// change cp
			if (goto_code(tabCode, pos-1)) goto exit;
			break;
			
		case 5: // GFETCH
			//test pos
		  if (next_code(tabCode,&currentByte)) goto exit;
			if (currentByte == -1) {
				fprintf(stderr, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
				goto exit;
			}
			pos = currentByte;
			// print debug
			if (debug) {
				fprintf(stdout, "%"PRIdPTR" ", pos);
				fflush(stdout);
			}
			// fetch data
			if (gfetch(globalEnv, pos, &type, &data)) goto exit;
			// case pair
			if (type == 5 && copy_pair((pair_t**)&data)) goto exit;
			// push data
			if (push(stack, type, data)) goto exit;
			break;
			
		case 6: // CALL
			// test params
		  if (next_code(tabCode,&currentByte)) goto exit;
			if (currentByte == -1) {
				fprintf(stderr, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
				goto exit;
			}
			params = currentByte;
			// print debug
			if (debug) {
				fprintf(stdout, "%"PRIdPTR" ", params);
				fflush(stdout);
			}
			
			fprintf(stdout,"\n");
			varray_print(localEnv);
			fprintf(stdout,"\n");
			varray_print(stack);
			// pop call
			if (pop(stack, &ftype, &fdata)) goto exit;
			if ((ftype != 2) && (ftype != 3)) {
				fprintf(stderr, "can only call funtion or prim not {[%"PRIdPTR"][%"PRIdPTR"]} at  %"PRIdPTR"\n", ftype, fdata, tabCode->pc-1);
				goto exit;
			}
			if (ftype == 2) { // case primitive
				if (primitive_functions[fdata](params, stack)) goto exit;
			} else { // case call to function
				var_data_t mem[params];
				// push argument to localEnv
				for (int i = 0; i< params; i++) {
					if (pop(stack, &(mem[i].type), &(mem[i].data))) goto exit;
					//fprintf(stdout, "{[%"PRIdPTR"][%"PRIdPTR"]}", mem[i].type, mem[i].data);
				}
				for (int i = params - 1; i >= 0; i--) {
					if (push(localEnv, mem[i].type, mem[i].data)) goto exit;
				}
				// check function call limit
				if (level > NB_CALLBACK) {
					fprintf(stderr, "callback exceeded limit %d\n",NB_CALLBACK);
					goto exit;
				}
				// save current window
				fun_return[level] = tabCode->pc;
				fun_argc[level] = params;
				level++;
				// goto function
				goto_code(tabCode, fdata-1);
			}
			break;
			
		case 7: // RETURN
			// return to previous window
			level--;
			if (level < 0) {
				fprintf(stderr, "too many returns\n");
				goto exit;
			}
			//test return value
			if (pop(stack, &type, &data)) goto exit;
			if (push(stack, type, data)) goto exit;
			if (type == 3) {
				if (lambda_passed) {
					lambda_passed_argc[lambda_passed_level-1] += fun_argc[level];
				} else {
					lambda_passed_argc[lambda_passed_level] = fun_argc[level];
					lambda_passed_level++;
					lambda_passed = 1;
				}
			} else {
				// remove past window local env
				params = fun_argc[level];
				if (lambda_passed_level != 0) {
					lambda_passed_level--;
					params = lambda_passed_argc[lambda_passed_level];
				}
				for (int i = 0; i < params; i++) {
					if (pop(localEnv, &type, &data)) goto exit;
				}
				lambda_passed = 0;
			}
			pos = fun_return[level];
			// goto previous window pc
			if (goto_code(tabCode, pos)) goto exit;
			break;

		case 8: // FETCH
			// test pos
		  if (next_code(tabCode,&currentByte)) goto exit;
			if (currentByte == -1) {
				fprintf(stderr, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
				goto exit;
			}
			pos = currentByte;
			// print debug
			if (debug) {
				fprintf(stdout, "%"PRIdPTR" ", pos);
				fflush(stdout);
			}
			// fetch data
			if (fetch(localEnv, pos, &type, &data)) goto exit;
			//case data is of type pair
			if ((type == 5) && copy_pair((pair_t**)&data)) goto exit;
			// push data
			if (push(stack, type, data)) goto exit;
			break;
			
		case 9: // JFALSE
			// test pos
		  if (next_code(tabCode,&currentByte)) goto exit;
			if (currentByte == -1) {
				fprintf(stderr, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
				goto exit;
			}
			pos = currentByte;
			// print debug
			if (debug) {
				fprintf(stderr, "%"PRIdPTR" ", pos);
				fflush(stdout);
			}
			// pop data
			if (pop(stack, &type, &data)) goto exit;
			// if type if bool and data is #f then goto
			fprintf(stdout, "{JF[%"PRIdPTR"][%"PRIdPTR"]}", type, data);
			if ((type == 4) && (data == 0)) {
				if (goto_code(tabCode, pos-1)) goto exit;
			}
			break;
			
		case 10: // STORE
			// test pos
		  if (next_code(tabCode,&currentByte)) goto exit;
			if (currentByte == -1) {
				fprintf(stderr, "\nend of file at %"PRIdPTR"\n", tabCode->pc);
				goto exit;
			}
			pos = currentByte;
			// print debug
			if (debug) {
				fprintf(stdout, "%"PRIdPTR" ", pos);
				fflush(stdout);
			}
			// pop last gdata
			if (pop(localEnv, &type, &data)) {
				goto exit;
			}
			// store gdata at pos
			if (store(localEnv, pos, type, data)) {
				goto exit;
			}
			break;
		}
		fprintf(stdout, "\n");
		if (next_code(tabCode,&currentByte)) goto exit;
	}
	fprintf(stdout, "Program complete\nexiting . . .\n");

 exit:
	//close output
	if (!std) {
		fprintf(output, "\n");
		fclose(output);
		if (debug) fprintf(stdout, "closed output\n");
	}
	
	// free code
	if (tabCode) {
		free_varray_code(tabCode);
		if (debug) fprintf(stdout, "free tabCode\n");
	}
	// free global env
	if (globalEnv) {
		free_varray(globalEnv);
		if (debug) fprintf(stdout, "free globalEnv\n");
	}
	// free global env
	if (localEnv) {
		free_varray(localEnv);
		if (debug) fprintf(stdout, "free localEnv\n");
	}
	// free stack
	if (stack) {
		free_varray(stack);
		if (debug) fprintf(stdout, "free stack\n");
	}
	// free exectuion_frame
	if (execution_frame) {
		free_varray_frame(execution_frame);
		if (debug) fprintf(stdout, "free execution_frame\n");
	}
	return;
}
