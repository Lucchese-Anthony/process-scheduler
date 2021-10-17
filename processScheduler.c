/* 	Author:			Anthony Lucchese
	Program Name:		processScheduler.c
	Compile and run:	gcc -o processScheduler processScheduler.c

	Description:		init a process, and run it using execvp()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

struct JOB {
	//init Job struct
	char command[5][25];
	long submissionTime;
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
void insertOrdered(List *listPtr, Job *jobPtr);
void createFork(Job *jobPtr);
//init methods/functions that appear later in the code

int main() {
	long currentSysTime;
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
			//this was my issue on the last assignment, my printJob()
			//was printing listPtr->firstJob, and threw a segfault because
			//the first job was not listed
			if (listPtr->numOfJobs > 0)  {
				Job *newPtr = deleteFirstJob(listPtr);
			printJob(newPtr);
			} else {
				printf("No Jobs!\n");
			}
		}
	}
	Job *nextJob = NULL;
	while (listPtr->numOfJobs != 0) {
		currentSysTime = time(NULL);
		nextJob = listPtr->first;
		if (currentSysTime >= nextJob->submissionTime + nextJob->startTime) {
			printf("SysTime: %d\n", currentSysTime);       
			Job *tempJob = deleteFirstJob(listPtr);
			printJob(tempJob);
			createFork(tempJob);
			printf("Job Completed...\n");
		}
	}
}

void createFork(Job *jobPtr) {
	int parent_pid, child_pid;
	char *argv[4];
	int status = 999;
	parent_pid = getpid();

	for(int i = 0; i <= 3; i++) {
		if (strcmp("", jobPtr->command[i]) != 0 ) {
			argv[i] = jobPtr->command[i];
		} else {
			argv[i] = NULL;
		}
	}
	child_pid = fork();
	if (child_pid == 0) {
		execvp(argv[0], argv);	// execute process
		exit(1);		// exit if error
	} else {
		waitpid(child_pid, &status, WEXITED);
	}

}

Job *createJob() {
	//simplified createJob function
	Job *jobPtr = (Job*)malloc(sizeof(Job));
	int input;
	scanf("%d", &input);
	for (int i = 0; i < 5; i++) {
		if (i < input) {
			scanf("%s", jobPtr->command[i]);
		} else {
			//if the length of the desired input is less than 5,
			//then insert an empty string where there should be
			//a string, instead of random memory alloc
			strcpy(jobPtr->command[i], "");
		}
	}
	scanf("%d", &jobPtr->startTime);
	jobPtr->submissionTime = time(NULL);
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
	Job *iterPtr;
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
	Job *newPtr;
	if (listPtr->numOfJobs == 1) {
		newPtr = listPtr->first;
		printf("Job Deleted:\n");
		//if the deleted node is the only one,
		//then remove it and reset all head/tail pointers
		listPtr->last = NULL;
		listPtr->first->prev = NULL;
		listPtr->numOfJobs = 0;
		listPtr->first = NULL;
	} else if (listPtr->numOfJobs > 1){
		newPtr = listPtr->first;
                printf("Job Deleted:\n");
		//if there are more than 1,
		//then delete the first one and reset the head
		listPtr->first = listPtr->first->next;
		listPtr->first->prev = NULL;

		listPtr->numOfJobs--;
	} else {
		//self explanatory below
		return newPtr;
	}
	return newPtr;
}

void printList(List *listPtr) {
	if (listPtr->numOfJobs==0){
		printf("Empty list!\n");
	} else {

		printf("# of jobs: %d\n", listPtr->numOfJobs);
		Job *ptr = listPtr->first;
		for (int i = 0; i < listPtr->numOfJobs; i++) {
			printf("Job %d:\n", i+1);
			printJob(ptr);
			ptr = ptr->next;
		}
	}
}

void printJob(Job *jobPtr) {
	printf("program Name:");
	int iter = 0;
	//while the list does not contain any empty strings, print.
	while(strcmp(jobPtr->command[iter], "") != 0) {
		printf(" %s", jobPtr->command[iter]);
		iter++;
	}
	printf("\nsubmission Time: %ld\n", jobPtr->submissionTime);
	printf("start Time: %d\n", jobPtr->startTime);
}
