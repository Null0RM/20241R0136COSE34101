#include "../inc/CPU_sche.h"

void	initializeQueue(priority_queue *Queue) {
	while (Queue->size > 0)
		pq_pop(Queue, FCFS_enum);
}

void	Config(priority_queue *jobQueue, priority_queue *readyQueue, priority_queue *waitingQueue, priority_queue *terminatedQueue)
{
	extern bool isIOWorking;
	extern bool isCPUWorking;
	// initialize queue
	initializeQueue(jobQueue);
	initializeQueue(readyQueue);
	initializeQueue(waitingQueue);
	initializeQueue(terminatedQueue);
	// IO & CPU state initialize
	isIOWorking = false;
	isCPUWorking = false;
}
