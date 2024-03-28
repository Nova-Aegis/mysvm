#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "vm.h"

#define FLAG_HELP "-h"
#define FLAG_HELP_FULL "--help"
#define FLAG_OUTPUT "-o"
#define FLAG_DEBUG "-d"
#define FLAG_COMPILE "-c"

#define COMPILER "./compiler/scompiler"

void help() {
	fprintf(stdout,"===============================\n");
	fprintf(stdout,"   my Virtual Scheme Machine   \n");
	fprintf(stdout,"===============================\n");
	fprintf(stdout,"Help :\n");
	fprintf(stdout,"----\n");
	fprintf(stdout,"	mysvm [options] <file.bc>\n");
	fprintf(stdout,"\n");
	fprintf(stdout,"Command line options:\n");
	fprintf(stdout,"	-h --help         : this help message\n");
	fprintf(stdout,"	-f <file>         : output results to file\n");
	fprintf(stdout,"	-d --debug        : output debug text (such as ASM during execution)\n");
	fprintf(stdout,"  -c                : compile the input file before launching the vm\n");
	fprintf(stdout,"----\n");
}

void scm_to_bc(char*arg) {
	int i;
	for (i = 0; arg[i] != '\0'; i++);
	arg[i-3] = 'b';
	arg[i-2] = 'c';
	arg[i-1] = '\0';
}

	
int main(int argc, char**argv){
	if (argc < 2) {
		help();
		return 0;
	}
	int i;
	char*output = NULL;
	int debug = 0;
	int compile = 0;
	for (i = 1; i<argc; i++) {
		if (!strcmp(argv[i], FLAG_HELP) || !strcmp(argv[i], FLAG_HELP_FULL)) {
				help();
				return 0;
		}
		if (!strcmp(argv[i], FLAG_OUTPUT)) {
			i++;
			output = argv[i];
			goto loop;
		}
		if (!strcmp(argv[i], FLAG_DEBUG)) {
			debug = 1;
			goto loop;
		}
		if (!strcmp(argv[i], FLAG_COMPILE)) {
			compile = 1;
			goto loop;
		}
		fprintf(stdout, "starting vm with output=%s debug=%d : compile=%d\nreading %s\n",(output?output:"NULL"), debug, compile, argv[i]);
		if (compile) {
			pid_t compiler;
			compiler = fork();
			if (compiler == -1) {
				//fork error
				fprintf(stderr, "fork error, aborting ...\n");
				return 1;
			}
			if (compiler == 0) {
				char*args[] = {COMPILER, argv[i], NULL};
				execv(COMPILER, args);
				//exec error
				fprintf(stdout, "exec error, aborting ...\n");
				return 1;
			} else {
				int res;
				wait(&res);
				if (!WIFEXITED(res) || WEXITSTATUS(res)) {
					// error during compilation
					exit(1);
				}
			  scm_to_bc(argv[i]);
			}
		}	
		vm_start(output, argv[i], debug);
		return 0;
	loop:
	}
	fprintf(stdout, "program file is missing\nexiting . . .\n");
	return 0;
}
