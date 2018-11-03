#include "headers.h"

// Returns the number of students that still need help
int remaining(){
	int r;

	// Lock shared student data
	sem_wait(&stutex);

	// Get shared student data
	r = REMAINING_STUDENTS;

	// Unlock shared student data
	sem_post(&stutex);

	return r;
}

// Method for the coordinator thread
void *coordinator(void *data){
	// Loop continuously, looking for people to help
	while(remaining() > 0){
		// Wait for a student to take a chair
		sem_wait(&stu_chairs);

		// Wait for an idle tutor
		sem_wait(&tut_idle);

		// Tell the tutor they have a student to help
		sem_post(&tut_help);

		// Tell the student they have a tutor to help them
		sem_post(&stu_help);

		// Wait until the student is done being helped 
		sem_wait(&stu_helped);

		// Let the tutor know they're done helping the student
		sem_post(&tut_done);
	}

	return NULL;
}
