#include "../header/unnamedsem.h"
#include "../header/file.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

char printBuffer[MAX_PRINT];
int sigintFlag;

pid_t parentPID;
pid_t* pushersPID[PUSHER_COUNT];
pid_t* chefPID[CHEF_COUNT];

char path_to_input_file     [PATH_LEN];
char path_to_sem_milk       [PATH_LEN];
char path_to_sem_flour      [PATH_LEN];
char path_to_sem_walnuts    [PATH_LEN];
char path_to_sem_sugar      [PATH_LEN];
char path_to_sem_pusherLock [PATH_LEN];
char path_to_sem_wholesaler [PATH_LEN];
char path_to_sem_chefs      [CHEF_COUNT][PATH_LEN];

sem_t* sem_milk;
sem_t* sem_flour;
sem_t* sem_walnuts;
sem_t* sem_sugar;
sem_t* sem_pusherLock;
sem_t* sem_wholesaler;
sem_t* sem_chefs[CHEF_COUNT];

int* gullac;
int* milkFlag;
int* flourFlag;
int* walnutsFlag;
int* sugarFlag;
char* ingredients;

int main(int argc, char* argv[]) {
    ctrlcla(argc, argv);
    startSignalHandler();
    putVariablesIntoShrdMem();
    putSemaphoresIntoShrdMem();
    createPushers(); if(sigintFlag) return 0;
    createChefs();   if(sigintFlag) return 0;
    wholesaler();
    termChildrenProcesses();
    closeSemaphores();
    printWholesalerDialog3(getpid());

    return 0;
}

void ctrlcla(int argc, char* argv[]) {
	if(argc != 3)
		exitWithError("Wrong argument count. Valid: 16 arguments.\n");
	
    if(strcmp(argv[1], "-i") == 0)
		strcpy(path_to_input_file, argv[2]);
	else
		exitWithError("Invalid CLA. It should be as: \"./hw3named -i inputFilePath -n sem_names\"\n");
    
    parentPID = getpid();
}

void startSignalHandler() {
    sigset_t sigSet;
    // Sigset to block other signals
    sigfillset(&sigSet); sigdelset(&sigSet, SIGINT); sigprocmask(SIG_BLOCK, &sigSet, NULL);
    // Sigaction struct
    struct sigaction sg;
    // Giving it a memory space
    memset(&sg, 0, sizeof(sg));
    // Telling which handler function to use
    sg.sa_handler = &handleSignal;
    // Adding signals to handle
    if(sigaction(SIGINT, &sg, NULL) == -1) {
        write(STDERR_FILENO, "Signal initialization failed. Aborting the program.\n", 54);
        exit(EXIT_FAILURE);
    }
}

void handleSignal(int signal) { sigintFlag = 1; }

void putVariablesIntoShrdMem() {
    // Shared memory preferences
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    gullac      = (int*)mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    milkFlag    = (int*)mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    flourFlag   = (int*)mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    walnutsFlag = (int*)mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    sugarFlag   = (int*)mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    for(int i=0; i<PUSHER_COUNT; i++)
       pushersPID[i] = (pid_t*)mmap(NULL, sizeof(pid_t), protection, visibility, -1, 0);
    for(int i=0; i<CHEF_COUNT; i++)
       chefPID[i] = (pid_t*)mmap(NULL, sizeof(pid_t), protection, visibility, -1, 0);
    ingredients = (char*)mmap(NULL, sizeof(char)*3, protection, visibility, -1, 0);
    ingredients[0] = ' '; ingredients[1] = ' ';
}

