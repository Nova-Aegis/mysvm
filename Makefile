VPATH = ./src:./obj:/.bin:./tests
FLAGS = -Wall -Wextra
CC = gcc

SRC = ./src
OBJ = ./obj
BIN = ./bin
TEST = ./tests

OUTPUT = $(BIN)/mysvm
OBJS = $(OBJ)/varray.o $(OBJ)/main.o $(OBJ)/vm.o $(OBJ)/pair.o $(OBJ)/prims.o

DESTINATION ?= ./mysvm

help :
	@echo "===================="
	@echo "   mysvm Makefile"
	@echo "===================="
	@echo "Help"
	@echo "-----"
	@echo "   make  <target> [parameters]"
	@echo "         dev     : create an developpement version of mysvm"
	@echo "         release : create the final and optimised version of mysvm"
	@echo "         clean   : remove all object file as well as executable"
	@echo "         install : provides the final project at [DESTINATION]"
	@echo "         test    : test dev version with $(TEST)/[TEST_FILE]"
	@echo "         debug   : runs dev version with valgrind and with $(TEST)/[TEST_FILE]"
	@echo "                   valgrind output available at [DEBUG_FILE] $(DEBUG_FILE)"
	@echo "         compile : compiles scheme program $(TEST)/[TEST_FILE]"
	@echo "                   runs dev version with given [TEST_FILE]"
	@echo ""; echo ""
	@echo "Parameters"
	@echo "-----"
	@echo "   [<variable>=<value> ...]"
	@echo "     DESTINATION : Path to create realease version"
	@echo "     TEST_FILE   : file to test for test, debug and compile. DO NOT APPEND FILE TYPE <.scm> <.bc>"
	@echo "     TEST_OUTPUT : file for program output for test and compile. Append file type."




dev : $(OBJS)
	$(CC) $(FLAGS) -g -o $(OUTPUT) $(OBJS)

release : $(OBJS)
	$(CC) $< $(FLAGS) -O3 -g -o $(OUTPUT)


##### CREATE OBJECT #####
OUTPUT_OBJECT = -o $(OBJ)/$@

${OBJ}/varray.o : ${SRC}/varray.c
	$(CC) -c $< $(FLAGS) -o $@

${OBJ}/main.o : ${SRC}/main.c
	$(CC) -c $< $(FLAGS) -o $@

${OBJ}/vm.o : ${SRC}/vm.c
	$(CC) -c $< $(FLAGS) -o $@

${OBJ}/pair.o : ${SRC}/pair.c
	$(CC) -c $< $(FLAGS) -o $@

${OBJ}/prims.o : ${SRC}/prims.c
	$(CC) -c $< $(FLAGS) -o $@

##### TOOLS #####

clean :
	@rm -f $(OBJS) $(OUTPUT)
#	@rm -f ./*~ ./*/*~
	@echo "cleaned environment"

install : release
	@mv $(OUTPUT) $(DESTIONATION)
	@echo "app has been installed at $(DESTINATION)"

TEST_FILE ?= test_file
TEST_OUTPUT ?= test_file.txt

test : dev
	@echo "testing dev with bytecode $(TEST_FILE).bc"
	$(OUTPUT) -d -o $(TEST)/$(TEST_OUTPUT) $(TEST)/$(TEST_FILE).bc
#	$(OUTPUT) -d $(TEST)/$(TEST_FILE).bc
	@cat $(TEST)/$(TEST_OUTPUT)


DEBUG_FILE ?= debug_problem
debug : dev
	@echo "testing debug dev with bytecode $(TEST)/$(TEST_FILE).bc output to ./$(DEBUG_FILE)"
	valgrind --leak-check=full --show-leak-kinds=all -q --log-file=./$(DEBUG_FILE) $(OUTPUT) -d $(TEST)/$(TEST_FILE).bc


SCOMPILER_DIR ?= ./compiler
SCOMPILER ?= scompiler
compile : dev
	$(SCOMPILER_DIR)/$(SCOMPILER) -a $(TEST)/$(TEST_FILE).scm
	$(OUTPUT) -d -o $(TEST)/$(TEST_FILE).txt -c $(TEST)/$(TEST_FILE).scm
	@echo; echo; echo "================================="; echo;
	@cat $(TEST)/$(TEST_FILE).scm
	@echo; echo; echo "================================="; echo;	
	@cat $(TEST)/$(TEST_FILE).txt
