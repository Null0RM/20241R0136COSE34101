#include "../inc/CPU_sche.h"

void Gantt()
{
	puts("");
	for (size_t i = 0; i < strlen(topLine) / 2 - strlen("[CPU Gantt Chart]") / 2; i++)
		printf(" ");
	puts("[CPU Gantt Chart]");
	puts(topLine);
	puts(middleLineCPU);
	puts(bottomLine);
	puts(timeLine);
	puts("");
	puts("");
	for (size_t i = 0; i < strlen(topLine) / 2 - strlen("[IO Gantt Chart]") / 2; i++)
		printf(" ");
	puts("[IO Gantt Chart]");
	puts(topLine);
	puts(middleLineIO);
	puts(bottomLine);
	puts(timeLine);
}