void putSemaphoresIntoShrdMem() {
    // Shared memory preferences
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    sem_milk       = (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0);
    sem_flour      = (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0);
    sem_walnuts    = (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0);
    sem_sugar      = (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0);
    sem_pusherLock = (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0);
    sem_wholesaler = (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0);
    for(int i=0; i<CHEF_COUNT; i++)
        sem_chefs[i] = (sem_t*)mmap(0, sizeof(sem_t), protection, visibility, 0, 0);

    sem_init(sem_milk, 1, 0);           
    sem_init(sem_flour, 1, 0);
    sem_init(sem_walnuts, 1, 0);
    sem_init(sem_sugar, 1, 0);
    sem_init(sem_pusherLock, 1, 1);
    sem_init(sem_wholesaler, 1, 0);
    for(int i=0; i<CHEF_COUNT; i++)
        sem_init(sem_chefs[i], 1, 0);
}

void createPushers() {
    pid_t pid;
    pid = fork(); if(pid == 0) { *pushersPID[0] = getpid(); pusherMilk();    closeSemaphores(); return; }
    pid = fork(); if(pid == 0) { *pushersPID[1] = getpid(); pusherFlour();   closeSemaphores(); return; }
    pid = fork(); if(pid == 0) { *pushersPID[2] = getpid(); pusherWalnuts(); closeSemaphores(); return; }
    pid = fork(); if(pid == 0) { *pushersPID[3] = getpid(); pusherSugar();   closeSemaphores(); return; }
}

void createChefs() {
    pid_t pid;
    pid = fork(); if(pid == 0) { *chefPID[0] = getpid(); *gullac += chef0_WS(); closeSemaphores(); return; }
    pid = fork(); if(pid == 0) { *chefPID[1] = getpid(); *gullac += chef1_FW(); closeSemaphores(); return; }
    pid = fork(); if(pid == 0) { *chefPID[2] = getpid(); *gullac += chef2_SF(); closeSemaphores(); return; }
    pid = fork(); if(pid == 0) { *chefPID[3] = getpid(); *gullac += chef3_MF(); closeSemaphores(); return; }
    pid = fork(); if(pid == 0) { *chefPID[4] = getpid(); *gullac += chef4_MW(); closeSemaphores(); return; }
    pid = fork(); if(pid == 0) { *chefPID[5] = getpid(); *gullac += chef5_SM(); closeSemaphores(); return; }
}

void pusherMilk() {
    for(;;) {
        sem_wait(sem_milk);         if(sigintFlag == 1) break;
        sem_wait(sem_pusherLock);   if(sigintFlag == 1) break;
        if(*flourFlag == 1)        { *flourFlag   = 0; sem_post(sem_chefs[3]); }
        else if(*walnutsFlag == 1) { *walnutsFlag = 0; sem_post(sem_chefs[4]); }
        else if(*sugarFlag == 1)   { *sugarFlag   = 0; sem_post(sem_chefs[5]); }
        else                       { *milkFlag = 1; }
        sem_post(sem_pusherLock);
    }
}

void pusherFlour() {
    for(;;) {
        sem_wait(sem_flour);        if(sigintFlag == 1) break;
        sem_wait(sem_pusherLock);   if(sigintFlag == 1) break;
        if(*milkFlag == 1)         { *milkFlag    = 0; sem_post(sem_chefs[3]); }
        else if(*sugarFlag == 1)   { *sugarFlag   = 0; sem_post(sem_chefs[2]); }
        else if(*walnutsFlag == 1) { *walnutsFlag = 0; sem_post(sem_chefs[1]); }
        else                       { *flourFlag = 1; }
        sem_post(sem_pusherLock);
    }    
}

void pusherWalnuts() {
    for(;;) {
        sem_wait(sem_walnuts);      if(sigintFlag == 1) break;
        sem_wait(sem_pusherLock);   if(sigintFlag == 1) break;
        if(*flourFlag == 1)      { *flourFlag = 0; sem_post(sem_chefs[1]); }
        else if(*milkFlag == 1)  { *milkFlag  = 0; sem_post(sem_chefs[4]); }
        else if(*sugarFlag == 1) { *sugarFlag = 0; sem_post(sem_chefs[0]); }
        else               { *walnutsFlag = 1; }
        sem_post(sem_pusherLock);
    }
}

