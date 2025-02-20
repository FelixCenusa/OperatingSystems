#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include "LinkedList.c"

//Shared Variables


int readFromFIle(Node** Ram,int nrOf, FILE *file, int pageSize){
    int number;
        if(fscanf(file,"%u",&number)==1){     
            int actualNumber = number / pageSize;    
            int exist = deleteNode(Ram,actualNumber);
            if(size(*Ram) == nrOf){
                pop(Ram);
            }
            append(Ram,actualNumber);
            //displayList(*Ram);

            if(exist == -1){
                //printf("i return now");
                return 1;
            }
        }
        else{
            fclose(file);
            return -1;
        }
    return 0;
}

int main(int argc, char** argv) {
    int numberOfPages= 0;
    int pageSize = 0;
    char* filename ="";
    if (argc > 1)
		numberOfPages = atoi(argv[1]);
        pageSize = atoi(argv[2]);
        filename = argv[3];
    FILE *pF = fopen(filename,"r");
    if (pF == NULL) {
        perror("Failed to open file");
        return-2;
    }

    

    Node *RAM = NULL; 

    int state = 0;
    int nrOfFault = 0;
    //printf("RAM[i1111111]");
    //printf("\n");
    while (state >= 0)
    {
        //  printf("%u",queue);
        //  printf("queue");
        //  printf("\n");  
        state = readFromFIle(&RAM,numberOfPages, pF, pageSize);
        if(state == 1){
            nrOfFault++;
            //printf("%u",nrOfFault);
            //printf("nrOfFault");
            //printf("\n");  
        
        }
        
    }
    //printf("RAM[i222222]");
    printf("\n");
    printf("%u",nrOfFault);
    free(RAM);
    return 0;
    
}