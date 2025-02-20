#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct threadArgs {
	unsigned int id;
	unsigned int numThreads;
	unsigned int squaredId;

};

void* child(void* params) {
	struct threadArgs *args = (struct threadArgs*) params;
	int* squared = (int*)malloc(sizeof(int));
	unsigned int childID = args->id;
	unsigned int numThreads = args->numThreads;
	args->squaredId = args->id * args->id;
	*squared = args->squaredId;
	printf("I am child %u out of %u childrens\n",childID, numThreads);
	return squared;
}

int main(int argc, char** argv) {
	pthread_t* children; // dynamic array of child threads
	struct threadArgs* args; // argument buffer
	unsigned int numThreads = 0;
	int* result;
	// get desired # of threads
	if (argc > 1)
		numThreads = atoi(argv[1]);
	children = malloc(numThreads * sizeof(pthread_t)); // allocate array of handles
	args = malloc(numThreads * sizeof(struct threadArgs)); // args vector
	for (unsigned int id = 0; id < numThreads; id++) {
		// create threads
		args[id].id = id;
		args[id].numThreads = numThreads;
		pthread_create(&(children[id]), // our handle for the child
			NULL, // attributes of the child
			child, // the function it should run
			(void*)&args[id]); // args to that function
	}
	for (unsigned int id = 0; id < numThreads; id++) {
		pthread_join(children[id], (void*)&result);
		printf("I am the parent (main) thread. and child %u has his id squared with %u \n",id,*result);

	}
	free(args); // deallocate args vector
	free(children); // deallocate array
	return 0;
}
