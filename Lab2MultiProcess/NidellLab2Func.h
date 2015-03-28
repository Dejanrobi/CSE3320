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
struct Cars * listOfCarsCopy;
struct Cars * sorted_carlist;
int    tasksPerProcess;

//***********************************STRUCTS--DECLARATIONS******************/
typedef struct Cars
{
    char make[MAKE_LENGTH];
    int  year;
}Cars;

void swapVehicle(int i, int j, int type);
void bubbleSort( int start, int end, int type);
void delimiter(char *lines, int i, int type);
void readWriteFile( int type );
void getSortedCars(int i, int j);
int  getTime( char *c_time_string, time_t current_time );
void mergeSorted(struct Cars *sorted,struct Cars *a,struct Cars *b, int m,int n );
void read2Processes(void);
void read4Processes(void);
void read10Processes(void);
void oneProcesses(void);
void twoProcesses (void);
void fourProcesses(void);
void tenProcesses(void);
void menu(void);
