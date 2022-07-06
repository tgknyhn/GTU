
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

void taskX();
void myNop(int time);


// Homework 2






void printArray(char array[]);
void printTable(char page);
void printResult();

void bubbleSort();
void insertionSort();
void quickSort();       
void quickSortStart(int low, int high);
int  partition(int low, int high);

void fifo(char page);
void secondChance(char page);
void lru(char page);














GlobalDescriptorTable gdt;
TaskManager taskManager;
InterruptManager interrupts(0x20, &gdt, &taskManager);

void taskEmpty();
Task taskTemporary(&gdt, taskX);

Task bubble(&gdt, bubbleSort);
Task insertion(&gdt, insertionSort);
Task quick(&gdt, quickSort);


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







// Homework 2

#define TABLE_SIZE 2
// Page list to test with the algorithm
char pages[] = "yzx";
int  size = sizeof(pages) - 1;
// Disk & Page Table Represantation
char page_table[TABLE_SIZE];
// Other variables
int first_empty_page = 0;               // Shows index of the first empty place in page table             
// Result variables
int hit          = 0;
int page_fault   = 0;
int pages_loaded = 0;
int disk_written = 0;


/* Sorting Algorithms */

void bubbleSort() {
    printf("Using LRU Page Replacement Algorithm.\n");
    printArray("Before Bubble Sort : ");
    int temp;
    for(int i=0; i<size; i++) {
        for(int j=0; j<size-(i+1); j++) {
            lru          (pages[j]);      pages_loaded++;
            lru          (pages[j+1]);    pages_loaded++;
            //fifo         (pages[j]);      pages_loaded++;
            //fifo         (pages[j+1]);    pages_loaded++;
            //secondChance (pages[j]);      pages_loaded++;
            //secondChance (pages[j+1]);    pages_loaded++;
            if(pages[j] > pages[j+1]) {
                temp       = pages[j];
                pages[j]   = pages[j+1];
                pages[j+1] = temp;
            }
        }
    }
    printArray("After Bubble Sort  : ");
    printResult();
}

void insertionSort() {
    printf("Using LRU Page Replacement Algorithm.\n");
    printArray("Before Insertion Sort : ");
    int page, j;
    for(int i=1; i<size; i++) {
        lru           (pages[i]);      pages_loaded++;
        //fifo         (pages[i]);      pages_loaded++;
        //secondChance (pages[i]);      pages_loaded++;          
        page = pages[i];
        j = i - 1;
        while(page < pages[j] && j >= 0) {
            lru          (pages[j]);      pages_loaded++;
            lru          (pages[j+1]);    pages_loaded++;
            //fifo         (pages[j]);      pages_loaded++;
            //fifo         (pages[j+1]);    pages_loaded++;
            //secondChance (pages[j]);      pages_loaded++;
            //secondChance (pages[j+1]);    pages_loaded++;
            pages[j+1] = pages[j];
            j = j-1;
        }
        lru           (pages[i]);      pages_loaded++;
        //fifo         (pages[i]);      pages_loaded++;
        //secondChance (pages[i]);      pages_loaded++;
        pages[j+1] = page; 
    }
    printArray("After Insertion Sort  : ");
    printResult();
}

void quickSort() {
    printf("Using Second Chance Page Replacement Algorithm.\n");
    //printf("Using Second Chance Page Replacement Algorithm.\n");
    printArray("Before Quick Sort : ");
    int low = 0;
    int high = size - 1;
    quickSortStart(low, high);
    printArray("After Quick Sort  : ");
    printResult();
}  

void quickSortStart(int low, int high) {
    if(low < high) {
        int pivot = partition(low, high);
 
        quickSortStart(low, pivot - 1);
        quickSortStart(pivot + 1, high);
    }
}  

int partition(int low, int high) {
    //lru           (pages[high]);      pages_loaded++;
    //fifo         (pages[high]);      pages_loaded++;
    secondChance (pages[high]);      pages_loaded++;          
    int pivot = pages[high]; 
    int index = low - 1;     
    int temp;
    for(int i=low; i<=(high-1); i++) {
        // If current element is smaller than the pivot
        //lru           (pages[i]);      pages_loaded++;
        //fifo         (pages[i]);      pages_loaded++;
        secondChance (pages[i]);      pages_loaded++;          
        if(pages[i] < pivot) {
            index++; // increment index of smaller element
            // Swapping between pages[index] <-> pages[i]
            //lru           (pages[index]);      pages_loaded++;
            //lru           (pages[i]);          pages_loaded++;
            //fifo         (pages[index]);      pages_loaded++;
            //fifo         (pages[i]);          pages_loaded++;
            secondChance (pages[index]);      pages_loaded++;          
            secondChance (pages[i]);          pages_loaded++;          
            temp         = pages[index];
            pages[index] = pages[i];
            pages[i]     = temp;
        }
    }
    // Swapping between pages[index+1] <-> pages[high]
    //lru           (pages[index+1]);      pages_loaded++;
    //lru           (pages[high]);      pages_loaded++;
    //fifo         (pages[index+1]);      pages_loaded++;
    //fifo         (pages[high]);      pages_loaded++;
    secondChance (pages[index+1]);      pages_loaded++;          
    secondChance (pages[high]);      pages_loaded++; 
    temp           = pages[index+1];
    pages[index+1] = pages[high];
    pages[high]    = temp;
    // Returning the index
    return index + 1;
}



/* Page Replacement Algorithms */

