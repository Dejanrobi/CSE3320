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
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "NidellLab3Func.h"
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
/****************************CONSTANTS***************************************/
//lines in the file
#define FILENAME "sorted.txt"
#define key(A)(A)
#define TRUE         1    
#define FALSE        0  
#define BUFF_SIZE 100
#define TEN_PROCESS  10
#define FOUR_PROCESS 4
#define TOTAL_CARS 106142
#define TWO_PROCESS  2
#define ONE_PROCESS  1
#define less(A,B) (key(A) < key(B))
#define HALF (TOTAL_CARS/2)
int    tokenIndex = 0; //incrementer for tokenizer



/**************************************FUNCTIONS*************************/
/**
 * Description:    Bubbles sorts cars first by year than by make
 * input--
 * int start:	   The starting index of where the process should start sorting
 * int end:	   The last index of where the process should end sorting
 * int type: 	   Not used..
 * 
 * return:         void 
 */
void  bubbleSort( int start, int end, int type){
    int i;
    int not_sorted;
    
    do{
        not_sorted = FALSE;                                  
        
        for (i=start; less(i,end-1); i++){
            if (less(listOfCars[i+1].year, listOfCars[i].year)){
                swapVehicle(i, (i+1), 1);
                not_sorted = TRUE;                        
            }
            else if ((listOfCars[i+1].year-listOfCars[i].year)== 0){
                if (strcmp(listOfCars[i].make, listOfCars[i+1].make) > 0){
                    swapVehicle(i, (i+1), 0);
                    not_sorted = TRUE;                     
                }
            }
        }
    }
    while (not_sorted);
}

/**
 * Description:    Helper method for bubble sort that swaps the parts of the Cars Struct
 * input-- 
 * int i:          first index item being swapped
 * int j:          second index item being swapped
 * int type:       what is being swapped. 0=Second Swap comparing make
 * 				   1= swap the cars make.	
 * return: 	       void
 */
void swapVehicle(int i, int j, int type){
    struct Cars temp;
    char stringSwap[20];
    
    if (type == 0){
        strcpy(stringSwap, listOfCars[i].make);
        strcpy(listOfCars[i].make, listOfCars[j].make);
        strcpy(listOfCars[j].make, stringSwap);
    }
    else{
        temp = listOfCars[i];
        listOfCars[i] = listOfCars[j];
        listOfCars[j] = temp;
    }
}
/**
 * Description:     Pulls Year and make from the file by line. Make is 2 tabs, year is 4 tabs
 *                  it then stores 2 copies 1 to listOfCars another to listOfCarsCopy
 * input--
 * char *lines:     The current line of the file being read
 * int i:	     current line number
 * int type:	     Type of read. some lines are 2 and 4 tabs other is 3 and 1.
 * 
 * return:          void
 */
void delimiter(char *lines, int i){
    int   delta=0;//keeps track of where we are in the readline.
    char *delimitedString;
    
    
    

        delimitedString=strtok(lines,"\t");//delimter
        while (delimitedString != NULL) {
            if(delta==1){
               strcpy(listOfCars[i].recallID,delimitedString);
               //printf("Line: %d	ID: %s\n",i,delimitedString);
            }
            if(delta==2) strcpy(listOfCars[i].make,delimitedString);
            if(delta==4) listOfCars[i].year=atoi(delimitedString);//convert to int
            delimitedString = strtok(NULL,"\t");
            delta++;
        }    
}

/**
 * Description:      Opens the file being read FLAT_RCL.txt reads line by line and passes this line 
 *                   to get tokenization to be parsed and stored. Reads from file FLAT_RCL.txt 
 *                   writes to file output.txt
 * intput--
 * int type:         type of file read: 1= setup file read 0= child process read writes to file
 * 
 * return:           void
 */
