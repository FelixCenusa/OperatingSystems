/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>




#define SIZE 1024

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

static void
init_matrix(void)
{
    int i, j;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++) {
	        /* Simple initialization, which enables us to easy check
	         * the correct answer. Each element in c will have the same
	         * value as SIZE after the matmul operation.
	         */
	        a[i][j] = 1.0;
	        b[i][j] = 1.0;
        }
}

void* matmul_par(void*arg)
{
    int row = *(int*)arg;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            c[row][i] = c[row][i] + (a[row][j]*b[j][i]);
        }
    }
}

static void
print_matrix(void)
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++)
	        printf(" %7.2f", c[i][j]);
	    printf("\n");
    }
}

int main(int argc, char **argv)
{
    pthread_t* threads; // dynamic array of child threads
	threads = malloc(SIZE * sizeof(pthread_t)); // allocate array of handles
    init_matrix();

    for (int i = 0; i < SIZE; i++)
    {
        int*id = malloc(sizeof(int));
        *id=i;
        pthread_create(&threads[i],NULL,matmul_par,id);
    }
    

    //print_matrix();
}