int reference_count[TABLE_SIZE] = {0};  // How many times a page is referenced
void lru(char page) {
    // First loop all the page table to find given page
    for(int i=0; i<TABLE_SIZE; i++) {
        // Increment hit and reference count by one if the page is present and return
        if(page_table[i] == page) {
            hit++;  
            reference_count[i]++;
            printTable(page);
            return;
        }
    }
    // If the page is not present in page_table, look if there is an empty place
    if(first_empty_page != -1) {
        page_table[first_empty_page] = page;    // Insert in the first empty space
        page_fault++;                           // Increment the page fault by 1
        if(first_empty_page == (TABLE_SIZE-1))  // If this is the last empty space at the page table
            first_empty_page = -1;              // Assign first empty page to -1
        else
            first_empty_page++;                 // Else, increment the first_empty_page by 1
    }
    // If there is no empty space in the page table, then find the page with the least reference
    else {
        // Finding index of the least referenced table
        int maxReference = 10000;
        int index = 0;
        for(int i=0; i<TABLE_SIZE; i++) {
            if(reference_count[i] < maxReference) {
                maxReference = reference_count[i];
                index = i;
            }
        }
        // After finding the index of the least referenced page, we replace it with the new page
        page_table[index] = page;
        page_fault++;               // Incrementing the page fault by 1
        disk_written++;
    }
    printTable(page);
}

int nextIndex = 0;
void fifo(char page) {
    printTable(page);
    int index = 0;
    // First loop all the page table to find given page
    for(int i=0; i<TABLE_SIZE; i++) {
        // Increment hit and reference count by one if the page is present and return
        if(page_table[i] == page) {
            hit++;  
            reference_count[i]++;
            printTable(page);
            return;
        }
    } 
    // If the page is not present in page_table, look if there is an empty place
    if(first_empty_page != -1) {
        page_table[first_empty_page] = page;    // Insert in the first empty space
        page_fault++;                           // Increment the page fault by 1
        if(first_empty_page == (TABLE_SIZE-1))  // If this is the last empty space at the page table
            first_empty_page = -1;              // Assign first empty page to -1
        else
            first_empty_page++;                 // Else, increment the first_empty_page by 1
    }
    else {
        index = nextIndex % TABLE_SIZE;
        page_table[index] = page;
        nextIndex++;
        disk_written++;
        page_fault++;               // Incrementing the page fault by 1
    }
    printTable(page);
}

int chance_table[TABLE_SIZE] = {0};
void secondChance(char page) {
    // First try to find page in the page table
    for(int i=0; i<TABLE_SIZE; i++) {
        // Increment hit and reference count by one if the page is present and return
        if(page_table[i] == page) {
            hit++;  
            chance_table[i] = 1;        // Set reference value to 1 to give it a second chance
            printTable(page);
            return;
        }
    }
    // If page not found in the page table, then check if the table is empty or not
    if(first_empty_page != -1) {
        page_table[first_empty_page] = page;     // Insert in the first empty space
        page_fault++;                            // Increment the page fault by 1
        chance_table[first_empty_page] = 0;      // Set page with 0 reference point (yet)
        if(first_empty_page == (TABLE_SIZE-1))   // If this is the last empty space at the page table
            first_empty_page = -1;               // Assign first empty page to -1
        else
            first_empty_page++;                  // Else, increment the first_empty_page by 1
    }
    // Else, if page table is not empty and page is not in the page table, then traverse the page table
    // To find a page with no reference point
    else {  
        for(int i=0; i<TABLE_SIZE*2; i++) {
            if(chance_table[i] == 0) {
                page_table[i] = page;
                printTable(page);
                return;
            }
            i = i % TABLE_SIZE;
            chance_table[i] = 0;
        } 
        page_fault++;
        disk_written++;
    }
    printTable(page);
}




/* Printing Functions */

void printArray(char message[]) {
    printf(message);
    printf(pages);
    printf("\n");
}

void printTable(char page) {
    char printSTR[2]; 
    printSTR[0] = page;
    printSTR[1] = '\0';

    printf("Incoming Page: ");
    printf(printSTR);
    printf(" -> ");

    if(first_empty_page != -1) {
        for(int i=0; i<first_empty_page; i++) {
            printSTR[0] = page_table[i];
            printf(printSTR);
            printf("  ");
        }
        for(int i=first_empty_page; i<TABLE_SIZE; i++)
            printf("-  ");
        printf("\n");
    }
    else {
        for(int i=0; i<TABLE_SIZE; i++) {
            printSTR[0] = page_table[i];
            printf(printSTR);
            printf("  ");
        }
        printf("\n");
    }
}

void printResult() {
    char str[10];
    printf("Number of hits : ");            printf(taskManager.myItoa(hit, str));
    printf("\nNumber of page faults : ");   printf(taskManager.myItoa(page_fault, str));
    printf("\nNumber of pages loaded : ");  printf(taskManager.myItoa(pages_loaded, str));
    printf("\nNumber of pages written back to the disk : "); printf(taskManager.myItoa(disk_written, str));
}

















void myNop(int time) {
    for(int i=0; i<time; i++) 
        for(int j=0; j<time; j++) 
            asm("nop");
}

void taskX() {
    while(true) {
        printf("x\n");
        myNop(5000);
    }
}


void taskEmpty() {
    while(true) sysprintf("");
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
                taskManager.AddTask(&bubble);
                break;
            case 'w':
                taskManager.AddTask(&insertion);
                break;
            case 'e':
                taskManager.AddTask(&quick);
                break;
            case 'r':
                taskManager.AddTask(&taskTemporary);
                break;
            case 'f':
                taskManager.TerminateTask(&taskTemporary);
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
            //printf(foo);
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
            //printf(foo);
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
    
    

    //printf("Initializing Hardware, Stage 1\n");
    
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
        
    //printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();
        
    //printf("Initializing Hardware, Stage 3\n");

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

    //printf("\n\n\n\n");
    
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
