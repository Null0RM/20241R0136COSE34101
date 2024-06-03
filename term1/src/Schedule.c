#include "../inc/CPU_sche.h"

void storeTimeLine()
{
	char time[10];
	sprintf(time, "%-4d", currentTime);
	strcat(timeLine, time);
}

void storeTopBottomLine()
{
	strcat(topLine, "+--+");
	strcat(bottomLine, "+--+");
}

void storeMiddleLine(bool isCpu, bool isIdle)
{
	char *selectedLine = isCpu ? middleLineCPU : middleLineIO;
	if (isIdle)
	{
		strcat(selectedLine, "|--|");
	}
	else
	{
		int pid = isCpu ? runningCPUProcess.Process_ID : runningIOProcess.Process_ID;
		char str[10];
		sprintf(str, "|P%d|", pid);
		strcat(selectedLine, str);
	}
}

void doIOOperation(Process currentProcess, priority_queue *readyQueue, schedule_enum sch_method)
{

	isIOWorking = true;

	// 들어왔다는건 일단 수행되었다는 거니까 io burst time 깎아줌.
	currentProcess.IO_burst_time--;

	//(io burst time 끝날때 (isr 처리 위한 cpu burst time 무조건 남아있을 것) || 1/2 확률로 IO 수행 끝낼 때) && (io busrt time 남아있는데 cpu burst time 1 이하가 아닐때)
	// 만약 io busrt time 남아있는데 cpu burst time 1 이하이면 무조건 다음 io 도 해당 프로세스로 되어야함. 왜냐하면 io 수행하고 isr 처리하기 위한 cpu burst time 남아있어야하기 때문
	if ((currentProcess.IO_burst_time == 0 || rand() % 2 == 0) && !(currentProcess.IO_burst_time > 0 && currentProcess.CPU_burst_time <= 1))
	{
		// IO operation -> readyQueue 에 넣기. 스케쥴링 따라서 넣어줘야 함.
		int tempCurrent = currentTime + 1;
		currentProcess.Arrival_time = tempCurrent;
		pq_push(readyQueue, currentProcess, sch_method);
		isIOWorking = false;
	}
	else
	{
		// IO operation 다음에도 얘로 하겠다.
		runningIOProcess = currentProcess;
	}
}

void doCPUOperation(Process currentProcess, priority_queue *waitingQueue, priority_queue *terminatedQueue)
{
	isCPUBusy = true;

	currentProcess.CPU_burst_time--;
	currentProcess.Processed_time++; // for RR

	// cpu burst time 끝나면 종료
	if (currentProcess.CPU_burst_time == 0)
	{
		// 끝난 프로세스의 처음 상태에 terminate time만 추가해서 terminate queue 큐에 담아줌.
		// currentProcess 는 runningCPUProcess가 넘어와서 CPU_burst_time등이 변경되어있는 상태니까 초기의 상태로 evaluation 해야 함
		int tempCurrent = currentTime + 1;
		Processes[currentProcess.Process_ID].terminated_time = tempCurrent;
		pq_push(terminatedQueue, Processes[currentProcess.Process_ID], FCFS_enum);
		isCPUBusy = false;
		return;
	}

	// IO 수행여부 결정. 1/2 확률 && io burst time이 남아있으면 수행 || io burst 가 남아있는데 cpu burst 1 이하로 남아있으면 waiting Queue로 넘어가서 수행 끝내고 와야함
	if ((rand() % 2 == 0 && currentProcess.IO_burst_time > 0) || (currentProcess.IO_burst_time > 0 && currentProcess.CPU_burst_time <= 1))
	{
		// cpu 끝내고 IO에 넣기
		int tempCurrent = currentTime + 1;
		currentProcess.Arrival_time = tempCurrent; // 현재 시간에는 cpu가 수행 이미 완료 됐고, 그 다음 시간에 waitingQueue로 들어갈 애니까 +1 해서 arrvalTime에 할당
		currentProcess.Processed_time = 0;			// cpu 연속된 시간 0으로 다시 초기화
		pq_push(waitingQueue, currentProcess, FCFS_enum);
		isCPUBusy = false;
	}
	else
	{
		// IO operation 발생 안하는 경우. 다음 수행될 프로세스도 이거란 얘기.
		runningCPUProcess = currentProcess;
	}
}

