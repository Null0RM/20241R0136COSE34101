#ifndef CPU_SCHE_H
#define CPU_SCHE_H

/* __Include_Files_START__ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* __Include_Files_END__ */

/* __MACROS_AND_TYPEDEF_START__*/
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
	process
} priority_queue;
/* __MACROS_AND_TYPEDEF_END__*/

/* __FUNCTIONS_START__ */

/* __FUNCTIONS_END__ */

#endif