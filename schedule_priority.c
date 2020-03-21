
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *head = NULL;
Task *pickNextTask();
int number_of_tasks = 0;
int time = 0;

void add(char *name, int priority, int burst) {
	Task *first = (Task *) malloc(sizeof(Task));
	first->name = name;
	first->priority = priority;
	first -> burst = burst;
	insert(&head, first);
	number_of_tasks++;
}
void schedule() {
	Task *curr;
	int i=0;
	int finishing_times[number_of_tasks];
	int burst_times[number_of_tasks];
	int response_times[number_of_tasks];
	while (head != NULL) {
		curr = pickNextTask();
		run(curr, curr->burst);
		response_times[i] = time;
		time = time + curr->burst;
		finishing_times[i] = time;
		burst_times[i] = curr->burst;
		delete(&head, curr);
		i++;
	}
	double sum_turnaround = 0;
	double sum_waiting = 0;
	double sum_response = 0;
	for (int k=0;k<number_of_tasks;k++) {
		sum_turnaround = finishing_times[k] + sum_turnaround;
		sum_waiting = sum_waiting + (finishing_times[k] - burst_times[k]);
		sum_response = sum_response + response_times[k];
	}
	printf("Average Turnaround Time = %.2f; Average Waiting Time = %.2f; Average Response Time = %.2f \n", sum_turnaround/number_of_tasks, sum_waiting/number_of_tasks, sum_response/number_of_tasks);
}
Task *pickNextTask() {
	struct node *sec;
	Task *now = head->task;
	sec = head->next;
	int priority_t=0;
	Task *priority_task = head -> task;
	if (now->priority > priority_t) {
		priority_task = now;
		priority_t = now -> priority;
	}
		while (sec!=NULL) {
			now = sec->task;
			if (now->priority >= priority_t) {
				priority_task = now;
				priority_t = now -> priority;
			}
			sec = sec->next;
		}
	return priority_task;
}
