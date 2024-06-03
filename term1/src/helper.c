#include "../inc/CPU_sche.h"

void swap(Process *first, Process *second) {
    Process tmp = *first;
    *first = *second;
    *second = tmp;
}

int pq_push(priority_queue *q, Process process, schedule_enum schedule_enum)
{

	int size = q->size;

	// if Queue Full, return.
	if (size + 1 > MAX_PROCESS_SIZE)
		return 0;

	q->queue[size] = process; // 마지막 빈 자리에 process 할당

	// 재정렬
	int current = size;			 // 현재 위치한 인덱스
	int parent = (size - 1) / 2; // 완전 이진트리에서 parent 인덱스

	switch (schedule_enum)
	{
	case FCFS_enum:
	case RR_enum:
		while (current > 0 && (q->queue[current].Arrival_time) < (q->queue[parent].Arrival_time))
		{
			swap(&(q->queue[current]), &(q->queue[parent]));
			current = parent;
			parent = (parent - 1) / 2;
		}
		break;
	case NonPreemptiveSJF_enum:
	case PreemtiveSJF_enum:
		while (current > 0 && (q->queue[current].CPU_burst_time) < (q->queue[parent].CPU_burst_time))
		{
			swap(&(q->queue[current]), &(q->queue[parent]));
			current = parent;
			parent = (parent - 1) / 2;
		}
		break;
	case NonPreemptivePriority_enum:
	case PreemtivePriority_enum:
		while (current > 0 && (q->queue[current].Priority) < (q->queue[parent].Priority))
		{
			swap(&(q->queue[current]), &(q->queue[parent]));
			current = parent;
			parent = (parent - 1) / 2;
		}
		break;
	default:
		break;
	}

	q->size++;

	return 1;
}

Process pq_pop(priority_queue *q, schedule_enum schedule_enum)
{
	// 비어있으면 return
	Process process = {.Process_ID = -1};
	if (q->size <= 0)
		return process;

	// 우선 순위 큐에서 pop 할 것은 가장 우선 순위가 높은 노드, 즉 루트
	Process ret = q->queue[0];
	q->size--;

	// 재정렬
	q->queue[0] = q->queue[q->size]; // 루트에 가장 낮은거 올림

	int current = 0;
	int leftChild = current * 2 + 1;
	int rightChild = current * 2 + 2;
	int maxNode = current;

	switch (schedule_enum)
	{
	case FCFS_enum:
	case RR_enum:
		while (leftChild < q->size)
		{
			// left child 가 있는데 max node의 arrival time 이 더 큰 경우
			if ((q->queue[maxNode]).Arrival_time > (q->queue[leftChild]).Arrival_time)
			{
				maxNode = leftChild;
			}
			// right child 까지 있는데 max node(방금전까지 leftChild의 값)의 arrival time 이 더 큰 경우
			if (rightChild < q->size && q->queue[maxNode].Arrival_time > q->queue[rightChild].Arrival_time)
			{
				maxNode = rightChild;
			}

			if (maxNode == current)
			{
				break;
			}
			else
			{
				swap(&(q->queue[current]), &(q->queue[maxNode]));
				current = maxNode;
				leftChild = current * 2 + 1;
				rightChild = current * 2 + 2;
			}
		}
		break;
	case NonPreemptiveSJF_enum:
	case PreemtiveSJF_enum:
		while (leftChild < q->size)
		{
			if ((q->queue[maxNode]).CPU_burst_time > (q->queue[leftChild]).CPU_burst_time)
			{
				maxNode = leftChild;
			}
			if (rightChild < q->size && q->queue[maxNode].CPU_burst_time > q->queue[rightChild].CPU_burst_time)
			{
				maxNode = rightChild;
			}

			if (maxNode == current)
			{
				break;
			}
			else
			{
				swap(&(q->queue[current]), &(q->queue[maxNode]));
				current = maxNode;
				leftChild = current * 2 + 1;
				rightChild = current * 2 + 2;
			}
		}
		break;
	case NonPreemptivePriority_enum:
	case PreemtivePriority_enum:
		while (leftChild < q->size)
		{
			if ((q->queue[maxNode]).Priority > (q->queue[leftChild]).Priority)
			{
				maxNode = leftChild;
			}
			if (rightChild < q->size && q->queue[maxNode].Priority > q->queue[rightChild].Priority)
			{
				maxNode = rightChild;
			}

			if (maxNode == current)
			{
				break;
			}
			else
			{
				swap(&(q->queue[current]), &(q->queue[maxNode]));
				current = maxNode;
				leftChild = current * 2 + 1;
				rightChild = current * 2 + 2;
			}
		}
		break;
	default:
		break;
	}
	return ret;
}
