COMPILE = gcc -ggdb3 -Wall source/main.c source/file.c -o hw5 -lpthread -lm
RUN = ./hw5 -i "input1.txt" -j "input2.txt" -o "output.csv" -n 3 -m 6

VALGRIND_FLAGS = --leak-check=full \--track-origins=yes \--verbose \--leak-check=full \--show-leak-kinds=all

compile: source/main.c source/file.c
	$(COMPILE)

run: 
	$(COMPILE) && $(RUN)

valgrind:
	$(COMPILE) && valgrind $(VALGRIND_FLAGS) $(RUN)

clean:
	rm hw5 *.csv