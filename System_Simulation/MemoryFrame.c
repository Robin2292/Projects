/* MemoryFrame.c: functions that help represent Framed Memory */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MemoryFrame.h"

memoryFrames_t *createMemoryFrames() {
    // create the memoryFrames_t struct
    memoryFrames_t *newFramework = malloc(sizeof(*newFramework));
    assert(newFramework);

    newFramework->numberFramesAvailable = MAX_FRAMES;

    // populate with Frame_t frames of size FRAME_SIZE up to MEMORY_SIZE
    newFramework->firstFrame = createSingleFrame(0);
    Frame_t *currentFrame = newFramework->firstFrame;

    for (int i = 1; i < MAX_FRAMES; i++) {
        currentFrame->nextFrame = createSingleFrame(i);
        currentFrame = currentFrame->nextFrame;
    }
    return newFramework;
}

// adds a new frame to framework 
Frame_t *createSingleFrame(int frameNumber) {
    Frame_t *frame = malloc(sizeof(*frame));
    assert(frame);

    frame->frameNumber = frameNumber;
    frame->isFree = FRAME_FREE;
    frame->nextFrame = NULL;
    frame->allocatedPage = NULL;

    return frame;
}

int fillFramesWithPages(memoryFrames_t *memoryFrames, processPages_t *processPages) {
    int numFramesFilled = 0;
    Frame_t *currentFrame = memoryFrames->firstFrame;
    Page_t *currentPage = processPages->head;
    // Scroll through memoryFrames to find the free ones, starting with lower id, until either available frames are
    // filled
    // or there are no more pages to fill
    while (memoryFrames->numberFramesAvailable > 0 && currentPage != NULL) {
        // Go through the frames looking for free ones
        while (currentFrame != NULL && currentFrame->isFree == FRAME_ALLOCATED  ) {
            currentFrame = currentFrame->nextFrame;
        }
        // Found free frame
        currentFrame->allocatedPage = currentPage;
        currentFrame->isFree = FRAME_ALLOCATED;

        processPages->numberPagesAllocated++;
        currentPage->allocatedToFrame = 1;
        currentPage->frameNumber = currentFrame->frameNumber;
        currentPage = currentPage->nextPage;

        // Update numberFramesAvailable in memoryFrames
        memoryFrames->numberFramesAvailable--;
        // Update numbFramesFilled
        numFramesFilled++;
    }
    return numFramesFilled;
}

int removeAllPagesFromFrames(memoryFrames_t *memoryFrames, processPages_t *processPages, int *framesRemovedArray,
                             int evictionEventPrinted) {
    int numFramesRemoved = 0;
    Frame_t *currentFrame = memoryFrames->firstFrame;
    Page_t *currentPage = processPages->head;

    // Check if the frame has the process
    // Search through frames
    while (processPages->numberPagesAllocated > 0) {

        if (currentFrame->allocatedPage == currentPage) {
            // Deallocate from Frame
            framesRemovedArray[numFramesRemoved] = currentFrame->frameNumber;
            currentFrame->isFree = FRAME_FREE;
            currentFrame->allocatedPage = NULL;
            memoryFrames->numberFramesAvailable++;

            currentPage->allocatedToFrame = 0;
            currentPage->frameNumber = NOT_MAPPED;
            processPages->numberPagesAllocated--;

            numFramesRemoved++;
        }
        if (currentFrame->nextFrame == NULL) {
            currentFrame = memoryFrames->firstFrame;
            if (currentPage->nextPage == NULL) {
                currentPage = processPages->head;
            } else {
                currentPage = currentPage->nextPage;
            }
        } else {
            currentFrame = currentFrame->nextFrame;
        }
    }
    // Delete processPages from process to indicate they are not allocated to memoryFrame
    freeProcessPages(processPages);
    return numFramesRemoved;
}

int percentageFramesUsed(memoryFrames_t *memoryFrames) {
    int framesUsed = MAX_FRAMES - memoryFrames->numberFramesAvailable;
    return ceil(((framesUsed * 1.0) / MAX_FRAMES) * 100);
}


void printEvictedFrames(int time, int numFramesRemoved, int *evictedFramesArray) {
    printf("%d,EVICTED,evicted-frames=[", time);
    for (int i = 0; i < numFramesRemoved; i++) {
        if (i == numFramesRemoved - 1) {
            printf("%d]\n", evictedFramesArray[i]);
        } else {
            printf("%d,", evictedFramesArray[i]);
        }
    }
}

// Evict the first frame of the process encountered, return the frame number evicted
int evictFirstFrame(processPages_t *processPages, memoryFrames_t *memoryFrames) {
    int frameEvicted = NO_FRAME_EVICTED;
    Page_t *currentPage = processPages->head;
    Frame_t *currentFrame = memoryFrames->firstFrame;
    while (currentPage->allocatedToFrame == 0) {
        currentPage = currentPage->nextPage;
    }
    while (currentFrame != NULL) {
        if(currentFrame->allocatedPage == currentPage) {
            currentFrame-> isFree = FRAME_FREE;
            currentFrame->allocatedPage = NULL;
            memoryFrames->numberFramesAvailable++;

            currentPage->allocatedToFrame = 0;
            currentPage->frameNumber = NOT_MAPPED;
            processPages->numberPagesAllocated--;
            return currentFrame->frameNumber;
        }
        currentFrame = currentFrame->nextFrame;
    }
    return frameEvicted;
}

void freeMemoryFrames(memoryFrames_t *memoryFrames){
    //go into first frame_t and start freeing until they are all done
    int numFreed = 0;
    Frame_t *currentFrame = memoryFrames->firstFrame;
    while (numFreed < MAX_FRAMES) {
        //check if there is an allocated page:, if there is its an error
        if (currentFrame->allocatedPage != NULL) {
            printf("\n\n ALL PAGES SHOULD BE FREED BY NOW!\n\n");
            exit(EXIT_FAILURE);
        }
        Frame_t *temp = currentFrame;
        currentFrame = currentFrame->nextFrame;
        // if the next page is done then just free this page and youre done
        if (temp->nextFrame == NULL) {
            free(temp);
            break;
        }
        free(temp);
        numFreed++;

    }
    //Free memoryFrames_t
    free(memoryFrames);
}

