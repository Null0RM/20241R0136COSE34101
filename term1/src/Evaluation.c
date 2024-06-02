#include "../inc/CPU_sche.h"

void Evaluation(priority_queue *terminatedQueue)
{
	int totalWaitingTime = 0;
	int totalTurnaroundTime = 0;
	puts("");
	puts("+-----+--------------+-----------------+--------------+-----------------+----------------+---------------+----------+");
	puts("| PID | Waiting_Time | Turnaround_Time | Arrival_Time | Terminated_Time | CPU_Burst_Time | IO_Burst_Time | Priority |");
	puts("+-----+--------------+-----------------+--------------+-----------------+----------------+---------------+----------+");
	while (terminatedQueue->size != 0)
	{
		Process process = pq_pop(terminatedQueue, FCFS_enum);
		int waitingTime = process.terminated_time - process.Arrival_time - process.CPU_burst_time;
		int turnaroundTime = process.terminated_time - process.Arrival_time;
		totalWaitingTime += waitingTime;
		totalTurnaroundTime += turnaroundTime;
		printf("| %2d  |      %2d      |        %2d       |      %2d      |        %2d       |       %2d       |      %2d       |    %2d    |\n", process.Process_ID, waitingTime, turnaroundTime, process.Arrival_time, process.terminated_time, process.CPU_burst_time, process.IO_burst_time, process.Priority);
		puts("+-----+--------------+-----------------+--------------+-----------------+----------------+---------------+----------+");
	}
	printf("Average Waiting time : %.2lf\n", (double)totalWaitingTime / MAX_PROCESS_SIZE);
	printf("Average Turnaround time : %.2lf\n\n", (double)totalTurnaroundTime / MAX_PROCESS_SIZE);
}
