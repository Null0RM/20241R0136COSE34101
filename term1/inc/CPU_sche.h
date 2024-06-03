#ifndef CPU_SCHE_H
# define CPU_SCHE_H

/* __Include_Files_START__ */

#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <time.h>
#  include <stdbool.h>

/* __Include_Files_END__ */

/* __MACROS_AND_TYPEDEF_START__*/
////ENUM_START////
typedef enum
{
	FCFS_enum = 1,
	NonPreemptiveSJF_enum = 2,
	PreemtiveSJF_enum = 3,
	NonPreemptivePriority_enum = 4,
	PreemtivePriority_enum = 5,
	RR_enum = 6
} schedule_enum;
////ENUM_END////
////MACROS_START////
#  define MAX_PROCESS_SIZE 5	
#  define TIME_QUANTUM 3
#  define MAX_CPU_BURST_TIME 9
#  define MAX_IO_BURST_TIME 4
#  define MAX_PRIORITY 4
#  define MAX_ARRIVAL_TIME 10
#  define MAX_TIMELINE_LEN 1500
////MACROS_END////

////TYPEDEF_START////
// 프로세스
typedef struct
{
	int Process_ID;
	int CPU_burst_time;
	int IO_burst_time;
	int Arrival_time;
	int Priority;
	int Processed_time;
	int terminated_time;
	int	queue_level;
} Process;
// 우선순위큐
typedef struct
{
	Process queue[MAX_PROCESS_SIZE];
	int size;
} priority_queue;
/* __MACROS_AND_TYPEDEF_END__*/

/* __FUNCTIONS_START__ */
////HELPER_FUCTION_START////
void swap(Process *first, Process *second);
Process pq_pop(priority_queue *q, schedule_enum schedule_enum);
int pq_push(priority_queue *q, Process process, schedule_enum schedule_enum);

////HELPER_FUNCTIONS_END////

////MAJOR_FUC_START////
void	Create_Process(Process *Processes);
void	Config(priority_queue *jobQueue, priority_queue *readyQueue, priority_queue *waitingQueue, priority_queue *terminatedQueue);
void	Schedule(schedule_enum sche_enum, priority_queue *jobQueue, priority_queue *readyQueue, priority_queue *waitingQueue, priority_queue *terminatedQueue);
void	Evaluation(priority_queue *terminatedQueue);
void	Gantt();
////MAJOR_FUC_END////
/* __FUNCTIONS_END__ */

/* __VARIABLES_START__ */
extern bool isCPUBusy;
extern bool isIOWorking;

extern int currentTime;

extern Process runningCPUProcess;
extern Process runningIOProcess;

extern char timeLine[MAX_TIMELINE_LEN];
extern char topLine[MAX_TIMELINE_LEN];
extern char bottomLine[MAX_TIMELINE_LEN];
extern char middleLineIO[MAX_TIMELINE_LEN];
extern char middleLineCPU[MAX_TIMELINE_LEN];

// processes array
extern Process Processes[MAX_PROCESS_SIZE];
// priority queue 초기화
extern priority_queue jobQueue;
extern priority_queue readyQueue;
extern priority_queue waitingQueue;
extern priority_queue terminatedQueue;
/* __VARIABLES_END__ */

#endif