void pusherSugar() {
    for(;;) {
        sem_wait(sem_sugar);        if(sigintFlag == 1) break;
        sem_wait(sem_pusherLock);   if(sigintFlag == 1) break;
        if(*flourFlag == 1)        { *flourFlag   = 0; sem_post(sem_chefs[2]); }
        else if(*walnutsFlag == 1) { *walnutsFlag = 0; sem_post(sem_chefs[0]); }
        else if(*milkFlag == 1)    { *milkFlag    = 0; sem_post(sem_chefs[5]); }
        else                       { *sugarFlag = 1; }
        sem_post(sem_pusherLock);
    }
}

int chef0_WS() {
    char ingredient1[] = "Walnuts"; char ingredient2[] = "Sugar"; int gullacCount = 0;
    printChefDialog1(0, getpid(), ingredient1, ingredient2);
    for(;;) {
        sem_wait(sem_chefs[0]); if(sigintFlag == 1) break;
        printChefDialog2(0, getpid(), ingredient1, ingredient2);
        gullacCount++;
        sem_post(sem_wholesaler);
    }
    printChefDialog3(0, getpid());
    return gullacCount;
}

int chef1_FW() {
    char ingredient1[] = "Flour"; char ingredient2[] = "Walnuts"; int gullacCount = 0;
    printChefDialog1(1, getpid(), ingredient1, ingredient2);
    for(;;) {
        sem_wait(sem_chefs[1]); if(sigintFlag == 1) break;
        printChefDialog2(1, getpid(), ingredient1, ingredient2);
        gullacCount++;
        sem_post(sem_wholesaler);
    }
    printChefDialog3(1, getpid());
    return gullacCount;
}

int chef2_SF() {
    char ingredient1[] = "Sugar"; char ingredient2[] = "Flour"; int gullacCount = 0;  
    printChefDialog1(2, getpid(), ingredient1, ingredient2);
    for(;;) {
        sem_wait(sem_chefs[2]); if(sigintFlag == 1) break;
        printChefDialog2(2, getpid(), ingredient1, ingredient2);
        gullacCount++;
        sem_post(sem_wholesaler);
    }    
    printChefDialog3(2, getpid());
    return gullacCount;
}

int chef3_MF() {
    char ingredient1[] = "Milk"; char ingredient2[] = "Flour"; int gullacCount = 0;
    printChefDialog1(3, getpid(), ingredient1, ingredient2);
    for(;;) {
        sem_wait(sem_chefs[3]); if(sigintFlag == 1) break;
        printChefDialog2(3, getpid(), ingredient1, ingredient2);
        gullacCount++;
        sem_post(sem_wholesaler);
    }
    printChefDialog3(3, getpid());
    return gullacCount;
}

int chef4_MW() {
    char ingredient1[] = "Milk"; char ingredient2[] = "Walnuts"; int gullacCount = 0;
    printChefDialog1(4, getpid(), ingredient1, ingredient2);
    for(;;) {
        sem_wait(sem_chefs[4]); if(sigintFlag == 1) break;
        printChefDialog2(4, getpid(), ingredient1, ingredient2);
        gullacCount++;
        sem_post(sem_wholesaler);
    }    
    printChefDialog3(4, getpid());
    return gullacCount;
}

int chef5_SM() {
    char ingredient1[] = "Sugar"; char ingredient2[] = "Milk"; int gullacCount = 0;
    printChefDialog1(5, getpid(), ingredient1, ingredient2);
    for(;;) {
        sem_wait(sem_chefs[5]); if(sigintFlag == 1) break;
        printChefDialog2(5, getpid(), ingredient1, ingredient2);
        gullacCount++;
        sem_post(sem_wholesaler);
    }
    printChefDialog3(5, getpid());
    return gullacCount;
}

