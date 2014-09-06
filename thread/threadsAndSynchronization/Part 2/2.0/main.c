#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

//Global Variables
int repsInRoom = 0; 			//no. of reps in room
int roomSize;				//max reps in room at once
int repID;				//current repID
int qReady = 0;				//question ready to be asked
int repsLeft;				//no. of reps left to process

//Locks
pthread_mutex_t speakerLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t repLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t roomCountLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t questionLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t questionDone = PTHREAD_MUTEX_INITIALIZER;

//Cond
pthread_cond_t enterRoom = PTHREAD_COND_INITIALIZER;
pthread_cond_t questionPending = PTHREAD_COND_INITIALIZER;
pthread_cond_t repSatisfied = PTHREAD_COND_INITIALIZER;

int main(int argc, char* argv[]) {
	int noReps;			//no. of reps defined by user

	//check for bad input
	if (argc != 3) {
		printf("Bad number of inputs\n");
		exit(-1);
	} else {
		noReps = atoi(argv[1]);
		roomSize = atoi(argv[2]);
		repsLeft = noReps;

		if (roomSize < 1) {  //wrong no. of available positions
			printf("Cannot have room size of 0\n");
			exit(-1);
		}

		if (noReps < 1) {	 //wrong no. of reporters
			printf("Cannot have 0 Reporters\n");
			exit(-1);
		}

		pthread_t sthread;		//speaker thread
		pthread_t threads[noReps];	//reporter threads
		pthread_create(&sthread, NULL, (void*) Speaker, NULL);

		int i;
		for (i = 0; i < noReps; i++)	//create and add reporters (threads)
			pthread_create(&threads[i], NULL, (void*) Reporter, (void*) i);

		for (i = 0; i < noReps; i++)	//join threads
			pthread_join(threads[i], NULL);

		pthread_cancel(sthread);	//ends speaker thread 
	}
	return 0;
}

void Speaker() {
	while (1)								//run for duration of program
	{
		while (qReady == 0)						//while there are no questions
			pthread_cond_wait(&questionPending, &speakerLock);//wait until a reporter asks a question

		AnswerStart();
		AnswerDone();
		qReady = 0;					//question has been answered, go back to 0
		pthread_cond_signal(&repSatisfied);
	}
}

void Reporter(int id) {
	pthread_mutex_lock(&repLock);		//lock room size and reporters in room
	if (repsInRoom == roomSize)
		pthread_cond_wait(&enterRoom, &repLock);
	pthread_mutex_unlock(&repLock);

	EnterConferenceRoom(id);

	int i = 0;
	for (; i < (id % 4) + 2; i++)					//ask questions
			{
		pthread_mutex_lock(&questionLock);//lock so only 1 question is asked at once
		repID = id;
		QuestionStart(id);
		QuestionDone(id);
		pthread_mutex_unlock(&questionLock);
		usleep(1);				//sleep to allow other threads to ask questions
	}

	LeaveConferenceRoom(id);
}

void AnswerStart() {
	printf("Speaker starts to answer question for reporter %d\n", repID);
}

void AnswerDone() {
	printf("Speaker is done with answer for reporter %d\n", repID);
}

void EnterConferenceRoom(int id) {
	pthread_mutex_lock(&roomCountLock);				//enter, so increase counter
	repsInRoom++;
	pthread_mutex_unlock(&roomCountLock);

	printf("Reporter %d enters the conference room\n", id);
}

void LeaveConferenceRoom(int id) {
	pthread_mutex_lock(&roomCountLock);
	repsInRoom--;			//1 less reporter in room, 1 less reporter overall
	repsLeft--;
	pthread_mutex_unlock(&roomCountLock);

	printf("Reporter %d leaves the conference room\n", id);
	pthread_cond_signal(&enterRoom);

//	if(repsLeft <= 0)
//		exit(0);
}

void QuestionStart(int id) {
	printf("Reporter %d asks a question\n", id);		//lock question ready
	pthread_mutex_lock(&speakerLock);
	qReady = 1;
	pthread_mutex_unlock(&speakerLock);
	pthread_cond_signal(&questionPending);
}

void QuestionDone(int id) {
	pthread_cond_wait(&repSatisfied, &questionDone);
	printf("Reporter %d is satisfied\n", id);
}
