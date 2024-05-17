/* MemoryFrame.h: structs to represent frames of physical memory */

#ifndef _MEMORYFRAME_H
#define _MEMORYFRAME_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ProcessPage.h"

#define FRAME_FREE 1
#define FRAME_ALLOCATED 0
#define MAX_FRAMES 512
#define NO_FRAME_EVICTED -1

// Frame_t represents one frame in memory
typedef struct Frame {
    int frameNumber;
    int isFree;
    struct Frame *nextFrame;
    Page_t *allocatedPage;
} Frame_t;

// memoryFrames_t represents all the existing frames in memory
typedef struct memoryFrames {
    int numberFramesAvailable;
    Frame_t *firstFrame;
} memoryFrames_t;

// Creates a memoryFrames_t framework and the individual frames (unallocated)
memoryFrames_t *createMemoryFrames();
// Creates a frame (unallocated) that is added to memoryFrames_t
Frame_t *createSingleFrame(int frameNumber);
/* Allocate a page to memory filling the lowest numbered free frames first.
Assumes enought frames for execution. May not match every process to frame.
Fills every frame it is able to. Return number of frames filled. */
int fillFramesWithPages(memoryFrames_t *memoryFrames, processPages_t *processPages);
// Deallocate all pages from frames given a processes pages. Returns number of pages removed
int removeAllPagesFromFrames(memoryFrames_t *memoryFrames, processPages_t *processPages, int *framesRemovedArray, int evictionEventPrinted);
int percentageFramesUsed(memoryFrames_t *memoryFrames);
void printEvictedFrames(int time, int numFramesRemoved, int *evictedFramesArray);
int evictFirstFrame(processPages_t *processPages, memoryFrames_t *memoryFrames);
void freeMemoryFrames(memoryFrames_t *memoryFrames);

#endif //_MEMORYFRAME_H