void Schedule(schedule_enum sche_enum, priority_queue *jobQueue, priority_queue *readyQueue, priority_queue *waitingQueue, priority_queue *terminatedQueue)
{
	// tie break 상황은 그냥 queue에서 맨 상위 노드에 있는걸로.
	while (terminatedQueue->size != MAX_PROCESS_SIZE)
	{
		storeTimeLine();
		storeTopBottomLine();

		// 생성된 프로세스 arrivalTime 체크해서 job 큐에서 reday 큐로 할당. 원래는 랜덤으로 프로세스 들어오겠지만 각 스케쥴링마다 같은 프로세스 사용해야하므로 jobQueue라는 저장공간에 있는 프로세스 꺼내다 씀
		while (true)
		{
			if (jobQueue->size > 0)
			{
				// job queue에 넣어놓은 proc들을 arrivalTime 순으로 꺼냄. 
				Process tmpPop = pq_pop(jobQueue, FCFS_enum);
				if (tmpPop.Arrival_time == currentTime)
				{
					// 현재 시간에 arrive 됐으면 readyQueue에 할당.
					pq_push(readyQueue, tmpPop, sche_enum);
				}
				else
				{
					// 현재 시간 아니면 다시 집어넣고 while 종료
					pq_push(jobQueue, tmpPop, FCFS_enum);
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (isCPUBusy)
		{
			// non preemtive 는 하던 작업 마저 수행 - cpu가 busy 상태면 전에 수행하던 프로세스가 전역변수 runningCPUProcess 에 할당되어 있을 것
			// preemtive 는 ready queue에 다음 프로세스가 있을때 현재 프로세스보다 더 프라이어리티 높나 체크. 현재가 높으면 하던거 수행. 안높으면 원래 하던거(running CPU process)는 레디 큐로 할당해주고 우선순위 높은거를 running cpu process로 바꿔야함
			// 밑에 공통적으로 doCPUOperation() 있음. switch 문에서는 runningProcess 뭘로 할거냐 결정해주는 것
			switch (sche_enum)
			{
			// non-preemptive 방식일 경우, 하던 작업을 마저 수행하면 된다.
			case FCFS_enum:
			case PreemtiveSJF_enum:
			case NonPreemptivePriority_enum:
				break;
			case PreemtivePriority_enum:
				if (readyQueue->size > 0)
				{
					Process tempProcess = pq_pop(readyQueue, sche_enum);
					if (runningCPUProcess.Priority > tempProcess.Priority) // priority
					{
						// 새로 들어온 process가 더 prior한 상황
						pq_push(readyQueue, runningCPUProcess, sche_enum);
						runningCPUProcess = tempProcess;
					}
					else
					{
						// 진행중인 process가 더 prior한 상황
						pq_push(readyQueue, tempProcess, sche_enum); // 다시 반납해줌
					}
				}
				break;
			case NonPreemptiveSJF_enum:
				if (readyQueue->size > 0)
				{
					Process tempProcess = pq_pop(readyQueue, sche_enum);
					if (runningCPUProcess.CPU_burst_time > tempProcess.CPU_burst_time) // cpu burst 
					{
						pq_push(readyQueue, runningCPUProcess, sche_enum);
						runningCPUProcess = tempProcess;
					}
					else
					{
						pq_push(readyQueue, tempProcess, sche_enum);
					}
				}
				break;
			case RR_enum:
				// 만약에 레디큐에 프로세스 없으면 원래 수행되던 애가 타임 퀀텀 관계 없이 계속 수행될 것.
				// 레디큐에 프로세스들 있으면 현재 돌리고 있는 프로세스의 타임퀀텀 체크
				if (readyQueue->size > 0)
				{
					Process tempProcess = pq_pop(readyQueue, sche_enum);
					// RR - 현재 진행되는 프로세스가 timequantum 이상으로 수행됐나 체크. 안넘었으면 이전 프로세스 계속 수행.
					// 시간 넘었으면 ready-queue 에 있던 다른 프로세스 수행
					if (runningCPUProcess.Processed_time >= TIME_QUANTUM)
					{
						runningCPUProcess.Processed_time = 0; // time quantum 지속 시간 초기화
						runningCPUProcess.Arrival_time = currentTime; // current 타임으로 arrival time 초기화. (수행에 문제 없음)
						pq_push(readyQueue, runningCPUProcess, sche_enum);
						runningCPUProcess = tempProcess;
					}
					else
					{
						pq_push(readyQueue, tempProcess, sche_enum);
					}
				}
				break;
			default:
				break;
			} // switch

			doCPUOperation(runningCPUProcess, waitingQueue, terminatedQueue); // 위의 코드에 의해 결정된 runningCPUProcess 시행
			storeMiddleLine(true, false);									  // 수행 될 process 그리기
		}
		else
		{
			// cpu not busy 상태면 readyQueue에서 꺼내서 cpu 할당해줘야함. 그 전에 readyQueue 비어있는 상태인지 체크. 비어있다면 cpu idle 상태.
			if (readyQueue->size > 0)
			{
				Process currentProcess = pq_pop(readyQueue, sche_enum);
				runningCPUProcess = currentProcess;
				doCPUOperation(runningCPUProcess, waitingQueue, terminatedQueue);
				storeMiddleLine(true, false);
			}
			else
			{
				// cpu idle
				storeMiddleLine(true, true);
			}
		}

		if (isIOWorking)
		{
			// io busy 하면 하던 작업 마저 수행
			doIOOperation(runningIOProcess, readyQueue, sche_enum);
			storeMiddleLine(false, false);
		}
		else
		{
			// io not busy 상태면 waitingQueue에서 꺼내서 io 할당해줘야함. 그 전에 waitingQueue 비어있는 상태인지 체크. 비어있다면 io idle 상태.
			if (waitingQueue->size > 0)
			{
				// IO 작업 시작.  arrival time 대로 진행되어야하므로 fcfs 스케쥴링.
				Process currentProcess = pq_pop(waitingQueue, FCFS_enum);
				int tempCurrent = currentTime + 1;

				if (currentProcess.Arrival_time == tempCurrent)
				{
					// 만약 위의 cpu 작업에서 바로 들어온거라면 넘어감. 다시 waitingQueue에 그대로 넣어주고 스루
					pq_push(waitingQueue, currentProcess, FCFS_enum);
					storeMiddleLine(false, true);
				}
				else
				{
					runningIOProcess = currentProcess;
					storeMiddleLine(false, false);
					doIOOperation(runningIOProcess, readyQueue, sche_enum);
				}
			}
			else
			{
				// io idle
				storeMiddleLine(false, true);
			}
		}

		// cpu랑 io 체크 작업 다 했으니까 time 올려주고 다음 작업 수행 반복
		currentTime++;
	} // while
	storeTimeLine(); // 마지막 시간 한번 더 그려줘야함
}
