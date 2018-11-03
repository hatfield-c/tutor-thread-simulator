#include "headers.h"
#include "coordinator.c"
#include "tutor.c"
#include "student.c"
#include "render.c"

const int MAX_ACTORS = 32;

int main(int argc, char* argv[]){
	// Check if enough args
	if(argc != 5){
		err("Not enough arguments!");
		return -1;
	}
	
	int i;
	// Check if each argument passed is a valid int, and store it into config[]
	// 0 : students
	// 1 : tutors
	// 2 : chairs
	// 3 : help
	int config[4];
	for(i = 1; i < argc; i++){
		config[i - 1] = atoi(argv[i]);
		if(config[i - 1] < 1 ||  config[i - 1] > MAX_ACTORS){
			err("Invalid arguments. Arguments must be of type int, greater than 0, and no greater than 32.");
			return -1;
		}
	}

	// Init global value
	MAX_CHAIRS = config[2];
	TAKEN_CHAIRS = 0;
	MAX_WAIT = 5;
	REMAINING_STUDENTS = config[0];

	// Init semaphores
	sem_init(&stutex, 0, 1);
	sem_init(&stu_chairs, 0, 0);
	sem_init(&stu_help, 0, 0);
	sem_init(&stu_helped, 0, 0);

	sem_init(&tut_idle, 0, 0);
	sem_init(&tut_help, 0, 0);
	sem_init(&tut_done, 0, 0);

	// Coordinator
	pthread_t coord_id;
	pthread_create(&coord_id, NULL, coordinator, NULL);

	// Tutors
	pthread_t tutor_ids[config[1]];
	for(i = 0; i < config[1]; i++){
		// Build data to pass to the thread
		struct args* tut_args = (struct args*)malloc(sizeof(struct args));
		tut_args->entity_num = i + 1; 

		// Make the thread, and pass the data to it
		pthread_create(&tutor_ids[i], NULL, tutor, (void*)tut_args);
	}

	// Students
	pthread_t student_ids[config[0]];
	for(i = 0; i < config[0]; i++){
		// Build data to pass to the thread
		struct args* stu_args = (struct args*)malloc(sizeof(struct args));
		stu_args->entity_num = i + 1;
		stu_args->entity_data = config[3];

		// Make the thread, and pass the data to it
		pthread_create(&student_ids[i], NULL, student, (void*)stu_args);
	}

	// Wait for all the students to finish
	for(i = 0; i < config[0]; i++){
		pthread_join(student_ids[i], NULL);
	}

	// The tutors and the coordinators may still be waiting on non-existent students.
	// This will clear out any deadlocks as a result, and allow them to exit gracefully
	for(i = 0; i < config[1]; i++){
		sem_post(&stu_chairs);
		sem_post(&tut_idle);
		sem_post(&tut_help);
		sem_post(&stu_helped);
		sem_post(&tut_done);
	}

	// Confirm that the tutor and coordinator threads exit gracefully
	pthread_join(coord_id, NULL);
	for(i = 0; i < config[1]; i++){
		pthread_join(tutor_ids[i], NULL);
	}

	// Once all theads are closed, exit the application
	printf("SUCCESS: All students have been helped, and all threads have been closed.\n");
	return 0;
}
