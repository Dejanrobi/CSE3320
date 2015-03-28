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
 * concurrently using pThreads. Do this for 1 2 4 and 10 threads A menu will be implemented to select which
 * qty of processes.
 * Compile with command: gcc -pthread -o Lab3 NidellLab3Func.c NidellLab3Func.h Lab3.c
 ********************************************/
/**************************HEADERS********************************************/
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
/****************************CONSTANTS***************************************/
//lines in the file
#define MAKE_LENGTH 10
#define RECALL_LENGTH 9
#define FILENAME "sorted.txt"
#define key(A)(A)
#define TOTAL_CARS 106142
#define TRUE         1    
#define FALSE        0   
#define TEN_PROCESS  10
#define FOUR_PROCESS 4
#define TWO_PROCESS  2
#define ONE_PROCESS  1
#define BUFF_SIZE 100
#define less(A,B) (key(A) < key(B))
#define HALF (TOTAL_CARS/2)

struct Cars * listOfCars;
struct Cars * sorted_carlist;
int    tasksPerProcess;

//***********************************STRUCTS--DECLARATIONS******************/
typedef struct Cars
{
    char make[MAKE_LENGTH];
    int  year;
    char recallID[RECALL_LENGTH];//Added for restrictions added by email from Poly.
}Cars;

void swapVehicle(int i, int j, int type);
void bubbleSort( int start, int end, int type);
void delimiter(char *lines, int i);
void readWriteFile();
void* getSortedCars(void* index);
int  getTime( char *c_time_string, time_t current_time );
void mergeSorted(struct Cars *sortedCars, struct Cars *first,struct Cars *second,int w, int x,int y,int z);
void threadStart(int numThreads);//Add for lab 3
void menu(void);
