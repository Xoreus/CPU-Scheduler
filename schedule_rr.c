
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *head = NULL;
Task *pickNextTask();
int time_quantum = 10;
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
	int k=0;
	int finishing_times[number_of_tasks];
	int burst_times[number_of_tasks];
	int response_times[number_of_tasks];
	while (head != NULL) {
		Task *first = (Task *) malloc(sizeof(Task));
		curr = pickNextTask();
		if (k < number_of_tasks) {
			burst_times[k] = curr->burst;
			response_times[k] = time;
			k++;
		}
		if (curr->burst > time_quantum) {
			run(curr, time_quantum);
			first->name = curr->name;
			first->priority = curr->priority;
			first->burst = (curr->burst)-10;
			time = time + 10;
			delete(&head, curr);		
			insert(&head, first);
		} else {
			run(curr, curr->burst);
			time = time + curr->burst;
			finishing_times[i] = time;
			delete(&head, curr);
			i++;
		}
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
		while (sec!=NULL) {
			now = sec->task;
			sec = sec->next;
		}
	return now;
}
