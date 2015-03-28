/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 3 Multi-threaded 1,2,4,10
 * Due Date: 3/1/2015
 *******************************/

/********************************************
 * Project Description: 
 * Create threads in parallel, and pass information between threads. Data is read from FLAT_RCL.txt
 * Use bubble sort to sort the data. Time each type of thread. Create a program that will run multiple threads
 * concurrently using pThreads. Do this for 1 2 4 and 10 threads A menu will be
 * implemented to select which qty of processes. 
 * Compile with command: gcc -pthread -o Lab3 NidellLab3Func.c NidellLab3Func.h Lab3.c
 *
 ********************************************/

/**************************HEADERS********************************************/
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include "NidellLab3Func.h"
#include <pthread.h>

/************************MAIN*************************************************/
int main(){
    int selection;
    sorted_carlist = malloc(sizeof(Cars) * TOTAL_CARS);
    listOfCars = malloc(sizeof(Cars) * TOTAL_CARS);    
    
    remove("output.txt");//remove file if exist
    readWriteFile();
    menu();
    free(listOfCars);     // Free memory
    
    return EXIT_SUCCESS;
    
    
}