void readWriteFile(){
    FILE  *fileStream;
    size_t readLineStat, lineLength = 0;    
    char  *tokenLines= NULL;    
          
        fileStream=fopen("FLAT_RCL.txt","r");
        if ( fileStream == NULL ) {
            perror("Error: "); 
	    printf("Press any key to exit...\n");
            exit(1);
        }
        while ((readLineStat = getline (&tokenLines, &lineLength, fileStream)) != -1){
           delimiter(tokenLines,tokenIndex++);
        }    
}

/**
 * Description:     Helper method to call bubble sort. First by make then by year
 * input--
 * int i:           Starting index of sort
 * int j:           Ending index of sort 
 * 
 * return:	     void
 */
void* getSortedCars(void* index) {
    long x=(long)index;
    int i=(int)x*tasksPerProcess;
    int j=i+tasksPerProcess;
    bubbleSort(i,j,1);    //Sort by make
    bubbleSort(i,j,0);    //Sort by year
}

/**
 * Description:              Gets the time that a process took
 * input--
 * char *c_time_string:      Fortmatted time string using ctime()
 * time_t current_time:	      Time stamp form process start 		      
 * 
 * return:                   int EXIT_FAILURE, EXIT_SUCCESS
 */
int getTime( char *cTimeString, time_t currentTime ){
    
    if ((currentTime == ((time_t)-1)) || (cTimeString == NULL)) {
        (void) fprintf(stderr, "\nCan't Compute time getTime() method");
        return EXIT_FAILURE;
        
    }else{
        printf("\nCurrent time is -->  %s\n", cTimeString); 
        return EXIT_SUCCESS;
    }
}

/**
 * Description:               Merges arrays together. Used after each thread is done bubblesorting
 * input--
 * struct Cars *sorted:       The final Sorted array of parts A and B
 * struct Cars *a:            The first array under merge
 * struct Cars *b:	       The second array under merge
 * int m:		       Size of Array A
 * int n:		       Size of Array B 
 * 
 * return:   		       void
 */
void mergeSorted(struct Cars *sortedCars, struct Cars *first,struct Cars *second,int w, int x,int y,int z){
    int n=z-y;
    int m= x-w;
    int i, j, k; 
    int mergeSize = (m+n);
    k=y;
    j=w;
    for (i = 0; i < mergeSize; ){
        if (j < x && k < z) {
            if (first[j].year < second[k].year) {
                sortedCars[i] = first[j];
                j++;
            }else {
                sortedCars[i] = second[k];
                k++;
            }i++;
            
        }else if (j == x) {
            for (; i < mergeSize;) {
                sortedCars[i] = second[k];
                k++; 
                i++;
            }
            
        }else {
            for (; i < mergeSize;) {
                sortedCars[i] = first[j];
                j++; 
                i++;
            }
        }
    }

   
}

/**
 * Description:               Runs the sort in a multithreaded process. given number of 
			      processes to run
 * input--
 * int numThreads	      Number of threads to be processed
 * 
 * return:   		       void
 */
