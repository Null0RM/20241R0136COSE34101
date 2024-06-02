#include "../inc/CPU_sche.h"

void	initializeQueue(priority_queue *Queue) {
	while (Queue->size > 0)
		pq_pop(Queue, FCFS_enum);
}

void	Config(priority_queue *jobQueue, priority_queue *readyQueue, priority_queue *waitingQueue, priority_queue *terminatedQueue)
{
	timeLine[0] = '\0';
	topLine[0] = '\0';
	bottomLine[0] = '\0';
	middleLineIO[0] = '\0';
	middleLineCPU[0] = '\0';

	currentTime = 0;
	isIOWorking = false;
	isCPUBusy = false;

	// initialize queue
	initializeQueue(jobQueue);
	initializeQueue(readyQueue);
	initializeQueue(waitingQueue);
	initializeQueue(terminatedQueue);
	// scheduling 시작할 때, arrival time 순서대로 빼내야 하니까 FCFS 방식으로 job queue에 넣어주고 시작.
	for (int i = 0; i < MAX_PROCESS_SIZE; i++)
	{
		pq_push(jobQueue, Processes[i], FCFS_enum); 
	}
}
