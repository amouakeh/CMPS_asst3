#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long * array;

long array_length=100;
// Global variable to which all threads are writing into
int count=0;
// This will be the result the sequential function returns
int correct_count=0;

int ThreadsNumber=1;
/*
    int count1s ()
    Requires: Array containing integers
    Effects: Returns the number of 1s in it.
    This function was given in the assignment 
    assumed it runs properly without testing 
    
*/
// Sequential Code - Given in the assignment
int count1s ()
 {

 for (int i=0; i<array_length; i++)
 {
        if (array[i] == 1)
    {
        
        correct_count++;
    } 
 }
    return correct_count;
 }
/*
    void * threadCounting(void *id)
    This function will be executed by each created thread.
    Requires: The id of the thread. The id ranges from 0 to n-1 where n=number of threads.
    Effects: Returns the number of ones the threads have counted.
        NO PRECAUTIONS ARE TAKEN TO PREVENT RACE-CONDITIONS. THE RESULT WE GET FROM THE THREADS IS EXPECTED TO BE INCORRECT.
    testcases:
    Array sizes of 100,1000,10000,1000000,10000000,100000000,1000000000 with threads of sizes 1,2,4,8,16,32,64. 
*/

 void * threadCounting(void *id)
 {
    long ID = (long) id;

    long numberofIterations = array_length / ThreadsNumber;

    long startIndex = ID*numberofIterations;

    long endIndex = startIndex+numberofIterations;

    // If this is the last thread, let it iterate till the end of the array
    if(array_length-endIndex<numberofIterations)
    {
        endIndex=array_length;
    }

    for(startIndex;startIndex<endIndex;startIndex++)
        {
            if(array[startIndex] == 1)
                count++;
        } 
 }

 int main()
{
    clock_t start_time,check_time1, end_time;

    array = (long*)malloc(array_length * sizeof(long));

    for (long i=0;i<array_length;i++)
    {
        array[i] = rand() % 5 + 1;
    }

    correct_count = count1s();
    int correct_answer=0;

    double timing [100] = {0};

    for(int m=0;m<100;m++)
    {

    start_time=clock();  
    pthread_t T[ThreadsNumber];

    for(int i=0;i<ThreadsNumber;i++)
    {
        pthread_create(&T[i],NULL,&threadCounting,(void*)i);
    }

    for (int i=0;i<ThreadsNumber;i++)
    {
        pthread_join(T[i],NULL);
    }

    end_time=clock();
    double total_Time_parallel = ((double) (end_time-start_time)) / CLOCKS_PER_SEC;
    timing[m] = total_Time_parallel;
    if(count==correct_count)
        correct_answer++;
    
    start_time=0;
    end_time=0;
    count=0;
    

    }
    double averageTime=0;
    for(int i=0;i<100;i++)
    {
        averageTime+=timing[i];
    }
    averageTime = averageTime/100.0;
    
   printf("Average Time: %f",averageTime);


    return 0;

}