void threadStart(int numThreads){
   
long index;   
pthread_t *currentThread;
    
   
  
    struct Cars tempFirst[TOTAL_CARS/2];   
    struct Cars tempSecond[TOTAL_CARS/2];

    struct Cars mergeFirst[TOTAL_CARS/5];
    struct Cars mergeSecond[TOTAL_CARS/5];   
    struct Cars mergeThird[TOTAL_CARS/5];
    struct Cars mergeFourth[TOTAL_CARS/5];
    struct Cars mergeFifth[TOTAL_CARS/5];
   
   //for second merge
    static struct Cars Amerge[TOTAL_CARS/2];    
    static struct Cars Bmerge[TOTAL_CARS/2];
   
    static struct Cars Cmerge[TOTAL_CARS];//had to make static. With the additional it this size of an array over flowed the stack
   //create memory for the threads
   
   currentThread=malloc(numThreads*sizeof(currentThread));
   tasksPerProcess=(TOTAL_CARS+(numThreads-1))/numThreads;
   
   //Create threads
   for(index=0;index<numThreads;index++){
       pthread_create(&currentThread[index],NULL,getSortedCars,(void*)index);
   }

   //Wait on the threads
   for(index=0;index<numThreads;index++){
       pthread_join(currentThread[index],NULL);
   }
   //Merge threads
   switch(numThreads){
     case 2:mergeSorted(sorted_carlist,listOfCars,listOfCars,0,tasksPerProcess,tasksPerProcess,TOTAL_CARS);
     break;
     case 4:
mergeSorted(tempFirst,listOfCars,listOfCars,0,tasksPerProcess,tasksPerProcess,tasksPerProcess*2);
mergeSorted(tempSecond,listOfCars,listOfCars,tasksPerProcess*2,tasksPerProcess*3,tasksPerProcess*3,TOTAL_CARS);

mergeSorted(sorted_carlist,tempFirst,tempSecond,0,tasksPerProcess*2,0,tasksPerProcess*2);
     break;


     case 10: 
mergeSorted(mergeFirst,listOfCars,listOfCars,0,tasksPerProcess,tasksPerProcess,tasksPerProcess*2);
mergeSorted(mergeSecond,listOfCars,listOfCars,tasksPerProcess*2,tasksPerProcess*3,tasksPerProcess*3,tasksPerProcess*4);
mergeSorted(mergeThird,listOfCars,listOfCars,tasksPerProcess*4,tasksPerProcess*5,tasksPerProcess*5,tasksPerProcess*6);
mergeSorted(mergeFourth,listOfCars,listOfCars,tasksPerProcess*6,tasksPerProcess*7,tasksPerProcess*7,tasksPerProcess*8);
mergeSorted(mergeFifth,listOfCars,listOfCars,tasksPerProcess*8,tasksPerProcess*9,tasksPerProcess*9,TOTAL_CARS);
mergeSorted(Amerge,mergeFirst,mergeSecond,0,tasksPerProcess*2,0,tasksPerProcess*2);
mergeSorted(Bmerge,mergeThird,mergeFourth,0,tasksPerProcess*2,0,tasksPerProcess*2);
mergeSorted(Cmerge,Amerge,Bmerge,0,tasksPerProcess*4,0,tasksPerProcess*4);
mergeSorted(sorted_carlist,Cmerge,mergeFifth,0,tasksPerProcess*8,0,tasksPerProcess*2);
     break;
  }
//print the data that has been sorted.
    

     FILE *openFile=fopen(FILENAME,"w");
     
     if(openFile==NULL){
       fprintf(openFile,"    Can't open file sorted.txt\n");
       exit(1);
     }
printf("Test8\n");
     if(numThreads==1){
        for(index=0;index<TOTAL_CARS;index++){
        fprintf(openFile, "Year: %d Make: %s ID: %s\n",listOfCars[index].year, listOfCars[index].make, listOfCars[index].recallID);
        
       }
     }else{
         for(index=0;index<TOTAL_CARS;index++){
            if(sorted_carlist[index].year>0){
            fprintf(openFile, "Year: %d Make: %s ID: %s\n",sorted_carlist[index].year,sorted_carlist[index].make,listOfCars[index].recallID);
	    }  
         }

     }
}


void menu(void){
   int    numThreads;
   char * c_time_string;//string to hold time
   time_t current_time = time(NULL); //get current time stamp     
   c_time_string = ctime(&current_time);  	
   time_t initialTime;
   time_t finalTime;
   
   printf("/////////////////////////////////////////////////////////////////");
   getTime( c_time_string, current_time );
   printf("/////////////////////////////////////////////////////////////////\n\n");
    
    
    //Prompt user for input
    printf("--------Provide How many threads you would like to run-------\n");
    printf(" Number of threads to choose from: 1,2,4,10\n");
    scanf("%d", &numThreads);
    
    if(numThreads>=11){
       printf("More than 10 threads are not aloud");
    }else{
       initialTime=time(NULL);//Start timer
       threadStart(numThreads);
       finalTime=time(NULL);
       printf("<<------------------------------->>\n");
       printf("Total time taken: %d seconds\n",(int)(finalTime-initialTime));
       
    }   
}