void wholesaler() {
    sleep(1);
    int readFD   = openFile(path_to_input_file, "r");
    int readSize = readFromFileFD(readFD, ingredients, 3);
    int pid = getpid();

    do {
        sellIngredients();
        printWholesalerDialog1(pid, getIngrName(ingredients[0]), getIngrName(ingredients[1]));
        sem_wait(sem_wholesaler);   if(sigintFlag == 1) break;
        printWholesalerDialog2(pid);
        readSize = readFromFileFD(readFD, ingredients, 3);
    } while(readSize == 2 || readSize == 3);
} 

void sellIngredients() {
    if(strchr(ingredients, 'M') != NULL)
        sem_post(sem_milk);
    if(strchr(ingredients, 'F') != NULL)
        sem_post(sem_flour);
    if(strchr(ingredients, 'W') != NULL)
        sem_post(sem_walnuts);
    if(strchr(ingredients, 'S') != NULL)
        sem_post(sem_sugar);
}

void closeSemaphores() {
    sem_close(sem_wholesaler);
    sem_close(sem_milk);
    sem_close(sem_flour);
    sem_close(sem_walnuts);
    sem_close(sem_sugar);
    sem_close(sem_pusherLock);
    for(int i=0; i<CHEF_COUNT; i++)
        sem_close(sem_chefs[i]);
} 

void printChefDialog1(int index, int pid, char* ingr1, char* ingr2) {
    sprintf(printBuffer, "chef%d (pid %d) is waiting for %s and %s (%c%c)\n", 
            index, pid, ingr1, ingr2, ingredients[0], ingredients[1]);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printChefDialog2(int index, int pid, char* ingr1, char* ingr2) {
    sprintf(printBuffer, "chef%d (pid %d) has taken the %s (%c%c)\n", 
        index, pid, ingr1, ingredients[0], ingredients[1]);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));

    char* temp = strchr(ingredients, ingr1[0]); temp[0] = ' ';

    sprintf(printBuffer, "chef%d (pid %d) has taken the %s (%c%c)\n", 
        index, pid, ingr2, ingredients[0], ingredients[1]);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
    sprintf(printBuffer, "chef%d (pid %d) is preparing the dessert (  )\n", index, pid);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
    sprintf(printBuffer, "chef%d (pid %d) has delivered the dessert (  )\n", index, pid);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printChefDialog3(int index, int pid) {
    sprintf(printBuffer, "chef%d (pid %d) is exiting (  )\n", index, pid);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printWholesalerDialog1(int pid, char* ingr1, char* ingr2) {
    sprintf(printBuffer, "the wholesaler (pid %d) delivers %s and %s (%c%c)\n", 
            pid, ingr1, ingr2, ingredients[0], ingredients[1]);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
    sprintf(printBuffer, "the wholesaler (pid %d) is waiting for the dessert (%c%c)\n", 
            pid, ingredients[0], ingredients[1]);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printWholesalerDialog2(int pid) {
    sprintf(printBuffer, "the wholesaler (pid %d) has obtained the dessert and left (  )\n", pid);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

void printWholesalerDialog3(int pid) {
    sprintf(printBuffer, "the wholesaler (pid %d) is done (total desserts: %d) (  )\n", pid, *gullac);
    write(STDOUT_FILENO, printBuffer, strlen(printBuffer));
}

char* getIngrName(char letter) {
    switch(letter) {
        case 'M':
            return "Milk";
        case 'W':
            return "Walnuts";
        case 'S':
            return "Sugar";
        case 'F':
            return "Flour";
    }
    return NULL;
}

void termChildrenProcesses() {
    for(int i=0; i<CHEF_COUNT; i++)
        kill(*chefPID[i], SIGINT);
    for(int i=0; i<PUSHER_COUNT; i++)
        kill(*pushersPID[i], SIGINT);
    for(int i=0; i<CHEF_COUNT+PUSHER_COUNT; i++)
        wait(NULL);
}

/* Finish */
