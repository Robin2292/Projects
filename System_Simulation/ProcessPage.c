/* ProcessPage.c: functions to create processPages and pages for a process */

#include "ProcessPage.h"

int checkIfAllocated(processPages_t *processPageHead) {
    int numPagesAlocated = 0;
    Page_t *currentPage = processPageHead->head;

    while (currentPage != NULL) {
        if (currentPage->allocatedToFrame != 0) {
            numPagesAlocated++;
        }
        currentPage = currentPage->nextPage;
    }

    return numPagesAlocated;
}

processPages_t* createProcessPages(int processMemorySize) {
    int numPages = totalPagesInProcess(processMemorySize);
    processPages_t *newProcessPages = malloc(sizeof(*newProcessPages));
    assert(newProcessPages);

    newProcessPages->totalNumberPages = numPages;
    //num allocated pages are zero at creation, updated when allocated to a frame
    newProcessPages->numberPagesAllocated = 0;

    // create and link numPages amt pages
    newProcessPages->head = createSinglePage();
    Page_t *currentPage = newProcessPages->head;
    for(int i = 1; i < numPages; i++) {
        currentPage->nextPage = createSinglePage();
        currentPage = currentPage->nextPage;
    }
    return newProcessPages;
}

Page_t* createSinglePage() {
    Page_t *page = malloc(sizeof(*page));
    assert(page);
// Set as unallocated when first created
    page->allocatedToFrame = 0;
    page->frameNumber = NOT_MAPPED;
    page->nextPage = NULL;

    return page;
}

int totalPagesInProcess(int processSize) {
    return ceil(processSize * 1.0 / PAGE_SIZE);
}

void freeProcessPages(processPages_t *processPages) {
    // Go through pages in processPages and free them
    // Scroll through all the pages
    Page_t *currentPage = processPages->head;
    while (currentPage != NULL) {
        Page_t *tempPage = currentPage;
        currentPage = tempPage->nextPage;
        // Free currentPage
        free(tempPage);
        processPages->totalNumberPages--;
    }
    // Free processPages
    if(processPages->totalNumberPages == 0) {
        free(processPages);
    } else {
        printf("THERE WAS AN ERROR IN FREEPROCESSPAGES \n THE NUMBER OF totalNumberPages IS %d\n",
               processPages->totalNumberPages);
        exit(EXIT_FAILURE);
    }
}

// Print frames used in pages.
void printFramesUsed(processPages_t *processPages) {
    int numPrinted = 0;
    Page_t *current = processPages->head;
    printf("[");
    while (current != NULL) {
        if (current->allocatedToFrame != 0 && current->frameNumber != NOT_MAPPED) {
            printf("%d", current->frameNumber);
            numPrinted++;
            if (numPrinted < processPages->numberPagesAllocated ) {
                printf(",");
            } else {
                printf("]");
                break;
            }
        }
        current = current->nextPage;

    }
}

// Determine page requirement for the process in VMA (task 4).
// Page requirement is the minimum number of pages the process needs to be allocated into frame memory. The returned
// integer takes into consideration the number of pages already allocated in memory and the size of the process.
int numPagesRequiredVMA(processPages_t *processPages) {
    if (processPages->totalNumberPages < MIN_PAGES_NEEDED) {
        return processPages->totalNumberPages - processPages->numberPagesAllocated;
    }
    if (processPages->numberPagesAllocated >= MIN_PAGES_NEEDED) {
        return 0;
    } else {
        return MIN_PAGES_NEEDED - processPages->numberPagesAllocated;
    }
}