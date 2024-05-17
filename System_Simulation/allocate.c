/* allocate.c: acts as a process scheduler implementing Round Robin scheduling.

To run the program:
./allocate -f <filename> -m (infinite | first-fit | paged | virtual) -q (1 | 2 | 3) 

    All options are required for program to run:
    -f： <filename> specifies a valid relative or absolute path to the input file describing the processes
    -m:  memory-strategy is one of (infinite | first-fit | paged | virtual)
    -q:  quantum where quantum is one of (1 | 2 | 3)
*/

#include "allocate.h"

int time = 0;
// int quantum = 1;
Process* runningProcess = NULL;

// reading from the terminal
char *filename = NULL;
char *memoryStrategy = NULL;
int quantum = 0;

// Set up memory structures
LinkedList *memoryBlock = NULL;
memoryFrames_t *memoryFrames = NULL;


int main(int argc, char **argv)
{
    readFromTerminal(argc, argv);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    ProcessQueue* processQueue = NULL;
    char processName[MAX_NAME_LEN];
    int arriveTime, runningTime, memory;
    int hasNextProcess = 1;

    // Performance statistics
    int numProcesses = 0, finishedProcess = 0;
    int *turnaroundTime = NULL;
    int *serviceTime = NULL;


    // Memory
    // create a memoryBlock of size 2048
    memoryBlock = createNode(1,START,END);
    // create a memoryFrames of size 2048
    memoryFrames = createMemoryFrames();
    //for memory debugging
    Frame_t *lastFrame = memoryFrames->firstFrame;
    while (lastFrame->nextFrame != NULL) {
        //go to the last one
        lastFrame = lastFrame->nextFrame;
    }
    
    // read the first process from the input file
    if (fscanf(file, "%d %s %d %d", &arriveTime, processName, &runningTime, &memory) != 4) {
        hasNextProcess = 0; // error occurs
    }

    // if all the processes have not finished running
    while (hasNextProcess || processQueue != NULL || runningProcess != NULL) {
        // enqueue all the process that have already arrived to processQueue
        while (hasNextProcess && arriveTime <= time) {
            Process* newProcess = createProcess(processName, arriveTime, runningTime, memory, READY);
            enQueue(&processQueue, newProcess);

            // Performance statistics      ===================================================
            int *temp = realloc(turnaroundTime, (numProcesses + 1) * sizeof(int));
            int *temp2 = realloc(serviceTime, (numProcesses + 1) * sizeof(int));
            // error allocating memory
            if (temp == NULL) {
                printf("Memory allocation failed\n");
                free(turnaroundTime);
                return 1; 
            }
            if (temp2 == NULL) {
                printf("Memory allocation failed\n");
                free(serviceTime);
                return 1;
            }
            turnaroundTime = temp;
            serviceTime = temp2;

            turnaroundTime[numProcesses] = -1; // initialise turn around time to -1;
            serviceTime[numProcesses++] = -1;
            if (fscanf(file, "%d %s %d %d", &arriveTime, processName, &runningTime, &memory) != 4) {
                hasNextProcess = 0; // already read all the input file
            }
        }
        // printf("======================================================================================================\n");
        // printProcessQueue(&processQueue);

        // when there are processes need to be executed
        if (processQueue != NULL || runningProcess != NULL) {
            // the current running process has finished running
            if (runningProcess != NULL && runningProcess->runningTime <= 0) {
                runningProcess->state = FINISHED;
                if (strcmp(memoryStrategy, "paged") == 0 || strcmp(memoryStrategy, "virtual") == 0) {
                    int evictedFrames[runningProcess->processPages->numberPagesAllocated];
                    int framesRemoved = removeAllPagesFromFrames(memoryFrames, runningProcess->processPages,
                                                                 evictedFrames, 0);
                    printEvictedFrames(time, framesRemoved, evictedFrames);
                }
                printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", time, runningProcess->name, processRemaining(&processQueue));

                // release the memory block when the process finished
                if (strcmp(memoryStrategy, "first-fit") == 0) {
                    releaseMemory(&memoryBlock, runningProcess->memoryBlock);
                }

                // store the turnaround time && service time of the process
                // turnaroundTime[(runningProcess->name[1]-'0') - 1] = time - runningProcess->arriveTime;
                serviceTime[finishedProcess] = runningProcess->serviceTime;
                turnaroundTime[finishedProcess++] = time - runningProcess->arriveTime;

                Process* temp = runningProcess;
                // free related pages ****
                free(temp);
                // all processes have finished -- terminating
                if (processQueue == NULL) {
                    runningProcess = NULL;
                    if (hasNextProcess == 0) {
                        printf("Turnaround time %d\n", calcTurnAroundTime(turnaroundTime, numProcesses));
                        calcTimeOverhead(turnaroundTime, serviceTime, numProcesses);
                        printf("Makespan %d\n", time);
                    }
                // get the next process to run when the current one has finished
                } else {
                    runNextProcess(&processQueue, memoryStrategy);
                }
            // first process comes in or the current running process is not finished and there are other processes waiting
            } else {
                // there is a running process: 1. processQueue == NULL
                //                             2. processQueue != NULL 
                if (runningProcess != NULL) {
                    if (processQueue != NULL) {
                        // append the current running process to the tail of the process queue
                        runningProcess->state = READY;
                        enQueue(&processQueue, runningProcess);
                        runNextProcess(&processQueue, memoryStrategy);
                    } else {
                        runningProcess->runningTime -= quantum;
                    }
                // there is no current running process -> the first process comes in
                } else {
                    runNextProcess(&processQueue, memoryStrategy);
                }
            }
        } else {
            if(hasNextProcess == 0) {
                break;
            }
        }
        time += quantum;
    }
    
    fclose(file);
    free(turnaroundTime);
    free(serviceTime);
    freeMemory(&memoryBlock);
    freeMemoryFrames(memoryFrames);
    return 0;
}

