#ifndef HEADERS
#define HEADERS

#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "render.c"

// Student mutex semaphore to protect student data
sem_t stutex;

// Controls student chair access
sem_t stu_chairs;

// Controls when a student is ready to be helped
sem_t stu_help;

// Controls when a student has been helped
sem_t stu_helped;

// Controls for idle tutors
sem_t tut_idle;

// Control for when a tutor is ready to help
sem_t tut_help;

// Control for when the tutor is done helping a student
sem_t tut_done;

// Max chairs allowed
int MAX_CHAIRS;

// Curently taken chairs
int TAKEN_CHAIRS;

// Max wait time (in seconds) for a student to sleep
int MAX_WAIT;

// Remaining students that need help
int REMAINING_STUDENTS;

// Data for args passed to student and tutor threads
struct args {
	int entity_num;
	int entity_data;
};

#endif
