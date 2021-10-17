/* 	Author:			Anthony Lucchese
	Program Name:	asgn3-lucchesea2.c
	Compile and run:gcc -o asgn3 asgn3.lucchesea2.c

	Description:	This program is built upon the previous assignment, 
					doubly linked list but adds a delete function, and 
					a situational insert function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct JOB {
	//init Job struct
	char programName[25];
	int submissionTime;
	int startTime;
	struct JOB *prev;
	struct JOB *next;
};

typedef struct JOB Job;

typedef struct LIST {
	//init list struct
	int numOfJobs;
	Job *first;
	Job *last;
} List;


Job *createJob();
Job *deleteFirstJob(List *listPtr);
void printJob(Job *JobPtr);
List *createList();
void printList(List *listPtr);
void appendJob(List *listPtr, Job *jobPtr);
void insertOrdered(List *listPtr, Job *jobPtr);
//init methods/functions that appear later in the code

int main() {
	List *listPtr;
	listPtr = createList();
	char input;
	while (input != '!') {
		//pretty much a input listener, ends if !, del if -, and add if +
		scanf("%c", &input);
		if (input == '+') {
			Job *jobPtr = createJob();
			insertOrdered(listPtr, jobPtr);	
		} else if (input == '-') {
			deleteFirstJob(listPtr);
		}
	}
	printList(listPtr);
}
Job *createJob() {
	//simplified createJob function
	Job *jobPtr = (Job*)malloc(sizeof(Job));
	scanf("%s%d", jobPtr->programName, &jobPtr->submissionTime);
	scanf("%d", &jobPtr->startTime);//longer than 80char together
	return jobPtr;
}

List *createList() {
	//init the list
	List *listPtr = (List*)malloc(sizeof(List));
	listPtr->numOfJobs = 0;
	listPtr->first = NULL;
	listPtr->last = NULL;
	return listPtr;
}

void appendJob(List *listPtr, Job *jobPtr) {
	jobPtr->next = NULL;
	jobPtr->prev = listPtr->last;
	listPtr->last->next = jobPtr;
	listPtr->last = jobPtr;
}

void initializeFirstLastJob(List *listPtr, Job *jobPtr) {
	listPtr->first = jobPtr;
	listPtr->last = jobPtr;
	jobPtr->next = NULL;
	jobPtr->prev = NULL;
}

void replaceFirstNode(List *listPtr, Job *jobPtr) {
	jobPtr->next = listPtr->first;
	jobPtr->prev = NULL;
	listPtr->first->prev = jobPtr;		
	listPtr->first = jobPtr;
}

void insertInMiddle(List *listPtr, Job *jobPtr, int count) {
	//there are more 2 or more nodes, and have to find where it goes in 
	//between the head and tail
	Job *iterPtr = (Job*)malloc(sizeof(Job));
	iterPtr = listPtr->first;
	while (count >= (iterPtr->submissionTime + iterPtr->startTime) ) {
		iterPtr = iterPtr->next;
	}
	jobPtr->next = iterPtr;
	jobPtr->prev = iterPtr->prev;
	iterPtr->prev->next = jobPtr;
	iterPtr->prev = jobPtr;
}

void insertOrdered(List *listPtr, Job *jobPtr) {
	int count = jobPtr->startTime + jobPtr->submissionTime;
	if (listPtr->numOfJobs == 0) {
		initializeFirstLastJob(listPtr, jobPtr);
	} else if (count<=listPtr->first->submissionTime+listPtr->first->startTime){
		replaceFirstNode(listPtr, jobPtr);
	} else if (count>=listPtr->last->submissionTime+listPtr->last->startTime){
		appendJob(listPtr, jobPtr);
	} else {
		insertInMiddle(listPtr, jobPtr, count);
	}
	listPtr->numOfJobs++;
}

Job *deleteFirstJob(List *listPtr) {
	Job *newPtr = (Job*)malloc(sizeof(Job));
	if (listPtr->numOfJobs == 1) {
		printJob(listPtr->first);
		newPtr = listPtr->first;
		listPtr->first->prev = NULL;
		listPtr->last = NULL;
		listPtr->first = NULL;
		listPtr->numOfJobs = 0;
		
	} else if (listPtr->numOfJobs > 1){
		//if there are more than 1,
		//then delete the first one and reset the tail
		printJob(listPtr->first);
		newPtr = listPtr->first;
		listPtr->first->prev = NULL;
		listPtr->first = listPtr->first->next;
	} else {
		//self explanatory below
		return NULL;
	}
	listPtr->numOfJobs--;
	return newPtr;
}

void printList(List *listPtr) {
	if (listPtr->numOfJobs==0){
		printf("Empty list!\n");
	} else {

		printf("\n# of jobs: %d\n", listPtr->numOfJobs);
		Job *ptr = listPtr->first;
		for (int i = 0; i < listPtr->numOfJobs; i++) {
			printf("Job %d:\n", i+1);
			printJob(ptr);
			ptr = ptr->next;
		}

		ptr = listPtr->last;
		printf("\nReverse Order:\n");
		for (int i = listPtr->numOfJobs; i != 0; i--) {
			printf("Job %d:\n", (listPtr->numOfJobs - (i-1)));
			printJob(ptr);
			ptr = ptr->prev;
		}
	}
}

void printJob(Job *jobPtr) {
	printf("program Name: %s\n", jobPtr->programName);
	printf("submission Time: %d\n", jobPtr->submissionTime);
	printf("start Time: %d\n", jobPtr->startTime);
}