#include "headers.h"

// Gets the number of students still waiting
int getWaitingStu(){
	int w;

	sem_wait(&stutex);
	w = TAKEN_CHAIRS;
	sem_post(&stutex);

	return w;
}

// Method to have the student thread sleep for a random amount of time
void student_idle(){
	// As a safety precaution, ensure the calculation is atomic
	sem_wait(&stutex);

	static int seed1 = 123456789;
	static int seed2 = 362436069;
	static int seed3 = 521288629;
	static int seed4 = 88675123;
	
	int variant;

	// Generate out random number, using overflow to increase randomness
	variant = seed1 ^ (seed1 << 11);
	seed1 = seed2;
	seed2 = seed3;
	seed3 = seed4;
	seed4 = seed4 ^ (seed4 >> 19) ^ (variant ^ (variant >> 8));
	
	// Get the time we sleep for, between 1 and MAX_WAIT
	int time = (seed4 % MAX_WAIT) + 1;

	sem_post(&stutex);

	// Tell the student to wait/sleep
	sleep(time);
}

// Determine if a chair is available, and if so, awards it to the student
int getChair(){
	sem_wait(&stutex);

	if(TAKEN_CHAIRS < MAX_CHAIRS){
		TAKEN_CHAIRS++;
		sem_post(&stutex);
		return 1;
	}
	sem_post(&stutex);

	return 0;
}

// Method the student thread enters
void *student(void *data){
	// Get the ID passed to the thread
	int id = ((struct args*)data)->entity_num;

	// Get the number of times to be helped
	int help = ((struct args*)data)->entity_data;

	// Number of times this student has been helped
	int helped = 0;
	
	// While the student keeps needing help, loop
	while(helped < help){
		// Check if a chair is available
		if(getChair()){
			// There is a chair, so output to the console that the student is taking a seat
			takeSeat(id, getWaitingStu());

			// Let the coordinator know the student has taken a chair
			sem_post(&stu_chairs);

			// Wait until the coordinator tells the student they're ready to be helped
			sem_wait(&stu_help);

			// Sleep for a random time
			student_idle();

			// Let the coordinator know the student is done being helped
			sem_post(&stu_helped);

			// Increase how much the student has been helped
			helped++;
		} else {
			// If no chairs, output to the console
			noChairs(id);
		}

		// Student goes idle for a time before trying again
		student_idle();
	}

	// Atomically decrement the number of students still needing help
	sem_wait(&stutex);
	REMAINING_STUDENTS--;
	sem_post(&stutex);

	// Return from the thread
	return NULL;
}
