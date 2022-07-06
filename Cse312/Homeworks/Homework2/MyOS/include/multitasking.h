 
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace myos
{
    
    struct CPUState
    {
        /* what we push */
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        /* what processor push */
        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;        
    } __attribute__((packed));
    
    
    class Task
    {
    friend class TaskManager;
    private:
        common::uint8_t stack[4096]; // 4 KiB
        CPUState* cpustate;
        // int to hold task index
        int stackIndex; 
    public:
        // Setter & Getter for stackIndex
        int getStackIndex() { return stackIndex; }
        void setStackIndex(int newIndex) { stackIndex = newIndex; }

        // Constructor & Desctructor
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
    };
    
    
    class TaskManager
    {
    private:
        int numTasks;
        int currentTask;
    public:
        Task* tasks[256];
        TaskManager();
        ~TaskManager();
        
        int getTaskCount()  { return numTasks;    }        
        int getCurrentTask(){ return currentTask; }        


        bool AddTask        (Task* task);
        bool TerminateTask  (Task* task);
        void YieldTask      ();
        bool JoinTask       (Task* task);
        void swapTask       (int index1, int index2);

        char* myItoa(int num, char *str);

        CPUState* Schedule(CPUState* cpustate);
    };
    
    
    
}


#endif