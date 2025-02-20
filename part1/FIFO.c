#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

//Shared Variables

int readFromFIle(int* Ram,int page,int nrOf, FILE *file, int pageSize){
    int number;
        if(fscanf(file,"%u",&number)==1){            
            for (int i = 0; i < nrOf; i++)
            {
                if(Ram[i] == number / pageSize){
                    return 1;
                }
            }
            Ram[page] = number / pageSize;
        }
        else{
            fclose(file);
            return -2;
        }
    return 0;
}

int main(int argc, char** argv) {
    int numberOfPages= 0;
    int pageSize = 0;
    if (argc > 1)
numberOfPages = atoi(argv[1]);
        pageSize = atoi(argv[2]);
    FILE *pF = fopen(argv[3],"r");
    if (pF == NULL) {
        perror("Failed to open file");
        return-2;
    }

    

    int* RAM = malloc(numberOfPages * sizeof(int));
    
    for (int i = 0; i < numberOfPages; i++)
    {
        RAM[i] = -1;
    }
    
    int queue = 0;
    int state = 0;
    int nrOfFault = 0;
    printf("RAM[i1111111]");
    printf("\n");
    while (state >= 0)
    {
        //  printf("%u",queue);
        //  printf("queue");
        //  printf("\n");  
        state = readFromFIle(RAM,queue,numberOfPages, pF, pageSize);
        if(state == 0){
            nrOfFault++;
            printf("%u",nrOfFault);
            printf("nrOfFault");
            printf("\n");  
        queue++;
        }
        if(queue >=numberOfPages){
            queue-=numberOfPages;
        }
        
    }
     printf("RAM[i222222]");
    printf("\n");
    printf("%u",nrOfFault);
    free(RAM);
    return 0;
    
}