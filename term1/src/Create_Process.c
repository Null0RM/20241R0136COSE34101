#include "../inc/CPU_sche.h"

Process Create_Random_Process(int pid) {
	int Process_ID = pid;
	int CPU_burst_time = rand() % (MAX_CPU_BURST_TIME) + 1;
	int IO_burst_time = CPU_burst_time <= 1 ? 0 : rand() % (MAX_IO_BURST_TIME + 1);
	int Arrival_time = rand() % (MAX_ARRIVAL_TIME + 1);
	int Priority = rand() % (MAX_PRIORITY + 1);
	Process Res_Process = {.Process_ID = Process_ID, .CPU_burst_time=CPU_burst_time, .IO_burst_time=IO_burst_time, .Arrival_time=Arrival_time, .Priority=Priority};
	return Res_Process;
}

void	Print_Processes(Process *Processes) {
	puts("+-----+----------------+---------------+--------------+----------+");
	puts("| PID | CPU_Burst_Time | IO_Burst_Time | Arrival_Time | Priority |");
	puts("+-----+----------------+---------------+--------------+----------+");

	for (int i = 0; i < MAX_PROCESS_SIZE; i++)
	{
		printf("| %2d  |       %2d       |      %2d       |      %2d      |    %2d    |\n", Processes[i].Process_ID, Processes[i].CPU_burst_time, Processes[i].IO_burst_time, Processes[i].Arrival_time, Processes[i].Priority);
		puts("+-----+----------------+---------------+--------------+----------+");
	}
}

void	Create_Process(Process *Processes)
{
	for(int i = 0; i < MAX_PROCESS_SIZE; i++) 
	{
		Processes[i] = Create_Random_Process(i);
	}
	Print_Processes(Processes);
}
