#include <stdio.h>
#include <pthread.h>

#include "coordinator.c"
#include "tutor.c"
#include "student.c"
#include "render.c"

const int MAX_ACTORS = 32;

int main(int argc, char* argv[]){
	if(argc != 5){
		err("Not enough arguments!");
		return -1;
	}

	int i;
	// 0 : students
	// 1 : tutors
	// 2 : chairs
	// 3 : help
	int config[4];
	for(i = 1; i < argc; i++){
		config[i - 1] = atoi(argv[i]);
		if(config[i - 1] < 1 ||  config[i - 1] > MAX_ACTORS){
			err("Invalid arguments. Arguments must be of type in, greater than 0, and no greater than 32.");
			return -1;
		}
	}

	// Coordinator
	pthread_t coord_id;
	pthread_create(&coord_id, NULL, coordinator, NULL);

	// Tutors
	pthread_t tutor_ids[config[1]];
	for(i = 0; i < config[1]; i++){
		pthread_create(&tutor_ids[i], NULL, tutor, NULL);
	}

	printf("FLAG\n");

	// Students
	pthread_t student_ids[config[0]];
	for(i = 0; i < config[0]; i++){
		pthread_create(&student_ids[i], NULL, student, NULL);
	}

	pthread_join(coord_id, NULL);
	for(i = 0; i < config[1]; i++){
		pthread_join(tutor_ids[i], NULL);
	}
	for(i = 0; i < config[0]; i++){
		pthread_join(student_ids[i], NULL);
	}

	return 0;
}
