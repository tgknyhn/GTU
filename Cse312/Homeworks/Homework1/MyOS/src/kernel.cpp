
#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <syscalls.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <drivers/ata.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>

#include <drivers/amd_am79c973.h>
#include <net/etherframe.h>
#include <net/arp.h>
#include <net/ipv4.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/tcp.h>


// #define GRAPHICSMODE


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;
using namespace myos::net;

// Producer Task
void producerWithoutRC();
void producerWithRC();
// Consumer Task
void consumerWithoutRC();
void consumerWithRC();
// Temp X
void taskX();

void JoinTaskKernel();

GlobalDescriptorTable gdt;
TaskManager taskManager;
InterruptManager interrupts(0x20, &gdt, &taskManager);

Task taskPRC  (&gdt, producerWithRC); 
Task taskCRC  (&gdt, consumerWithRC);
Task taskPWRC (&gdt, producerWithoutRC); 
Task taskCWRC (&gdt, consumerWithoutRC); 
Task taskTemporary(&gdt, taskX);


void printf(char* str)
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

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}
void printfHex16(uint16_t key)
{
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}
void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}
void sysprintf(char* str)
{
    asm("int $0x80" : : "a" (4), "b" (str));
}


/* My Functions - Start */

int turn = 0;
bool interested[] = {false, false};

void enter_region(int process) {
    // ID of the other process
    int other = 1 - process;
    // Assigning it as interested
    interested[process] = true;
    // Saying it is this process' turn
    turn = process;
    // Staying in this loop until our conditions met
    while (turn == process && interested[other] == true) /* sleeping until condition brokes */ ;
}

void leave_region(int process) {
    interested[process] = false;
}

void myNop(int time) {
    for(int i=0; i<time; i++) 
        for(int j=0; j<time; j++) 
            asm("nop");
}


// Producer - Consumer Functions with Race Condition

#define N_RC 50
int indexPRC = 0;
int indexCRC = 0;
int count_rc = 0;
int buffer_rc[N_RC];
bool producerFlag_rc = false;
bool consumerFlag_rc = false;

void producerWithRC() {
    // Declaring the item
    int item = 0;
    char str[10];
    // Infinite loop
    while(true) {
        // Generating new value of the item
        item++; if(item > 1000) item = 0;
        // Going to sleep if buffer is full
        //while(count_rc == N_RC) myNop(100000) ;
        if(count_rc >= N_RC) {
            producerFlag_rc = true;
            while(producerFlag_rc) {
                printf("Inside producer while loop\n");
                myNop(10000);
                producerFlag_rc = true;
            }
        }
        // Putting item to the buffer
        buffer_rc[indexPRC] = item; 
        indexPRC = (indexPRC+1) % N_RC;
        count_rc++;
        // Assigning false to the consumer flag since it is not empty anymore
        consumerFlag_rc = false;
        printf("Producer inserted item : ");
        printf(taskManager.myItoa(item, str));
        printf("\n");

        // Leaving the critical region
        myNop(8000);
    }
}

void consumerWithRC() {
    // Declaring the item and string buffer 
    int item; 
    char str[10];
    // Infinite loop
    while(true) {
        // First checking if buffer array is full or not
        //while(count_rc == 0) myNop(100000) ;
        if(count_rc <= 0) {
            consumerFlag_rc = true;
            while(consumerFlag_rc) {
                myNop(10000);
                printf("Inside consumer while loop\n");
                consumerFlag_rc = true;
            }
        }

        // Getting item from the buffer
        item = buffer_rc[indexCRC];
        indexCRC = (indexCRC+1) % N_RC;
        count_rc--;
        // Assigning false to the producer flag since buffer is not full anymore
        producerFlag_rc = false;
        // Printing result
        printf("ConsumerRC received item : ");
        printf(taskManager.myItoa(item, str));
        printf("\n");
        
        // Leaving the critical region
        myNop(6000);
    }
}

// Producer - Consumer Functions without Race Condition


#define N_WRC 50
int indexWPRC = 0;
int indexWCRC = 0;
int count_wrc = 0;
int buffer_wrc[N_RC];
bool producerFlag_wrc = false;
bool consumerFlag_wrc = false;


