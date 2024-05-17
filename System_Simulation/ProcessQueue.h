/** ProcessQueue.h: Structs that represents the processes during processes scheduling. These help to represent the Process Manager in a system. */
#ifndef _PROCESSQUEUE_H
#define _PROCESSQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#include "MemoryList.h"
#include "ProcessPage.h"
#include "MemoryFrame.h"

#define MAX_NAME_LEN 8

typedef enum State {
    READY,
    RUNNING,
    FINISHED
} State;

typedef struct Process {
    char name[MAX_NAME_LEN];
    int arriveTime;
    int runningTime;
    int serviceTime; //runtime left
    int memory; //memory size of process
    processPages_t *processPages;
    LinkedList* memoryBlock;
    State state;
} Process;

typedef struct ProcessQueue {
    Process* process;
    struct ProcessQueue* next;
} ProcessQueue;

// put the process at the end of the queue
void enQueue(ProcessQueue** queue, Process* process);
ProcessQueue* createProcessQueue(Process* process);
Process* createProcess(char* name, int arriveTime, int runningTime, int memory, State state);


// dequeue the head of the process from the queue
Process* deQueueFirstProcess(ProcessQueue** queue);

// free queues
void freeProcessQueue(ProcessQueue** queue);

// printing info
void printProcessQueue(ProcessQueue** queue);
void printProcess(Process* process);
int processRemaining(ProcessQueue** queue);

// statistical performance
int calcTurnAroundTime(int* arr, int length);

#endif //_PROCESSQUEUE_H