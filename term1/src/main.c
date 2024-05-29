#include "../inc/CPU_sche.h"

int main(void)
{
	srand(time(NULL)); // random seed 설정
	// processes array
	Process Processes[MAX_PROCESS_SIZE];
	// priority queue 초기화
	priority_queue jobQueue = {.size = 0};
	priority_queue readyQueue = {.size = 0};
	priority_queue waitingQueue = {.size = 0};

	while (1) {
		Create_Process();
		Config();
		Schedule();
		Evaluation();
		Gantt();
	}
}