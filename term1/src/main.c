#include "../inc/CPU_sche.h"

bool isIOWorking = false;
bool isCPUWorking = false;

void	print_menu() {
	char *guide_letter =
		"수행하고자하는 알고리즘의 번호를 선택해주세요. 종료를 원하시면 이외의 숫자를 눌러주세요\n"
		"0 : FCFS\n"
		"1 : SJF\n"
		"2 : STRF\n"
		"3 : Preemtive Priority\n"
		"4 : Non Preempvie Priority\n"
		"5 : RR\n";

	printf("%s", guide_letter);
}

int main(void)
{
	srand(time(NULL)); // random seed 설정
	// processes array
	Process Processes[MAX_PROCESS_SIZE];
	// priority queue 초기화
	priority_queue jobQueue = {.size = 0};
	priority_queue readyQueue = {.size = 0};
	priority_queue waitingQueue = {.size = 0};
	// priority_queue terminatedQueue = {.size = 0};

	int sche;

	while (1) {
		print_menu();
		scanf("%d", &sche);
		if (sche > 5 || sche < 0)
			continue;
		Create_Process(Processes);
		Config(&jobQueue, &readyQueue, &waitingQueue);
		//Schedule(sche, &jobQueue, &readyQueue, &waitingQueue, &terminatedQueue);
		// Evaluation();
		// Gantt();
	}
}
