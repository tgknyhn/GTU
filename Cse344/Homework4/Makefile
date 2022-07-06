VALGRIND_FLAGS  = --leak-check=full \--track-origins=yes \--verbose \--show-leak-kinds=all
COMPILE_MAIN    = gcc -ggdb3 -Wall -pthread source/main.c source/file.c -o hw4
RUN_MAIN        = ./hw4 -C 10 -N 5 -F "input.txt"

compile: source/main.c
	$(COMPILE_MAIN)

run:
	$(COMPILE_MAIN) && $(RUN_MAIN)

valgrind:
	$(COMPILE_MAIN) && valgrind $(VALGRIND_FLAGS) $(RUN_MAIN)

clean:
	rm hw4 

.PHONY: compile run valgrind