void producerWithoutRC() {
   // Declaring the item
    int item = 0;
    char str[10];
    // Infinite loop
    while(true) {
        // Entering to the critical region
        enter_region(0);
        // Generating new value of the item
        item++; if(item > 1000) item = 0;
        // Going to sleep if buffer is full
        //while(count_rc == N_RC) myNop(100000) ;
        if(count_wrc >= N_RC) {
            producerFlag_wrc = true;
            while(producerFlag_wrc) {
                printf("Inside producer while loop\n");
                myNop(10000);
                producerFlag_wrc = true;
            }
        }
        // Putting item to the buffer
        buffer_wrc[indexWPRC] = item; 
        indexWPRC = (indexWPRC+1) % N_WRC;
        count_wrc++;
        // Assigning false to the consumer flag since it is not empty anymore
        consumerFlag_wrc = false;
        printf("Producer inserted item : ");
        printf(taskManager.myItoa(item, str));
        printf("\n");

        // Leaving the critical region
        myNop(8000);
        leave_region(0);
    }
}

void consumerWithoutRC() {
    // Declaring the item and string buffer 
    int item; 
    char str[10];
    // Infinite loop
    while(true) {
        // Entering to the critical region
        enter_region(1);
        // First checking if buffer array is full or not
        //while(count_rc == 0) myNop(100000) ;
        if(count_wrc <= 0) {
            consumerFlag_wrc = true;
            while(consumerFlag_wrc) {
                myNop(10000);
                printf("Inside consumer while loop\n");
                consumerFlag_wrc = true;
            }
        }
        // Getting item from the buffer
        item = buffer_wrc[indexWCRC];
        indexWCRC = (indexWCRC+1) % N_WRC;
        count_wrc--;
        // Assigning false to the producer flag since buffer is not full anymore
        producerFlag_wrc = false;
        // Printing result
        printf("ConsumerRC received item : ");
        printf(taskManager.myItoa(item, str));
        printf("\n");
        
        // Leaving the critical region
        myNop(6000);
        leave_region(1);
    }
}

void taskX() {
    while(true) {
        printf("X");
        myNop(5000);
    }
}

void taskEmpty() {
    while(true) sysprintf("");
}

Task join(&gdt, JoinTaskKernel);

void JoinTaskKernel() {
    taskManager.JoinTask(&taskTemporary);
}

/* My Function - End */



class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        switch(c)
        {
            // Creating Tasks
            case 'q':
                taskManager.AddTask(&taskPRC);
                break;
            case 'w':
                taskManager.AddTask(&taskCRC);
                break;
            case 'e':
                taskManager.AddTask(&taskTemporary);
                break;
            case 'r':
                taskManager.AddTask(&taskPWRC);
                break;
            case 't':
                taskManager.AddTask(&taskCWRC);
                break;

            // Terminating Tasks
            case 'a':
                taskManager.TerminateTask(&taskPRC);
                break;
            case 's':
                taskManager.TerminateTask(&taskCRC);
                break;
            case 'd':
                taskManager.TerminateTask(&taskTemporary);
                break;
            case 'f':
                taskManager.TerminateTask(&taskPWRC);
                break;
            case 'g':
                taskManager.TerminateTask(&taskCWRC);
                break;

            // Joining to a task
            case 'c':
                taskManager.AddTask(&join);
                break;
            // Yielding a task
            case 'm':
                taskManager.YieldTask();
                break;

            default:
                printf("Not assigned a command to this character\n");
                break;
        }
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};

class PrintfUDPHandler : public UserDatagramProtocolHandler
{
public:
    void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket* socket, common::uint8_t* data, common::uint16_t size)
    {
        char* foo = " ";
        for(int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }
    }
};


class PrintfTCPHandler : public TransmissionControlProtocolHandler
{
public:
    bool HandleTransmissionControlProtocolMessage(TransmissionControlProtocolSocket* socket, common::uint8_t* data, common::uint16_t size)
    {
        char* foo = " ";
        for(int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }
        
        
        
        if(size > 9
            && data[0] == 'G'
            && data[1] == 'E'
            && data[2] == 'T'
            && data[3] == ' '
            && data[4] == '/'
            && data[5] == ' '
            && data[6] == 'H'
            && data[7] == 'T'
            && data[8] == 'T'
            && data[9] == 'P'
        )
        {
            socket->Send((uint8_t*)"HTTP/1.1 200 OK\r\nServer: MyOS\r\nContent-Type: text/html\r\n\r\n<html><head><title>My Operating System</title></head><body><b>My Operating System</b> http://www.AlgorithMan.de</body></html>\r\n",184);
            socket->Disconnect();
        }
        
        
        return true;
    }
};






typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    
    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);
    /* 
    printf("heap: 0x");
    printfHex((heap >> 24) & 0xFF);
    printfHex((heap >> 16) & 0xFF);
    printfHex((heap >> 8 ) & 0xFF);
    printfHex((heap      ) & 0xFF);
     */
    void* allocated = memoryManager.malloc(1024);
    
    /* 
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xFF);
    printfHex(((size_t)allocated >> 16) & 0xFF);
    printfHex(((size_t)allocated >> 8 ) & 0xFF);
    printfHex(((size_t)allocated      ) & 0xFF);
    printf("\n");
     */
    
    Task emptyTask(&gdt, taskEmpty); 
    taskManager.AddTask(&emptyTask);

    
    SyscallHandler syscalls(&interrupts, 0x80);
    
    

    printf("Initializing Hardware, Stage 1\n");
    
    #ifdef GRAPHICSMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif
    
    DriverManager drvManager;
    
        #ifdef GRAPHICSMODE
            KeyboardDriver keyboard(&interrupts, &desktop);
        #else
            PrintfKeyboardEventHandler kbhandler;
            KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);
        
    
        #ifdef GRAPHICSMODE
            MouseDriver mouse(&interrupts, &desktop);
        #else
            MouseToConsole mousehandler;
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);
        
        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        #ifdef GRAPHICSMODE
            VideoGraphicsArray vga;
        #endif
        
    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();
        
    printf("Initializing Hardware, Stage 3\n");

    #ifdef GRAPHICSMODE
        vga.SetMode(320,200,8);
        Window win1(&desktop, 10,10,20,20, 0xA8,0x00,0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif


    /*
    printf("\nS-ATA primary master: ");
    AdvancedTechnologyAttachment ata0m(true, 0x1F0);
    ata0m.Identify();
    
    printf("\nS-ATA primary slave: ");
    AdvancedTechnologyAttachment ata0s(false, 0x1F0);
    ata0s.Identify();
    ata0s.Write28(0, (uint8_t*)"http://www.AlgorithMan.de", 25);
    ata0s.Flush();
    ata0s.Read28(0, 25);
    
    printf("\nS-ATA secondary master: ");
    AdvancedTechnologyAttachment ata1m(true, 0x170);
    ata1m.Identify();
    
    printf("\nS-ATA secondary slave: ");
    AdvancedTechnologyAttachment ata1s(false, 0x170);
    ata1s.Identify();
    // third: 0x1E8
    // fourth: 0x168
    */
    

                 

                   
    amd_am79c973* eth0 = (amd_am79c973*)(drvManager.drivers[2]);

    
    // IP Address
    uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;
    uint32_t ip_be = ((uint32_t)ip4 << 24)
                | ((uint32_t)ip3 << 16)
                | ((uint32_t)ip2 << 8)
                | (uint32_t)ip1;
    eth0->SetIPAddress(ip_be);
    EtherFrameProvider etherframe(eth0);
    AddressResolutionProtocol arp(&etherframe);    

    
    // IP Address of the default gateway
    uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 2;
    uint32_t gip_be = ((uint32_t)gip4 << 24)
                   | ((uint32_t)gip3 << 16)
                   | ((uint32_t)gip2 << 8)
                   | (uint32_t)gip1;
    
    uint8_t subnet1 = 255, subnet2 = 255, subnet3 = 255, subnet4 = 0;
    uint32_t subnet_be = ((uint32_t)subnet4 << 24)
                   | ((uint32_t)subnet3 << 16)
                   | ((uint32_t)subnet2 << 8)
                   | (uint32_t)subnet1;
                   
    InternetProtocolProvider ipv4(&etherframe, &arp, gip_be, subnet_be);
    InternetControlMessageProtocol icmp(&ipv4);
    UserDatagramProtocolProvider udp(&ipv4);
    TransmissionControlProtocolProvider tcp(&ipv4);
    
    
    interrupts.Activate();

    printf("\n\n\n\n");
    
    arp.BroadcastMACAddress(gip_be);
    
    
    PrintfTCPHandler tcphandler;
    TransmissionControlProtocolSocket* tcpsocket = tcp.Listen(1234);
    tcp.Bind(tcpsocket, &tcphandler);
    //tcpsocket->Send((uint8_t*)"Hello TCP!", 10);

    
    //icmp.RequestEchoReply(gip_be);
    
    //PrintfUDPHandler udphandler;
    //UserDatagramProtocolSocket* udpsocket = udp.Connect(gip_be, 1234);
    //udp.Bind(udpsocket, &udphandler);
    //udpsocket->Send((uint8_t*)"Hello UDP!", 10);
    
    //UserDatagramProtocolSocket* udpsocket = udp.Listen(1234);
    //udp.Bind(udpsocket, &udphandler);

    
    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}
