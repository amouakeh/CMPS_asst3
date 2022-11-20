#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct fill_cache
{
    long id;
    char *  characters;
};


long *array;
long array_length = 10000000;
int correct_count = 0;
int ThreadsNumber = 8;
long *threads_independantResults;
/*
    int count1s ()
    Requires: Array containing integers
    Effects: Returns the number of 1s in it.
    This function was given in the assignment 
    assumed it runs properly without testing 
    
*/
int count1s()
{

    for (int i = 0; i < array_length; i++)
    {
        if (array[i] == 1)
        {

            correct_count++;
        }
    }
    return correct_count;
}
/*
    void * threadCounting(void *my_struct)
    This function will be executed by each created thread.
    Requires: pointer my_struct
    Effects: Returns the number of ones the threads have counted.
        NO PRECAUTIONS ARE TAKEN TO PREVENT RACE-CONDITIONS. THE RESULT WE GET FROM THE THREADS IS EXPECTED TO BE INCORRECT.
    testcases:
    Array sizes of 100,1000,10000,1000000,10000000,100000000,1000000000 with threads of sizes 1,2,4,8,16,32,64. 
*/
void *threadCounting(void * my_struct)
{
    struct fill_cache *struct_ptr = (struct fill_cache*) my_struct;
    long numberofIterations = array_length / ThreadsNumber;
    long startIndex = (struct_ptr->id) * numberofIterations;
    printf("%d\n", startIndex);
    long endIndex = startIndex + numberofIterations;

    int private_counter = 0;
    int remaining_bytes=256000-((numberofIterations*8)+(3*sizeof(long))+sizeof(int));
    struct_ptr->characters=(char*)malloc(remaining_bytes*sizeof(char));

    if (array_length - endIndex < numberofIterations)
    {
        endIndex = array_length;
    }

    for (startIndex; startIndex < endIndex; startIndex++)
    {
        if (array[startIndex] == 1)
            private_counter++;
    }

    threads_independantResults[struct_ptr->id] = private_counter;
    free(struct_ptr->characters);
}

int main()
{
    int count = 0;

    clock_t start_time, end_time;
    start_time = clock();

    array = (long *)malloc(array_length * sizeof(long));
    threads_independantResults = (long *)malloc(ThreadsNumber * sizeof(long));

    for (long i = 0; i < array_length; i++)
    {
        array[i] = rand() % 5 + 1;
    }

    correct_count = count1s();
    

    pthread_t T[ThreadsNumber];

    for (int i = 0; i < ThreadsNumber; i++)
    {
        
        struct fill_cache fill ;
        fill.id=i;
        pthread_create(&T[i], NULL, &threadCounting, &fill);
    }

    for (int i = 0; i < ThreadsNumber; i++)
    {
        pthread_join(T[i], NULL);
    }

    end_time = clock();
    double total_Time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    for (int j = 0; j < ThreadsNumber; j++)
    {
        count += threads_independantResults[j];
    }

    printf("# of Threads: %d\n", ThreadsNumber);
    printf("Time Taken: %f\n", total_Time);
    printf("Correct count of 1's= %d\n", correct_count);
    printf("Count of 1s with multiple threads: %d\n", count);
    free(array);
    free(threads_independantResults);
    return 0;
}