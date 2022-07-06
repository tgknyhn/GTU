# Arguments
VALGRIND_FLAGS = --leak-check=full \--track-origins=yes \--verbose \--show-leak-kinds=all
PORT           = 10015
IP             = 127.0.0.1
# Files
F_SERVANT = ../source/servant.c
F_SERVER  = ../source/server.c
F_CLIENT  = ../source/client.c
F_MYTIME  = ../source/utilities/mytime.c
F_FILE    = ../source/utilities/file.c
F_QUEUE   = ../source/data_structures/queue.c
# Executables
EXE_SERVANT = ../executables/servant
EXE_SERVER  = ../executables/server
EXE_CLIENT  = ../executables/client
# Compile
GCC = gcc -ggdb3 -Wall -pthread
COMPILE_SERVANT = $(GCC) $(F_SERVANT) $(F_FILE) $(F_MYTIME) -o $(EXE_SERVANT)
COMPILE_SERVER  = $(GCC) $(F_SERVER)  $(F_FILE) $(F_QUEUE) $(F_MYTIME) -o $(EXE_SERVER)
COMPILE_CLIENT  = $(GCC) $(F_CLIENT)  $(F_FILE) $(F_MYTIME) -o $(EXE_CLIENT)
# Run
RUN_SERVANT = ./$(EXE_SERVANT) -d ../dataset -c 1-81 -r $(IP) -p $(PORT)
RUN_SERVER  = ./$(EXE_SERVER)  -p $(PORT) -t 11
RUN_CLIENT  = ./$(EXE_CLIENT)  -r ../requestFile -q $(PORT) -s $(IP) 

compile : $(F_SERVANT) $(F_SERVER) $(F_CLIENT)
	$(COMPILE_SERVANT) && $(COMPILE_SERVER) && $(COMPILE_CLIENT)

servant :
	$(COMPILE_SERVANT) && $(RUN_SERVANT)
server :
	$(COMPILE_SERVER) && $(RUN_SERVER)
client : 
	$(COMPILE_CLIENT) && $(RUN_CLIENT)

valgrind_servant:
	$(COMPILE_SERVANT) && valgrind $(VALGRIND_FLAGS) $(RUN_SERVANT)
valgrind_server:
	$(COMPILE_SERVER) && valgrind $(VALGRIND_FLAGS) $(RUN_SERVER)
valgrind_client:
	$(COMPILE_CLIENT) && valgrind $(VALGRIND_FLAGS) $(RUN_CLIENT)

clean : 
	rm $(EXE_SERVANT) $(EXE_SERVER) $(EXE_CLIENT)

.PHONY : compile servant server client valgrind_server valgrind_servant valgrind_client