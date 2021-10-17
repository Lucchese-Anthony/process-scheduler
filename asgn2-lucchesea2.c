/*	Program: 	Doubly Linked List in C
	File name: 	asgn2-lucchesea2.c
	Author: 	Anthony Lucchese
	Compile: 	gcc -o asgn2 asgn2-lucchese.c
	Run:		./asgn2
	Description:	This program asks for various commands,
			start times, submission times, and creates
			a linked list printing in the head->tail 
			and tail->head directions

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defines the struct of JOB
struct JOB {
	char programName[25];
	int submissionTime;
	int startTime;
	struct JOB *prev;
	struct JOB *next;
};

typedef struct JOB Job;

typedef struct LIST {
	int numOfJobs;
	Job *headJob;
	Job *tailJob;
} List;

//declaring function prototypes before the main function
Job *createJob();
void printJob(Job *JobPtr, int jobCount);
List *createList();
void appendJob(List *listPtr, Job *jobPtr);
void printList(List *listPtr);

int main() {
	List *listPtr;

	listPtr = createList();
	
	//prefices the amount of links that will be needed
	
	int jobAmount;
	printf("How many Jobs: ");
	scanf("%d", &jobAmount);

	//create (input) amount of Jobs and add them to the list
	for (int i = 0; i < jobAmount; i++) {
		Job *jobPtr = createJob();
		appendJob(listPtr, jobPtr);
	}

	printList(listPtr);
}
Job *createJob() {

	//these are the temp variables used when a user types their inputs
	char programInput[25];
	int subInput;
	int startInput;

	printf("Enter a command: ");
	scanf("%s", programInput);
	Job *jobPtr = (Job*)malloc(sizeof(Job));
	memcpy(jobPtr->programName, programInput, 24);
	//memcopy is used because assigning a char array to a struct char array
	//by means of 'jobPtr->programName = programInput' throws an error
	
	printf("Enter a submission time: ");
	scanf("%d", &subInput);
	jobPtr->submissionTime = subInput;
	//submission time is set

	printf("Enter a start time: ");
	scanf("%d", &startInput);
	jobPtr->startTime = startInput;
	//start time is set

	return jobPtr;
}

List *createList() {
	List *listPtr = (List*)malloc(sizeof(List));
	listPtr->numOfJobs = 0;
	listPtr->headJob = NULL;
	listPtr->tailJob = NULL;

	return listPtr;
}

//method to append a job to the end of a (doubly) linked list

void appendJob(List *listPtr, Job *jobPtr) {
	if (listPtr->numOfJobs == 0) {
		// no jobs on the list
		jobPtr->prev = NULL;
		jobPtr->next = NULL;

		listPtr->headJob = jobPtr;
		listPtr->tailJob = jobPtr;
		
	} else {
		//1 or more nodes
	
		jobPtr->next = NULL;
		jobPtr->prev = listPtr->tailJob;
		//set the next and previous of the appended link

		listPtr->tailJob->next = jobPtr;
		//the next value of the 'tail' Job, is set to jboPtr
		//so that the previous tail is connected to the current tail
			
		listPtr->tailJob = jobPtr;
		///the new tail job is assigned jobPtr now

	}
	listPtr->numOfJobs++;
}

void printList(List *listPtr) {
	if (listPtr->numOfJobs==0){
		printf("Empty list!\n");
	} else {
		
		printf("\n# of jobs: %d\n", listPtr->numOfJobs);
		Job *ptr = listPtr->headJob;
		for (int i = 0; i < listPtr->numOfJobs; i++) {
			printJob(ptr, i+1);
			ptr = ptr->next;
			//this prints the links from head to tail, starting from 0-n
			//(or really its 1 to (n-1))
		}

		ptr = listPtr->tailJob;
		printf("\nReverse Order:\n");
		for (int i = listPtr->numOfJobs; i != 0; i--) {
			printJob(ptr, (listPtr->numOfJobs - (i-1)));
			ptr = ptr->prev;
			//this is just the inverse of the other for loop, where
			//the loop starts at the tail, and increments down
		}
	}
}

void printJob(Job *jobPtr, int jobCount) {
	//prints the various job arguments how it is stated in the assignment
	printf("Job %d:\n", jobCount);
	printf("program Name: %s\n", jobPtr->programName);
	printf("submission Time: %d\n", jobPtr->submissionTime);
	printf("start Time: %d\n", jobPtr->startTime);
}

