#ifndef UNNAMEDSEM_H
#define UNNAMEDSEM_H

#define MAX_PRINT 500
#define PATH_LEN 100

#define CHEF_COUNT 6
#define PUSHER_COUNT 4

#define SEM_PERMS (S_IRWXU | S_IRWXG | S_IRWXO)
#define SEM_FLAG_CE (O_CREAT | O_EXCL)
#define SEM_FLAG_E  (O_EXCL)

void ctrlcla(int argc, char* argv[]);

void startSignalHandler();
void handleSignal(int signal);

void putVariablesIntoShrdMem();
void putSemaphoresIntoShrdMem();

void createPushers();
void createChefs();

void pusherMilk();
void pusherFlour();
void pusherWalnuts();
void pusherSugar();

int chef0_WS();
int chef1_FW();
int chef2_SF();
int chef3_MF();
int chef4_MW();
int chef5_SM();

void wholesaler();
void sellIngredients();

void closeSemaphores();

void printChefDialog1(int index, int pid, char* ingredient1, char* ingredient2);
void printChefDialog2(int index, int pid, char* ingredient1, char* ingredient2);
void printChefDialog3(int index, int pid);

void printWholesalerDialog1(int pid, char* ingr1, char* ingr2);
void printWholesalerDialog2(int pid);
void printWholesalerDialog3(int pid);

char* getIngrName(char letter);

void termChildrenProcesses();

#endif