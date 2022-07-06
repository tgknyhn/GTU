#include <stdio.h>
#define TABLE_SIZE 5


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

// Page list to test with the algorithm
char pages[] = "dgjafljskljdfkgjaklsdkjagfhj";
int  size = sizeof(pages) - 1;
// Disk & Page Table Represantation
char page_table[TABLE_SIZE];
// Other variables
int reference_count[TABLE_SIZE] = {0};  // How many times a page is referenced
int page_count = 0;                     // Current page count on the page table
int first_empty_page = 0;               // Shows index of the first empty place in page table             
// Result variables
int hit          = 0;
int page_fault   = 0;
int pages_loaded = 0;
int disk_written = 0;

int main(int argc, char *argv[])
{
    //bubbleSort();
    //insertionSort();
    //quickSort();
    printResult();
    return 0;
}

void bubbleSort() {
    printArray("Before Bubble Sort : ");
    int temp;
    for(int i=0; i<size; i++) {
        for(int j=0; j<size-(i+1); j++) {
            //lru          (pages[j]);      pages_loaded++;
            //lru          (pages[j+1]);    pages_loaded++;
            //fifo         (pages[j]);      pages_loaded++;
            //fifo         (pages[j+1]);    pages_loaded++;
            secondChance (pages[j]);      pages_loaded++;
            secondChance (pages[j+1]);    pages_loaded++;
            if(pages[j] > pages[j+1]) {
                temp       = pages[j];
                pages[j]   = pages[j+1];
                pages[j+1] = temp;
            }
        }
    }
    printArray("After Bubble Sort  : ");
}

void insertionSort() {
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
}

void quickSort() {
    printArray("Before Quick Sort : ");
    int low = 0;
    int high = size - 1;
    quickSortStart(low, high);
    printArray("After Quick Sort  : ");
}  

void quickSortStart(int low, int high) {
    if(low < high) {
        int pi = partition(low, high);
 
        quickSortStart(low, pi - 1);
        quickSortStart(pi + 1, high);
    }
}  

int partition(int low, int high) {
    lru           (pages[high]);      pages_loaded++;
    //fifo         (pages[high]);      pages_loaded++;
    //secondChance (pages[high]);      pages_loaded++;          
    int pivot = pages[high]; 
    int index = low - 1;     
    int temp;
    for(int i=low; i<=(high-1); i++) {
        // If current element is smaller than the pivot
        lru           (pages[i]);      pages_loaded++;
        //fifo         (pages[i]);      pages_loaded++;
        //secondChance (pages[i]);      pages_loaded++;          
        if(pages[i] < pivot) {
            index++; // increment index of smaller element
            // Swapping between pages[index] <-> pages[i]
            lru           (pages[index]);      pages_loaded++;
            lru           (pages[i]);          pages_loaded++;
            //fifo         (pages[index]);      pages_loaded++;
            //fifo         (pages[i]);          pages_loaded++;
            //secondChance (pages[index]);      pages_loaded++;          
            //secondChance (pages[i]);          pages_loaded++;          
            temp         = pages[index];
            pages[index] = pages[i];
            pages[i]     = temp;
        }
    }
    // Swapping between pages[index+1] <-> pages[high]
    lru           (pages[index+1]);      pages_loaded++;
    lru           (pages[high]);      pages_loaded++;
    //fifo         (pages[index]);      pages_loaded++;
    //fifo         (pages[high]);      pages_loaded++;
    //secondChance (pages[index]);      pages_loaded++;          
    //secondChance (pages[high]);      pages_loaded++; 
    temp           = pages[index+1];
    pages[index+1] = pages[high];
    pages[high]    = temp;
    // Returning the index
    return index + 1;
}

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

void fifo(char page) {}

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

void printArray(char message[]) {
    printf("%s", message);
    for(int i=0; i<size; i++)
        printf("%c", pages[i]);
    printf("\n");
}

void printTable(char page) {
    /* printf("Incoming Page: %c ->  ", page);
    if(first_empty_page != -1) {
        for(int i=0; i<first_empty_page; i++)
            printf("%c  ", page_table[i]);
        for(int i=first_empty_page; i<TABLE_SIZE; i++)
            printf("-  ");
        printf("\n");
    }
    else {
        for(int i=0; i<TABLE_SIZE; i++)
            printf("%c  ", page_table[i]);
        printf("\n");
    } */
}

void printResult() {
    printf("Number of hits : %d\n", hit);
    printf("Number of page faults : %d\n", page_fault);
    printf("Number of pages loaded : %d\n", pages_loaded);
    printf("Number of pages written back to the disk : %d\n", disk_written);
}