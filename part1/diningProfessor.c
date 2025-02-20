#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

//Shared Variables
sem_t forks[5];

void* child(void* params) {
	int thinking = rand() % 5 + 1; // rand() % 5 generates [0,4], adding 1 gives [1,5]
	int thinkingAfterFork = rand() % 9 + 2; // rand() % 5 generates [0,4], adding 1 gives [1,5]
	int eating = rand() % 6 + 5; // rand() % 5 generates [0,4], adding 1 gives [1,5]
	int leftFork,rightFork;
	int *childID = (int*)params;
	printf("Hello im professor number #%u and i'm now gonna be thinking for #%u seconds\n", *childID,thinking);
	sleep(thinking);
	printf("Hello im professor number #%u and im done thinking\n", *childID);
	while (1)
	{
		
		sem_getvalue(&forks[*childID-1],&leftFork);
		sem_getvalue(&forks[*childID%5],&rightFork);
		if(leftFork == 1 && rightFork == 1){
			printf("Hello im professor number #%u BOTH FORKS ARE FREE SO IM GONNA PICK UP AND EAT \n", *childID);
			sem_wait(&forks[*childID-1]);
		  	sem_wait(&forks[(*childID)%5]);
			printf("Hello im professor number #%u BOTH forks has been PICKED ill eat now\n", *childID);
		 	sleep(eating);
			printf("Hello im professor number #%u and im done eating so ill drop my forks\n", *childID);
			sem_post(&forks[*childID-1]);
			sem_post(&forks[(*childID)%5]);
			printf("Hello im professor number #%u and i'm now gonna be thinking for #%u seconds\n", *childID,thinking);
			sleep(thinking);
			printf("Hello im professor number #%u and im done thinking\n", *childID);
		}	
	}
	free(childID);
	
}

int main(int argc, char** argv) {
	srand(time(NULL));
	pthread_t children[5]; // dynamic array of child threads
	unsigned int numThreads = 5;

	// get desired # of threads
	for (unsigned int id = 1; id < numThreads+1; id++) {
		sem_init(&forks[id-1],0,1);
		int *idNumber = malloc(sizeof(int)); // Allocate a new memory block for each thread		
		// create threads
		*idNumber = id;
		pthread_create(&(children[id-1]), // our handle for the child
			NULL, // attributes of the child
			child, // the function it should run
			(void*)idNumber); // args to that function
	}
	for (unsigned int id = 0; id < numThreads; id++) {
		pthread_join(children[id], NULL );
	}
	return 0;
}
