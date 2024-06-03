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
	char *middleLine = isCpu ? middleLineCPU : middleLineIO;
	if (isIdle)
	{
		strcat(middleLine, "|--|");
	}
	else
	{
		int pid = isCpu ? runningCPUProcess.Process_ID : runningIOProcess.Process_ID;
		char str[10];
		sprintf(str, "|P%d|", pid);
		strcat(middleLine, str);
	}
}

void doIOOperation(Process currentProcess, priority_queue *readyQueue, schedule_enum sch_method)
{

	isIOWorking = true;

	// 일단 io burst time 깎아줌.
	currentProcess.IO_burst_time--;

	//(io burst time 끝날때 || 1/2 확률로 IO 수행 끝낼 때) && (io busrt time 남아있는데 cpu burst time 1 이하가 아닐때)
	// 만약 io busrt time 남아있는데 cpu burst time 1 이하이면 다음 io 도 해당 프로세스로 되어야함. isr 과정이 남아있어야 함.
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
		// 다음에도 얘로 하겠다.
		runningIOProcess = currentProcess;
	}
}

void doCPUOperation(Process currentProcess, priority_queue *waitingQueue, priority_queue *terminatedQueue)
{
	isCPUBusy = true;

	currentProcess.CPU_burst_time--;
	currentProcess.Processed_time++; // for RR

	// cpu burst time 0이면 process 종료
	if (currentProcess.CPU_burst_time == 0)
	{
		// 끝난 프로세스의 처음 상태에 terminate time만 추가해서 terminate queue 큐에 담아줌.
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
		currentProcess.Processed_time = 0;		   // cpu 연속된 시간 0으로 다시 초기화
		pq_push(waitingQueue, currentProcess, FCFS_enum);
		isCPUBusy = false;
	}
	else
	{
		// IO operation 발생 안하는 경우.
		runningCPUProcess = currentProcess;
	}
}

void Schedule(schedule_enum sche_enum, priority_queue *jobQueue, priority_queue *readyQueue, priority_queue *waitingQueue, priority_queue *terminatedQueue)
{
	while (terminatedQueue->size != MAX_PROCESS_SIZE)
	{
		storeTimeLine();
		storeTopBottomLine();

		// arrive time == currentTime이라면 readyQueue에 할당.
		while (true)
		{
			if (jobQueue->size > 0)
			{
				Process tmpPop = pq_pop(jobQueue, FCFS_enum);
				if (tmpPop.Arrival_time == currentTime)
				{
					pq_push(readyQueue, tmpPop, sche_enum);
				}
				else
				{
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
			// non preemtive 는 하던 작업 마저 수행
			// preemtive 는 ready queue에 다음 프로세스가 있을때 현재 프로세스보다 더 할당 순위 높나 체크. 
			// runningProcess 뭘로 할거냐 결정해주고, doCPUoperation 수행.
			switch (sche_enum)
			{
			// non-preemptive 일 경우, 그냥 지나가면 됨.
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
			}
			doCPUOperation(runningCPUProcess, waitingQueue, terminatedQueue); // runningCPUProcess에 대해 doCPUOperation 수행
			storeMiddleLine(true, false);
		}
		else
		{
			// isCPUBusy == 0 상태면 readyQueue에서 꺼내서 할당해줘야함. 
			// readyQueue 비어있는 상태인지 체크. 비어있다면 cpu idle 상태.
			if (readyQueue->size > 0)
			{
				Process currentProcess = pq_pop(readyQueue, sche_enum);
				runningCPUProcess = currentProcess;
				doCPUOperation(runningCPUProcess, waitingQueue, terminatedQueue);
				storeMiddleLine(true, false);
			}
			else // ready queue busy
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
			// isIOBusy == 0 상태면 waitingQueue에서 꺼내서 io 할당해줘야함. 그 전에 waitingQueue 비어있는 상태인지 체크. 비어있다면 io idle.
			if (waitingQueue->size > 0)
			{
				// IO 작업 시작.  arrival time 대로 진행되어야하므로 fcfs 스케쥴링.
				Process currentProcess = pq_pop(waitingQueue, FCFS_enum);
				int tempCurrent = currentTime + 1;

				if (currentProcess.Arrival_time == tempCurrent)
				{
					// cpu 작업이 그대로 들어온거라면 넘어감. 다시 waitingQueue에 넣어줌.
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
		// 1 time operation finished
		currentTime++;
	}
	storeTimeLine(); 
}
