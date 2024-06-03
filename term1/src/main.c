#include "../inc/CPU_sche.h"

bool isIOWorking = false;
bool isCPUBusy = false;
int	currentTime = 0;
Process runningCPUProcess;
Process runningIOProcess;
char timeLine[MAX_TIMELINE_LEN];
char topLine[MAX_TIMELINE_LEN];
char bottomLine[MAX_TIMELINE_LEN];
char middleLineIO[MAX_TIMELINE_LEN];
char middleLineCPU[MAX_TIMELINE_LEN];
// processes array
Process Processes[MAX_PROCESS_SIZE];
// priority queue 초기화
priority_queue jobQueue = {.size = 0};
priority_queue readyQueue = {.size = 0};
priority_queue waitingQueue = {.size = 0};
priority_queue terminatedQueue = {.size = 0};

void	print_menu() {
	char *menu =
		"+============== Scheduling Algorithms ==============+\n"
		"|                                                   |\n"
		"|             1 : FCFS                              |\n"
		"|             2 : SJF (Preemptive)                  |\n"
		"|             3 : SJF (Non Preemptive)              |\n"
		"|             4 : Priority (Preemptive)             |\n"
		"|             5 : Priority (Non Preemptive)         |\n"
		"|             6 : Round Robin                       |\n"
		"|             7 : 프로그램 종료                     |\n"
		"|                                                   |\n"
		"+===================================================+\n"
		"실행할 알고리즘 번호를 선택해주세요 >> ";

	printf("%s", menu);
}

bool finish_menu()
{
	printf("\n계속 하시려면 1을, 종료를 원하시면 아무 키나 입력해주세요. >> ");
	int finFlag;
	scanf("%d", &finFlag);
	return finFlag == 1 ? true : false;
}

int main(void)
{
	srand(time(NULL)); // random seed 설정

	int sche;

	while (1) {
		Create_Process(Processes);

		print_menu();
		scanf("%d", &sche);
		if (sche == 7) {
			break;
		}
		else if (sche > 6 || sche < 1) {
			printf("Please select a correct number\n");
			continue;
		}
		Config(&jobQueue, &readyQueue, &waitingQueue, &terminatedQueue);
		Schedule(sche, &jobQueue, &readyQueue, &waitingQueue, &terminatedQueue);
		// print Gantt chart
		Gantt();
		// scheduler evaluation
		Evaluation(&terminatedQueue);
		// finish scheduler or continue
		switch (finish_menu()) {
			case 1:
				continue;
				break;
			default:
				return 0;
		}
	}
}
