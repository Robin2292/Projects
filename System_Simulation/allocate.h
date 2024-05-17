/** allocate.h: functions that help to run the program and calculate stats */
#ifndef _ALLOCATE_H
#define _ALLOCATE_H

#include <unistd.h> // reading from command line
#include <getopt.h>
#include "ProcessQueue.h"
#include "MemoryFrame.h"
#include "ProcessPage.h"

#define MIN_PAGE_REQ 4

void runNextProcess(ProcessQueue** processQueue, char* memoryStrategy);
int readFromTerminal(int argc, char **argv);
void calcTimeOverhead(int *turnaroundTime, int *serviceTime, int numProcesses);
#endif //_ALLOCATE_H