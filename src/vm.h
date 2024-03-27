
#define NB_CALLBACK 128

/**
 * void vm_start(const char*outputFile, const char*codeFile, int debug)
 * arguments :
 * - `outputFile` is the filename of the output (const char* or NULL if none given)
 * - `codeFile` is the filename of the bytecode to execute (mandatory)
 * - `debug` if debug != 0 then asm code will be printed to stdout
 **/
void vm_start(const char*outputFile, const char*codeFile, int debug);
