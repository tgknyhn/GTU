COMPILE_NAMED   = gcc -ggdb3 -Wall -pthread source/namedsem.c source/file.c -o hw3named
COMPILE_UNNAMED = gcc -ggdb3 -Wall -pthread source/unnamedsem.c source/file.c -o hw3unnamed

SEMAPHORES = "milk" "flour" "walnuts" "sugar" "pusherlock" "wholesaler" "ch0" "ch1" "ch2" "ch3" "ch4" "ch5"
RUN_NAMED   = ./hw3named -i "ingredients.txt" -n $(SEMAPHORES)
RUN_UNNAMED = ./hw3unnamed -i "ingredients.txt"

VALGRIND_FLAGS  = --leak-check=full \--track-origins=yes \--verbose \--show-leak-kinds=all

compile: source/namedsem.c source/unnamedsem.c
	$(COMPILE_NAMED) && $(COMPILE_UNNAMED)

named:
	$(COMPILE_NAMED) && $(RUN_NAMED)

unnamed:
	$(COMPILE_UNNAMED) && $(RUN_UNNAMED)

valgrind_named:
	$(COMPILE_NAMED) && valgrind $(VALGRIND_FLAGS) $(RUN_NAMED)

valgrind_unnamed:
	$(COMPILE_UNNAMED) && valgrind $(VALGRIND_FLAGS) $(RUN_UNNAMED)


.PHONY: compile named unnamed valgrind_named valgrind_unnamed