COMPILE_CLIENT   = gcc -ggdb3 -Wall source/clientX.c source/file.c source/matrix.c -o client -lm -lpthread 
COMPILE_SERVER_Y = gcc -ggdb3 -Wall source/serverY.c source/file.c source/matrix.c source/daemon.c -o serverY -lm -lpthread 
VALGRIND_FLAGS = --leak-check=full \--track-origins=yes \--verbose \--leak-check=full \--show-leak-kinds=all
RUN_CLIENT = ./client -s "/tmp/ServerFIFO" -o "data.csv"
RUN_SERVER_Y = ./serverY -s "/tmp/ServerFIFO" -o "result.log" -p 2 -r 3 -t 5

compile: source/clientX.c source/serverY.c
	$(COMPILE_CLIENT) && $(COMPILE_SERVER_Y)

run_client:
	$(COMPILE_CLIENT) && $(COMPILE_SERVER_Y) && $(RUN_CLIENT)

run_server_y:
	$(COMPILE_CLIENT) && $(COMPILE_SERVER_Y) && $(RUN_SERVER_Y)

client_valgrind:
	$(COMPILE_CLIENT) && $(COMPILE_SERVER_Y) && valgrind $(VALGRIND_FLAGS) $(RUN_CLIENT)

server_valgrind:
	$(COMPILE_CLIENT) && $(COMPILE_SERVER_Y) && valgrind $(VALGRIND_FLAGS) $(RUN_SERVER_Y)
