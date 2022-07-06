BLOCK_SIZE = 8
SYSTEM_DAT = "mySystem.txt"
RUN_FILE   = makeFileSystem

compile: source/part2.c
	gcc source/part2.c -o $(RUN_FILE)

run: 
	gcc source/part2.c -o $(RUN_FILE) && ./$(RUN_FILE) $(BLOCK_SIZE) $(SYSTEM_DAT) 
