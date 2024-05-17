/* ProcessPage.h: structs to represent process pages of virtual memory */

#ifndef _PROCESSPAGE_H
#define _PROCESSPAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#define PAGE_SIZE 4
#define NOT_MAPPED -1
#define MIN_PAGES_NEEDED 4

// Page_t for a single page of virtual memory
typedef struct Page {
    int allocatedToFrame;
    //int pageNumber;
    struct Page *nextPage;
    int frameNumber;
    // struct Frame_t *allocatedFrame;
} Page_t;

// processPages_t represents all the pages created for process. Pages represented as a linked list
typedef struct processPages {
    int totalNumberPages;
    int numberPagesAllocated;
    Page_t *head;
    // struct Process *process;

} processPages_t;

// Check if pages allocated in memoryFrames, return number of pages allocated. If no pages, return 0.
int checkIfAllocated(processPages_t *processPageHead);
// Create processPages and the linked Pages, given a process memory size
processPages_t* createProcessPages(int processMemorySize);
Page_t* createSinglePage();
int totalPagesInProcess(int processSize);
void freeProcessPages(processPages_t *processPages);
void printFramesUsed(processPages_t *processPages);
int numPagesRequiredVMA(processPages_t *processPages);

#endif //_PROCESSPAGE_H