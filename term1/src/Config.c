#include "../inc/CPU_sche.h"

void	initializeQueue(priority_queue *Queue) {
	while (Queue->size > 0)
		pq_pop(Queue, FCFS_enum);
}

void	Config(priority_queue *jobQueue, priority_queue *readyQueue, priority_queue *waitingQueue, priority_queue *terminatedQueue)
{
	currentTime = 0;
	isIOWorking = false;
	isCPUWorking = false;
	// initialize queue
	initializeQueue(jobQueue);
	initializeQueue(readyQueue);
	initializeQueue(waitingQueue);
	initializeQueue(terminatedQueue);
	for (int i = 0; i < MAX_PROCESS_SIZE; i++)
	{
		pq_push(jobQueue, Processes[i], FCFS_enum); // 나중에 time 체크하면서 빼낼때 arrival time 대로 빼내야 하니까 FCFS 스케쥴링대로
	}
	// IO & CPU state initialize
	timeLine[0] = '\0';
	topLine[0] = '\0';
	bottomLine[0] = '\0';
	middleLineIO[0] = '\0';
	middleLineCPU[0] = '\0';
}
