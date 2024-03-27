# mysvm

mini-Scheme Virtual Machine

## Version : 0.1

## Author

- Guillaume Regnault

## Language

- C (VM)
- Scheme (compiler)

## Synopsis :

mysvm is an attempt to create a virtual machine for mini-scheme. It is purely for experimental purposes and should not be used as the official svm.
mysvm has a mini-scheme compiler included. This compiler was not developed by the author and is just there to test mysvm. Read the compiler's Licence before continuing. The source files are not included for the compiler.

## Information :

You can test mysvm with your own mini-scheme. Note, not all operations are included in mini-scheme.

## Note :

This version of mysvm does not have a bootstrap. Please use the makefile to execute test programs. 

### mysvm can handle multiple operations such as :

- additions (+)
- subtractions (-)
- multiplications (*)
- pairs / list (cons / list)
- equality (eq?)
- zero testing (zero?)

### This version of mysvm currently does not handle :

- divisions (/) (due to floating numbers)
- pair testing (pair?) 


## Testing and Running

The `Makefile` provides support for compiling and testing tasks.
Execute `make help` for the help

### Compiling :

#### Creating development version

`make dev`

#### Creating and installing final version

`make install`

### Testing :

#### testing with a generic scheme bytecode file

`make test [TEST_FILE=<test_file.bc>] [TEST_OUTPUT=<test_output.txt>]`

#### testing with a specific scheme file

`make compile [TEST_FILE=<schem_file>]`
do not add `.scm` at the end of the `scheme_file`.

#### testing with a generic scheme bytecode file and valgrind

`make debug [TEST_FILE=<test_file.bc>] [DEBUG_FILE=<debug_problem>]`
if the program seg fault a `<DEBUG_FILE>.core.####` will be created