#include "../inc/CPU_sche.h"

void Gantt()
{
	puts("[CPU Gantt Chart]");
	puts(topLine);
	puts(middleLineCPU);
	puts(bottomLine);
	puts(timeLine);
	puts("");
	puts("[IO Gantt Chart]");
	puts(topLine);
	puts(middleLineIO);
	puts(bottomLine);
	puts(timeLine);
}