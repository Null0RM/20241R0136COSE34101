#include "../inc/CPU_sche.h"

bool isIOWorking = false;
bool isCPUWorking = false;

int main(void)
{
	srand(time(NULL)); // random seed 설정
	// processes array
	Process Processes[MAX_PROCESS_SIZE];
	// priority queue 초기화
	priority_queue jobQueue = {.size = 0};
	priority_queue readyQueue = {.size = 0};
	priority_queue waitingQueue = {.size = 0};

	// while (1) {
		Create_Process(Processes);
		Config(&jobQueue, &readyQueue, &waitingQueue);
		// Schedule();
		// Evaluation();
		// Gantt();
	// }

}