// run the next process
void runNextProcess(ProcessQueue** processQueue, char* memoryStrategy) {
    // RR with infinite Memory====================================================================================
    if (strcmp(memoryStrategy, "infinite") == 0) {
        runningProcess = deQueueFirstProcess(processQueue);
        runningProcess->state = RUNNING;
        printf("%d,RUNNING,process-name=%s,remaining-time=%d\n", time, runningProcess->name, runningProcess->runningTime);

        // RR with contiguous Memory====================================================================================
    } else if (strcmp(memoryStrategy, "first-fit") == 0) {
        // get the next process in the queue
        Process* process = deQueueFirstProcess(processQueue);
        // the process has already been allocated a memory block
        if (process->memoryBlock != NULL) {
            process->state = RUNNING;
            runningProcess = process;
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, runningProcess->name, runningProcess->runningTime, percentageUsed(&memoryBlock), process->memoryBlock->start);

            // try to allocate memory block to the process
        } else {
            LinkedList* hole = firstFit(&memoryBlock, process->memory);
            // there is no memory left for the process
            while (hole == NULL) {
                // enqueue the process to the tail of the queue
                enQueue(processQueue, process);
                // get another process
                process = deQueueFirstProcess(processQueue);

                // find a process that has not been allocated to a memory
                if (process->memoryBlock == NULL) {
                    hole = firstFit(&memoryBlock, process->memory);
                    continue;
                    // reach the end of the queue and find process that has been allocated memory
                } else {
                    process->state = RUNNING;
                    runningProcess = process;
                    printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, runningProcess->name, runningProcess->runningTime, percentageUsed(&memoryBlock), process->memoryBlock->start);
                    break;
                }
            }
            // successfully allocated memory
            if (hole != NULL) {
                process->memoryBlock = hole;
                process->state = RUNNING;
                runningProcess = process;
                printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,allocated-at=%d\n", time, runningProcess->name, runningProcess->runningTime, percentageUsed(&memoryBlock), process->memoryBlock->start);
            }
        }
    } else if (strcmp(memoryStrategy, "paged") == 0) {
        runningProcess = deQueueFirstProcess(processQueue);
        // Check if the runningProcess's page are allocated in memory
        if (runningProcess->processPages == NULL) { // If not allocated in memory, create the pages for runningProcess
            runningProcess->processPages = createProcessPages(runningProcess->memory);
            // If there are enough frames to fit all the pages
            if (memoryFrames->numberFramesAvailable >= runningProcess->processPages->totalNumberPages) {
            // Allocate pages to frames
            fillFramesWithPages(memoryFrames, runningProcess->processPages);

            } else {// If there are not enough frames
                // Free frames in memory following least-recently executed policy until there are enough free frames to allocate the processPages. Least recently executed is the next runningProcess in the processQueue.
                int evictionEventPrinted = 0;
                int evictedFrames[MAX_FRAMES];
                // Get the next runningProcess
                ProcessQueue* currentProcessQueue = (*processQueue);
                // Go through that process
                while(currentProcessQueue != NULL && memoryFrames->numberFramesAvailable <=
                runningProcess->processPages->totalNumberPages) {
                    // Check if that process  has pages allocated in memory
                    if (currentProcessQueue->process->processPages != NULL) {
                        // Deallocate all the processes from memoryFrame && delete processes
                        int numFramesRemoved = removeAllPagesFromFrames(memoryFrames,
                                                                        currentProcessQueue->process->processPages,
                                                                        evictedFrames,
                                                                        evictionEventPrinted);
                        if (evictionEventPrinted == 0) {
                            printEvictedFrames(time, numFramesRemoved, evictedFrames);
                            evictionEventPrinted = 1;
                        }
                        currentProcessQueue->process->processPages = NULL;
                    }
                    // Check if enough pages have been deallocated
                    if (memoryFrames->numberFramesAvailable >= runningProcess->processPages->totalNumberPages) {
                        fillFramesWithPages(memoryFrames, runningProcess->processPages);
                        break;
                    } else {
                        currentProcessQueue = currentProcessQueue->next;
                    }
                }
            }
        }

        // If allocated in memory, run runningProcess
        // function to check if the processes in processPages are all allocated
        int numAllocated = checkIfAllocated(runningProcess->processPages);
        if (numAllocated != runningProcess->processPages->totalNumberPages) {
            printf("ERROR ALLOCATING PAGES\n NOT ALL PAGES ALLOCATED? numAllocated = %d\n\n", numAllocated);
            exit(EXIT_FAILURE);
        } else {
            // Run process
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,", time, runningProcess->name,
                   runningProcess->runningTime, percentageFramesUsed(memoryFrames));
            printf("mem-frames=");
            printFramesUsed(runningProcess->processPages);
            printf("\n");
        }
        //printf("paged\n");
    } else if (strcmp(memoryStrategy, "virtual") == 0) {
        runningProcess = deQueueFirstProcess(processQueue);
        int pageRequirement = 0; // The number of additional pages that must be allocated to memory
        // IF: runningProcess -> processPage == NULL (it has not created pages yet)
        if (runningProcess->processPages == NULL) {
            runningProcess->processPages = createProcessPages(runningProcess->memory);
        }
        // IF: runningProcess has processPages created
        if (runningProcess->processPages != NULL) {
            pageRequirement = numPagesRequiredVMA(runningProcess->processPages);
            if (pageRequirement <= 0) { // page requirements met, run program
                if (pageRequirement < 0) {
                    printf("pageRequirement calculated incorrect\n");
                    exit(EXIT_FAILURE);
                }
                // Run process
                printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,", time, runningProcess->name,
                       runningProcess->runningTime, percentageFramesUsed(memoryFrames));
                printf("mem-frames=");
                printFramesUsed(runningProcess->processPages);
                printf("\n");
            } else { // page reqs not met
                if (memoryFrames->numberFramesAvailable >= pageRequirement) {
                    // Fill as many free frames as possible, may be larger than requirement
                    fillFramesWithPages(memoryFrames, runningProcess->processPages);
                } else { // not enough frames
                    ProcessQueue *leastRecentProcessQueue = (*processQueue);
                    int pageEvictPrinted = 0;
                    while (memoryFrames->numberFramesAvailable < pageRequirement) {
                        processPages_t *leastRecentProcessPages = leastRecentProcessQueue->process->processPages;
                        if (leastRecentProcessPages != NULL && leastRecentProcessPages->numberPagesAllocated > 0) {
                            int evictedFrames[pageRequirement];
                            int numPagesEvicted = 0;
                            while(leastRecentProcessPages->numberPagesAllocated > 0 &&
                                  memoryFrames->numberFramesAvailable < pageRequirement) {
                                evictedFrames[numPagesEvicted] = evictFirstFrame(leastRecentProcessPages, memoryFrames);
                                numPagesEvicted++;
                            }
                            if (pageEvictPrinted == 0) {
                                printEvictedFrames(time, numPagesEvicted, evictedFrames);
                                pageEvictPrinted = 1;
                            }
                        }

                        // Move onto next process if still not enough pages freed
                        leastRecentProcessQueue = leastRecentProcessQueue->next;
                    }
                    // Check if there are enough pages mapped
                    if (memoryFrames->numberFramesAvailable == pageRequirement) {
                        fillFramesWithPages(memoryFrames, runningProcess->processPages);
                    } else {
                        printf("ALLOCATE.C LINE 315: SOMETHIGN WRONG WITH MEMORYFRAMES CALCULATIONS\n\n\n");
                        exit(EXIT_FAILURE);
                    }
                }
                // Run process
                printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%%,", time, runningProcess->name,
                       runningProcess->runningTime, percentageFramesUsed(memoryFrames));
                printf("mem-frames=");
                printFramesUsed(runningProcess->processPages);
                printf("\n");
            }
        }

    }
    runningProcess->runningTime -= quantum;
}

int readFromTerminal(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "f:m:q:")) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
            case 'm':
                memoryStrategy = optarg;
                break;
            case 'q':
                quantum = atoi(optarg); // string to int
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s -f <filename> -m <memory-strategy> -q <quantum>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check if enough parameters have been provided
    if (filename == NULL || memoryStrategy == NULL || quantum == 0) {
        fprintf(stderr, "All parameters are required\n");
        fprintf(stderr, "Usage: %s -f <filename> -m <memory-strategy> -q <quantum>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    return 0;
}

// Overhead is turnaround time / service time of a process
void calcTimeOverhead(int *turnaroundTime, int *serviceTime, int numProcesses) {

    double maxTurnaroundTime = 0;
    double averageTurnaroundTime = 0;
    for (int i = 0; i < numProcesses; i++) {
        double sum = turnaroundTime[i] * 1.0 / serviceTime[i];
        if (sum > maxTurnaroundTime) {
            maxTurnaroundTime = sum;
        }
        averageTurnaroundTime += sum;
    }
    averageTurnaroundTime = averageTurnaroundTime / numProcesses;
    printf("Time overhead %.2f %.2f\n", maxTurnaroundTime, averageTurnaroundTime);
}