
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *head = NULL;
Task *pickNextTask();
int time_quantum = 10;
bool rr_mode = 0;
int number_of_tasks = 0;
int time = 0;
bool completed = 0;
int tid = 0;

void add(char *name, int priority, int burst) {
	Task *first = (Task *) malloc(sizeof(Task));
	first->name = name;
	first->tid = tid;
	first->priority = priority;
	first -> burst = burst;
	insert(&head, first);
	tid ++;
	number_of_tasks++;
}
void schedule() {
	Task *curr;
	int i=0;
	int finishing_times[number_of_tasks];
	int burst_times[number_of_tasks];
	int response_times[number_of_tasks];
	int touched[number_of_tasks];
	for (int i=0;i<number_of_tasks;i++) {
		touched[i] = 0;
	}
	while (head != NULL) {
		Task *first = (Task *) malloc(sizeof(Task));
		curr = pickNextTask();
		if (rr_mode) {
			if (curr->tid < number_of_tasks) {
				if (touched[curr->tid]==0) {
					response_times[curr->tid] = time;
					burst_times[curr->tid] = curr->burst;
					touched[curr->tid]=1;
				}
			}
			if (curr->burst > time_quantum) {
				run(curr, time_quantum);
				first->name = curr->name;
				first->tid = tid;
				tid++;
				first->priority = curr->priority;
				first->burst = (curr->burst)-10;
				delete(&head, curr);	
				time = time + 10;	
				insert(&head, first);
				rr_mode = 0;
			} else {
				run(curr, curr->burst);
				time = time + curr->burst;
				finishing_times[i] = time;
				delete(&head, curr);
				i++;
				rr_mode = 0;
			}
		} else {
				run(curr, curr->burst);
				if (curr->tid < number_of_tasks) {
					if (touched[curr->tid]==0) {
						response_times[curr->tid] = time;
						touched[curr->tid]=1;
						burst_times[curr->tid] = curr->burst;
					}
				}
				time = time + curr->burst;
				finishing_times[i] = time;
				delete(&head, curr);
				i++;
				rr_mode = 0;
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
	int priority_t=0;
	int same_priority =0;
	Task *priority_task = head -> task;
	if (now->priority > priority_t) {
		priority_task = now;
		priority_t = now -> priority;
	}
		while (sec!=NULL) {
			now = sec->task;
			if (now->priority > priority_t) {
				priority_task = now;
				priority_t = now -> priority;
				same_priority = 0;
			} else if (now->priority == priority_t) {
				same_priority ++;
				priority_task = now;
				priority_t = now -> priority;
			}
			sec = sec->next;
		}
		if (same_priority > 0) {
			rr_mode = 1;
			same_priority = 0;
		}
	return priority_task;
}
