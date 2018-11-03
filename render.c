#ifndef RENDER
#define RENDER

#include <stdio.h>

// There has been an error, output it to the screen
void err(char* msg){
	printf("[ERROR]: %s\n", msg);
}


// A student is taking a seat
void takeSeat(int id, int waiting){
	printf("Student %d taks a seat. Waiting students = %d\n", id, waiting);
}

// No chairs are left for a student
void noChairs(int id){
	printf("Student %d found no empty chairs, and will try again later.\n", id);
}

// A tutor is helping a student
void tutorMsg(int id, int waiting){
	printf("Tutor %d helping a student. Waiting students = %d\n", id, waiting);
}

#endif
