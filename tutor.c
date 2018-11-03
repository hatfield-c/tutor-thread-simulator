#include "headers.h"

// Returns the number of students still waiting
int waitingStu(){
	int w;

	// Lock shared student data
	sem_wait(&stutex);

	// Get shared student data
	w = TAKEN_CHAIRS;

	// Unlock shared student data
	sem_post(&stutex);

	return w;
}

// Returns the remaining students still needing help
int remainingStu(){
	int s;

	// Lock shared student data
	sem_wait(&stutex);

	// Get shared student data
	s = REMAINING_STUDENTS;

	// Unlock chared student data
	sem_post(&stutex);

	return s;
}

// Method that the thread enters
void *tutor(void *data){
	// Get the tutor ID from passed data
	int id = ((struct args*)data)->entity_num;

	// Continuously loop to find and help students
	while(remainingStu() > 0){
		// Let the coordinator know the tutor is ready to help
		sem_post(&tut_idle);

		// Wait until the coordinator is assigned a student to help
		sem_wait(&tut_help);
		
		if(remainingStu() < 1)
			break;

		// Now that we have a student to help, atomically decrement the number
		// of students waiting
		sem_wait(&stutex);
		TAKEN_CHAIRS--;
		sem_post(&stutex);

		// Output a message to the console
		tutorMsg(id, waitingStu());

		// Wait until we're done helping this student
		sem_wait(&tut_done);
	}

	// Exit the thread
	return NULL;
}
