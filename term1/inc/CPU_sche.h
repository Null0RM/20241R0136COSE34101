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
	FCFS_enum,
	NonPreemptiveSJF_enum,
	PreemtiveSJF_enum,
	NonPreemptivePriority_enum,
	PreemtivePriority_enum,
	RR_enum
} schedule_enum;
////ENUM_END////
////MACROS_START////
#  define MAX_PROCESS_SIZE 10
#  define TIME_QUANTUM 3
#  define MAX_CPU_BURST_TIME 10
#  define MAX_IO_BURST_TIME 5
#  define MAX_PRIORITY 4
#  define MAX_ARRIVAL_TIME 10
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
} Process;
// 우선순위큐
typedef struct
{
	Process heap[MAX_PROCESS_SIZE];
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
void Create_Process(Process *Processes);
void	Config();
void	Schedule();
void	Evaluation();
void	Gantt();
////MAJOR_FUC_END////
/* __FUNCTIONS_END__ */

/* __VARIABLES_START__ */
extern bool isCPUWorking;
extern bool isIOWorking;
/* __VARIABLES_END__ */

#endif