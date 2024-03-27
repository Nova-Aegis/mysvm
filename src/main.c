#include <stdio.h>
#include <string.h>
#include "vm.h"

#define FLAG_HELP "-h"
#define FLAG_HELP_FULL "--help"
#define FLAG_OUTPUT "-o"
#define FLAG_DEBUG "-d"

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
	fprintf(stdout,"----\n");
}


	
int main(int argc, char**argv){
	if (argc < 2) {
		help();
		return 0;
	}
	int i;
	char*output = NULL;
	int debug = 0;
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
		fprintf(stdout, "starting vm with output=%s debug=%d :\nreading %s\n",(output?output:"NULL"), debug, argv[i]);
		vm_start(output, argv[i], debug);
		return 0;
	loop:
	}
	fprintf(stdout, "program file is missing\nexiting . . .\n");
	return 0;
}
