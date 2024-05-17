/* ProcessQueue.c: Process manager functions that allocates processe in round-robin manner */
#include "ProcessQueue.h"

// =============================================  Enqueueing  ===================================================
// put the process at the end of the queue
void enQueue(ProcessQueue** queue, Process* process) {
    if (process == NULL) {
        return;
    }
    ProcessQueue* newProcessQueue = createProcessQueue(process);
    if (*queue == NULL) {
        *queue = newProcessQueue;
    } else {
        ProcessQueue* cur = *queue;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = newProcessQueue;
    }
    // printf("process %s enqueued at time %d\n", process->name, time);
}

// create a new processQueue for a process
ProcessQueue* createProcessQueue(Process* process) {
    ProcessQueue* queue = (ProcessQueue*) malloc(sizeof(ProcessQueue));
    if (queue == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    queue->process = process;
    queue->next = NULL;
    return queue;
}

// create a new process based on the file input
Process* createProcess(char* name, int arriveTime, int runningTime, int memory, State state) {
    Process* process = (Process*) malloc(sizeof(Process));
    if (process == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    strncpy(process->name, name, MAX_NAME_LEN);
    process->name[MAX_NAME_LEN - 1] = '\0';
    process->arriveTime = arriveTime;
    process->runningTime = runningTime;
    process->serviceTime = runningTime;
    process->memory = memory;
    process->state = state;
    process->memoryBlock = NULL;
    process->processPages = NULL;
    return process;
}

// ===============================================  Dequeueing  =====================================================

// dequeue the head of the process queue
Process* deQueueFirstProcess(ProcessQueue** queue) {
    if (*queue == NULL) {
        return NULL;
    }
    ProcessQueue* head = *queue;
    *queue = head->next;
    Process* process = head->process;
    free(head);
    return process;
}

// ============================================  Printing and Free  ===================================================
// free what is in the process queue
void freeProcessQueue(ProcessQueue** queue) {
    while (*queue != NULL) {
        ProcessQueue* current = *queue;
        free(current->process); // Free the Process object
        *queue = current->next; // Move to next queue node
        free(current); // Free the current node
    }
}

// printing processes
void printProcess(Process* process) {
    printf("%s\t%d\t%d\t%d\t%d\n", process->name, process->arriveTime, process->runningTime, process->memory, process->state);
}

int processRemaining(ProcessQueue** queue) {
    ProcessQueue* cur = *queue;
    int num = 0;
    while (cur != NULL) {
        cur = cur->next;
        num++;
    }
    return num;
}

// ===============================================  Calculation  ======================================================
int calcTurnAroundTime(int* arr, int length) {
    float sum = 0.0;
    for (int i = 0; i < length; i++)
    {
        sum += arr[i];
    }
    return (int) ceil(sum / length);
}

