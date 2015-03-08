/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 2 Multi-process 1,2,4,10
 * Due Date: 3/1/2015
 *******************************/

/********************************************
 * Project Description: 
 * Create processes, run processes in parallel, and pass information between processes. Data is read from FLAT_RCL.txt
 * Use bubble sort to sort the data. Time each type of process. Create a program that will run multiple processes
 * concurrently using fork() and exec(). Do this for 1 2 4 and 10 processes A menu will be implemented to selected which
 * qty of processes. 
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
#include "NidellLab2Func.h"

/************************MAIN*************************************************/
void main(){
    listOfCarsCopy = malloc(sizeof(Cars) * TOTAL_CARS);
    sorted_carlist = malloc(sizeof(Cars) * TOTAL_CARS);
    listOfCars = malloc(sizeof(Cars) * TOTAL_CARS);
    
    
    remove("output.txt");//file could possibly be in dir. Remove it before creating new file
    
    readWriteFile(1);
    menu();

    free(listOfCars);     // Free memory
    free(listOfCarsCopy); // Free memory
    
    
}




