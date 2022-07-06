
#include <multitasking.h>

using namespace myos;
using namespace myos::common;



void myNop2(int time) {
    for(int i=0; i<time; i++) 
        for(int j=0; j<time; j++) 
            asm("nop");
}

void printf2(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}


Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));
    
    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;
    
}

Task::~Task()
{
}

        
TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager()
{
}

bool TaskManager::AddTask(Task* task)
{
    if(numTasks >= 256)
        return false;
    
    task->setStackIndex(numTasks);
    tasks[numTasks++] = task;
    return true;
}

bool TaskManager::TerminateTask(Task* task) {
    if(numTasks <= 0)
        return false;

    // Index of the task inside tasks array
    int index = task->getStackIndex();

    // If index+1 is equal to number of tasks than there is no tasks to return 
    // So just change it with the NULL task
    if(index+1 == numTasks)
        tasks[index] = nullptr;
    // If task is not the last task inside tasks array than swap the next tasks by 1 index to the left
    else {
        for(int i=index; i<numTasks; i++) {
            tasks[i] = tasks[i+1]; 
            tasks[i]->setStackIndex(i); // Changing also stack index since we are swapping their locations
        }
        tasks[numTasks] = nullptr;
    }
    // Lastly decrement the task count by 1
    numTasks--;
    // Returning result as true
    return true;
}


bool TaskManager::JoinTask(Task* task) {
    bool isThere = true;

    while(isThere){
        printf2("Not allowing program to terminate until given task terminates.\n");
        isThere = false;
        for(int i=1; i < numTasks; i++)
            if(tasks[i] == task)
                isThere = true;
        myNop2(10000);
    }
    printf2("Program Terminated. Abording the program.\n");
}

void TaskManager::YieldTask() {
    if(numTasks == 1) 
        return;
    
    int n = 12000;

    myNop2(n);
    /* 
    for(int i=currentTask; i<numTasks-1; i++)
        swapTask(i, i+1);
    */
}

void TaskManager::swapTask(int index1, int index2) {
    common::uint32_t tempBuffer[13];

    // First swapping cpu states

    // Storing tasks 1 cpu states into a temporary buffer
    tempBuffer[0]  = tasks[index1]->cpustate->cs; 
    tempBuffer[1]  = tasks[index1]->cpustate->eax; 
    tempBuffer[2]  = tasks[index1]->cpustate->ebp; 
    tempBuffer[3]  = tasks[index1]->cpustate->ebx; 
    tempBuffer[4]  = tasks[index1]->cpustate->ecx; 
    tempBuffer[5]  = tasks[index1]->cpustate->edi; 
    tempBuffer[6]  = tasks[index1]->cpustate->edx; 
    tempBuffer[7]  = tasks[index1]->cpustate->eflags; 
    tempBuffer[8]  = tasks[index1]->cpustate->eip; 
    tempBuffer[9]  = tasks[index1]->cpustate->error; 
    tempBuffer[10] = tasks[index1]->cpustate->esi; 
    tempBuffer[11] = tasks[index1]->cpustate->esp; 
    tempBuffer[12] = tasks[index1]->cpustate->ss; 

    // Then assigning task 2 cpu states into task 1 
    tasks[index1]->cpustate->cs     = tasks[index2]->cpustate->cs; 
    tasks[index1]->cpustate->eax    = tasks[index2]->cpustate->eax; 
    tasks[index1]->cpustate->ebp    = tasks[index2]->cpustate->ebp; 
    tasks[index1]->cpustate->ebx    = tasks[index2]->cpustate->ebx; 
    tasks[index1]->cpustate->ecx    = tasks[index2]->cpustate->ecx; 
    tasks[index1]->cpustate->edi    = tasks[index2]->cpustate->edi; 
    tasks[index1]->cpustate->edx    = tasks[index2]->cpustate->edx; 
    tasks[index1]->cpustate->eflags = tasks[index2]->cpustate->eflags; 
    tasks[index1]->cpustate->eip    = tasks[index2]->cpustate->eip; 
    tasks[index1]->cpustate->error  = tasks[index2]->cpustate->error; 
    tasks[index1]->cpustate->esi    = tasks[index2]->cpustate->esi; 
    tasks[index1]->cpustate->esp    = tasks[index2]->cpustate->esp; 
    tasks[index1]->cpustate->ss     = tasks[index2]->cpustate->ss; 
    
    // Finally assigning temporary states into task 2
    tasks[index2]->cpustate->cs     = tempBuffer[0]; 
    tasks[index2]->cpustate->eax    = tempBuffer[1];
    tasks[index2]->cpustate->ebp    = tempBuffer[2];
    tasks[index2]->cpustate->ebx    = tempBuffer[3];
    tasks[index2]->cpustate->ecx    = tempBuffer[4];
    tasks[index2]->cpustate->edi    = tempBuffer[5]; 
    tasks[index2]->cpustate->edx    = tempBuffer[6]; 
    tasks[index2]->cpustate->eflags = tempBuffer[7]; 
    tasks[index2]->cpustate->eip    = tempBuffer[8]; 
    tasks[index2]->cpustate->error  = tempBuffer[9]; 
    tasks[index2]->cpustate->esi    = tempBuffer[10];
    tasks[index2]->cpustate->esp    = tempBuffer[11];
    tasks[index2]->cpustate->ss     = tempBuffer[12];

    // After swapping cpu states, it is time to swap stack and stack index
    common::uint8_t * bufferStack;
    int bufferStackIndex;

    // First swapping stack
    bufferStack = tasks[index1]->stack;
    *tasks[index1]->stack = *tasks[index2]->stack;
    *tasks[index2]->stack = *bufferStack;

    // Then swapping the index
    bufferStackIndex = tasks[index1]->stackIndex;
    tasks[index1]->stackIndex = tasks[index2]->stackIndex;
    tasks[index2]->stackIndex = bufferStackIndex;
} 


CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if(numTasks <= 0)
        return cpustate;
    
    if(currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;
    
    if(++currentTask >= numTasks)
        currentTask %= numTasks;
    return tasks[currentTask]->cpustate;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        char temp = *(str+start); *(str+start) = *(str+end); *(str+end) = temp; // Swap
        start++;
        end--;
    }
}
 
char* TaskManager::myItoa(int num, char* str)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    if (num < 0) {
        isNegative = true;
        num = -num;
    }
 
    while (num != 0)
    {
        int rem = num % 10;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/10;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}