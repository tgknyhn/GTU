COMPILE_CHILD = gcc -ggdb3 -Wall childProcess.c file.c -o child
COMPILE_PARENT = gcc -ggdb3 -Wall parentProcess.c file.c -o parent -lm
RUN_PARENT = ./parent -i "inputFile.dat" -o "outputFile.dat"
VALGRIND_FLAGS = --leak-check=full \--track-origins=yes \--verbose

compile: parentProcess.c childProcess.c
	$(COMPILE_CHILD) && $(COMPILE_PARENT)
