/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 2 Multi-process 1,2,4,10
 * Due Date: 3/1/2015
 * Header Link for Lab2.c
 *******************************/

/********************************************
 * Project Description: 
 * Create processes, run processes in parallel, and pass information between processes. Data is read from FLAT_RCL.txt
 * Use bubble sort to sort the data. Time each type of process. Create a program that will run multiple processes
 * concurrently using fork() and exec(). Do this for 1 2 4 and 10 processes A menu will be implemented to selected which
 * qty of processes. 
 * Citation: Linux MAN pages, Professor Levine's Original Code given for lab 1, Various snippets from Stack overflow/ dreamInCode
 * for Fork() sizing of Arrays includes and defines.
 ********************************************/
 
 
/**************************HEADERS********************************************/
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "NidellLab2Func.h"
#include <sys/types.h>
#include <stdio.h>
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
void delimiter(char *lines, int i, int type){
    int   delta=0;//keeps track of where we are in the readline.
    char *delimitedString;
    
    if (type == 1){        
        delimitedString=strtok(lines,"\t");//delimter
        while (delimitedString != NULL) {
            if(delta==2) strcpy(listOfCars[i].make,delimitedString);
            if(delta==4) listOfCars[i].year=atoi(delimitedString);//convert to int
            delimitedString = strtok(NULL,"\t");
            delta++;
        }
    }
    else{
        delimitedString=strtok(lines," ");
        while (delimitedString != NULL) {
            if(delta==3) strcpy(listOfCarsCopy[i].make,delimitedString);
            if(delta==1) listOfCarsCopy[i].year=atoi(delimitedString);
            delimitedString = strtok(NULL," ");
            delta++;
        }
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
void readWriteFile( int type ){
    FILE  *fileStream;
    size_t readLineStat, lineLength = 0;
    int i = 0;
    char  *tokenLines= NULL;

    printf("Doing Data Management\n");
    if (type == 1){
        fileStream = fopen("FLAT_RCL.txt", "r");

        if ( fileStream == NULL ) {
            perror("Error: "); printf("Press any key to exit...\n");
            exit(1);
        }
        while ((readLineStat = getline (&tokenLines, &lineLength, fileStream)) != -1){
           delimiter(tokenLines, tokenIndex++, type);
        }
    }
    else{
        fileStream = fopen(FILENAME, "r");
        if ( fileStream == NULL ) {
            perror("Error: "); printf("Press any key to exit...\n");
            exit(1);
        }
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
void getSortedCars(int x, int y) {
    bubbleSort(x,y,1);    //Sort by make
    bubbleSort(x,y,0);    //Sort by year
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
 * Description:               Merges arrays together. Used after each process is done bubblesorting
 * input--
 * struct Cars *sorted:       The final Sorted array of parts A and B
 * struct Cars *a:            The first array under merge
 * struct Cars *b:	       The second array under merge
 * int m:		       Size of Array A
 * int n:		       Size of Array B
 * 
 * return:   		       void
 */
void mergeSorted(struct Cars *sortedCars,struct Cars *first,struct Cars *second, int x,int y ){
    int i, j, k; 
    j = k = 0;
    int mergeSize = (x+y);
     
    for (i = 0; i < mergeSize; ){
        if (j < x && k < y) {
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
                k++; i++;
            }
            
        }else {
            for (; i < mergeSize;) {
                sortedCars[i] = first[j];
                j++; i++;
            }
        }
    }
}

/**
 * Description:           Splits data into two processes and merges the sorted list
 * input--
 * none
 * return:                void 
 */

void read2Processes(void){
    
    int k, currentCount, size1, size2; 
    size1 = size2 = currentCount = 0;
    struct Cars *partition1  = malloc(sizeof(Cars) * (TOTAL_CARS/2));
    struct Cars *partition2  = malloc(sizeof(Cars) * (TOTAL_CARS/2));
    
    readWriteFile(0);    
    
    while (currentCount < TOTAL_CARS) { 
        if (currentCount < HALF){
            partition1[size1].year = listOfCarsCopy[currentCount].year;
            strcpy(partition1[size1].make, listOfCarsCopy[currentCount].make); size1++;
            
        }else{                  // Stores the 2nd data partition collected from the file
            partition2[size2].year = listOfCarsCopy[currentCount].year;
            strcpy(partition2[size2].make, listOfCarsCopy[currentCount].make); size2++;
        }currentCount++;
    }

    // Merging the processes at a range of n/2
    mergeSorted(sorted_carlist, partition1, partition2, size1, size2);

    // Free memory
    free( sorted_carlist ); free( partition1 ); free( partition2 );
}

/**
 * Description:           Splits data into four processes and merges the sorted list
 * input--
 * none
 * return:                void 
 */

void read4Processes(){    
    int  size1, size2, size3, size4, k, currentCount;
    int  mergeSize1, mergeSize2, finalMergeSize;
    
    struct Cars *partition1  = malloc(sizeof(Cars) * (TOTAL_CARS/FOUR_PROCESS));
    struct Cars *partition2  = malloc(sizeof(Cars) * (TOTAL_CARS/FOUR_PROCESS));
    struct Cars *partition3  = malloc(sizeof(Cars) * (TOTAL_CARS/FOUR_PROCESS));
    struct Cars *partition4  = malloc(sizeof(Cars) * (TOTAL_CARS/FOUR_PROCESS));
    struct Cars *merge1st, *merge2nd;
    
    size1 = size2 = size3 = size4 = currentCount = 0;
    
    readWriteFile(0);
    
    while (currentCount < TOTAL_CARS) {//create partitions with seperate data        
        if (currentCount < HALF/FOUR_PROCESS){
            partition1[size1].year = listOfCarsCopy[currentCount].year;
            strcpy(partition1[size1].make, listOfCarsCopy[currentCount].make); size1++;            
        }else if (currentCount >= (TOTAL_CARS/FOUR_PROCESS) && currentCount < (TOTAL_CARS/2)){
            partition2[size2].year = listOfCarsCopy[currentCount].year;
            strcpy(partition2[size2].make, listOfCarsCopy[currentCount].make); size2++;
        }else if (currentCount >= (TOTAL_CARS/2) && currentCount < (3*(TOTAL_CARS/FOUR_PROCESS))){
            partition3[size3].year = listOfCarsCopy[currentCount].year;
            strcpy(partition3[size3].make, listOfCarsCopy[currentCount].make); size3++;            
        }else if (currentCount >= (3*(TOTAL_CARS/FOUR_PROCESS)) && currentCount < TOTAL_CARS){
            partition4[size4].year = listOfCarsCopy[currentCount].year;
            strcpy(partition4[size4].make, listOfCarsCopy[currentCount].make); size4++;            
        }
        currentCount++;
    }

    //calculate the sizes for each array
    mergeSize1 = (size1 + size2);
    mergeSize2 = (size3 + size4);
    finalMergeSize = mergeSize1 + mergeSize2;
    //calculate the sizes for each array
    merge1st   = malloc((mergeSize1)*sizeof(struct Cars));
    merge2nd   = malloc((mergeSize2)*sizeof(struct Cars));
    //Merge four to two
    mergeSorted(merge1st,partition1, partition2, size1, size2);
    mergeSorted(merge2nd,partition3, partition4, size3, size4);
    //merge 2 to 1
    mergeSorted(sorted_carlist, merge1st, merge2nd, mergeSize1, mergeSize2);    
 
    free(merge1st); free(merge2nd); free(sorted_carlist);
    
}

/**
 * Description:           Splits data into 10 processes and merges the sorted list
 * input--
 * none
 * return:                void 
 */
void read10Processes(){
    int size0,    size1,     size2,   size3,   size4, sizeA;
    int size5,    size6,     size7,   size8,   size9, sizeB;
    int size1st,  size2nd,   size3rd, sizeC,   k, currentCount=0;
    int size4th,  size5th,   sizeFinal;
    
    struct Cars *merge1st,   *merge2nd, *merge3rd, *merge4th, *merge5th;
    struct Cars *mergeA,     *mergeB,   *mergeC;
    printf("allocating memory for Cars\n");
    struct Cars *partition0  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition1  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition2  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition3  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition4  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition5  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition6  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition7  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition8  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
    struct Cars *partition9  = malloc(sizeof(Cars) * (TOTAL_CARS/TEN_PROCESS));
   
    
    size0 = size1 = size2 = size3 = size4 = sizeA = 0;
    size5 = size6 = size7 = size8 = size9 = sizeB = 0;
  
    readWriteFile(0);
    printf("Reading 10 processes\n");
    while (currentCount < TOTAL_CARS) { //create partitions with seperate data
        
        if (currentCount < HALF/TEN_PROCESS){
            partition0[size0].year = listOfCarsCopy[currentCount].year;
            strcpy(partition0[size0].make, listOfCarsCopy[currentCount].make); size0++;
            
        }else if (currentCount >= (1*(TOTAL_CARS/TEN_PROCESS))&&currentCount < (2*(TOTAL_CARS/TEN_PROCESS))){
            partition1[size1].year = listOfCarsCopy[currentCount].year;
            strcpy(partition1[size1].make, listOfCarsCopy[currentCount].make); size1++;
            
        }else if ((currentCount >= (2*(TOTAL_CARS/TEN_PROCESS)))&&(currentCount < (3*(TOTAL_CARS/TEN_PROCESS)))){
            partition2[size2].year = listOfCarsCopy[currentCount].year;
            strcpy(partition2[size2].make, listOfCarsCopy[currentCount].make); size2++;
            
        }else if ((currentCount >= (3*(TOTAL_CARS/TEN_PROCESS)))&&(currentCount < (4*(TOTAL_CARS/TEN_PROCESS))) ){
            partition3[size3].year = listOfCarsCopy[currentCount].year;
            strcpy(partition3[size3].make, listOfCarsCopy[currentCount].make); size3++;
            
        }else if ((currentCount >= (4*(TOTAL_CARS/TEN_PROCESS)))&&(currentCount < (5*(TOTAL_CARS/TEN_PROCESS))) ){
            partition4[size4].year = listOfCarsCopy[currentCount].year;
            strcpy(partition4[size4].make, listOfCarsCopy[currentCount].make); size4++;
            
        }else if ((currentCount >= (5*(TOTAL_CARS/TEN_PROCESS)))&&(currentCount < (6*(TOTAL_CARS/TEN_PROCESS))) ){
            partition5[size5].year = listOfCarsCopy[currentCount].year;
            strcpy(partition5[size5].make, listOfCarsCopy[currentCount].make); size5++;
            
        }else if ((currentCount >= (6*(TOTAL_CARS/TEN_PROCESS)))&&(currentCount < (7*(TOTAL_CARS/TEN_PROCESS))) ){
            partition6[size6].year = listOfCarsCopy[currentCount].year;
            strcpy(partition6[size6].make, listOfCarsCopy[currentCount].make); size6++;
            
        }else if ((currentCount >= (7*(TOTAL_CARS/TEN_PROCESS)))&&(currentCount < (8*(TOTAL_CARS/TEN_PROCESS))) ){
            partition7[size7].year = listOfCarsCopy[currentCount].year;
            strcpy(partition7[size7].make, listOfCarsCopy[currentCount].make); size7++;
            
        }else if ((currentCount >= (8*(TOTAL_CARS/10)))&&(currentCount < (9*(TOTAL_CARS/10))) ){
            partition8[size8].year = listOfCarsCopy[currentCount].year;
            strcpy(partition8[size8].make, listOfCarsCopy[currentCount].make); size8++;
            
        }else if ((currentCount >= (9*(TOTAL_CARS/10)))&&(currentCount < TOTAL_CARS)){
            partition9[size9].year = listOfCarsCopy[currentCount].year;
            strcpy(partition9[size9].make, listOfCarsCopy[currentCount].make); size9++;
        }
        currentCount++;
    }  
   printf("Calculating array Size\n");
   //calculate the sizes for each array
   size1st = (size0 + size1);
   size2nd = (size2 + size3);
   size3rd = (size4 + size5);
   size4th = (size6 + size7);
   size5th = (size8 + size9);
   //calculate the sizes for each array
   sizeA = (size1st + size2nd);
   sizeB = (size3rd + size4th);
   sizeC = (sizeA + sizeB);
   sizeFinal = (sizeC + size5th);
   //setup merging arrays
    merge1st= malloc((size1st)*sizeof(struct Cars));
    merge2nd= malloc((size2nd)*sizeof(struct Cars));
    merge3rd= malloc((size3rd)*sizeof(struct Cars));
    merge4th= malloc((size4th)*sizeof(struct Cars));
    merge5th= malloc((size5th)*sizeof(struct Cars));
    mergeA  = malloc((sizeA)*sizeof(struct Cars));
    mergeB  = malloc((sizeB)*sizeof(struct Cars));
    mergeC  = malloc((sizeA+sizeB)* sizeof(struct Cars));
    printf("Merging Array\n");
    //Start merging 10 to 5
   mergeSorted(merge1st,partition0,partition1,size0,size1);
   mergeSorted(merge2nd,partition2,partition3,size2,size3);
   mergeSorted(merge3rd,partition4,partition5,size4,size5);
   mergeSorted(merge4th,partition6,partition7,size6,size7);
   mergeSorted(merge5th,partition8,partition9,size8,size9);
   //Merge from 5 to 3
   mergeSorted(mergeA,merge1st,merge2nd,size1st,size2nd);
   mergeSorted(mergeB,merge3rd,merge4th,size3rd,size4th);
   //Merge 3 to 2
   mergeSorted(mergeC,mergeA,mergeB,sizeA,sizeB);
   //merge 2 to complete sorted list.
   mergeSorted(sorted_carlist,mergeC,merge5th,sizeC,size5th);
    
   // Free memory
   free(merge1st);
   free(merge2nd);
   free(merge3rd);
   free(merge4th);
   free(merge5th);
   free(sorted_carlist);
   free(mergeA);
   free(mergeB);
   free(mergeC);
}

/**
 * Description:           No processes created. Just sort
 * input--
 * none
 * return:                void 
 */
void oneProcesses()
{
    int i =0;
    FILE *dataFile=fopen( FILENAME, "a+");
    tasksPerProcess=(TOTAL_CARS+(2-1))/2;
    
    getSortedCars(0,TOTAL_CARS);
    for (i=0; i<TOTAL_CARS; i++) {
          fprintf(dataFile, "Year: %d Make: %s\n",listOfCars[i].year,listOfCars[i].make);
    }
    fclose(dataFile);    
}

/**
 * Description:           2 processes read from listOfCars and writes it to a file output.txt
 * input--
 * none
 * return:                void 
 */
void twoProcesses (void){
    int    i = 0;
    FILE   *dataFile = fopen( FILENAME, "a+");
    pid_t  child1, child2;
    tasksPerProcess = (TOTAL_CARS + (2-1))/2;

    
    
    if ( dataFile == NULL ) {//If file isn't there or already open
        fprintf(dataFile, "......Problem with file--> output.txt file\n\n");
        exit(1);
    }    
    if ( (child1 = fork()) != 0 ) {//create child 1
        // Parent Process
        
        if ( (child2 = fork()) != 0 ){//create child 2
            // Parent Process            
        }else {            
            getSortedCars(tasksPerProcess, (TOTAL_CARS) );
            for (i=tasksPerProcess; i<TOTAL_CARS; i++) {//Task for Child2
                fprintf(dataFile, "Year: %d Make: %s\n",
                        listOfCars[i].year,
                        listOfCars[i].make);
            }exit(0);
        }        
    }else {        
        getSortedCars( 0, tasksPerProcess );
        for (i=0; i<tasksPerProcess; i++) {//task for Child1
            fprintf(dataFile, "Year: %d Make: %s\n",
                    listOfCars[i].year,
                    listOfCars[i].make);
        }exit(0);
    }
    wait(&child1);
    wait(&child2);
    fclose(dataFile);
    read2Processes();   
}

/**
 * Description:           4 processes read from listOfCars and writes it to a file output.txt
 * input--
 * none
 * return:                void 
 */
void fourProcesses(void){
    FILE   *dataFile = fopen( FILENAME, "a+");
    pid_t  child1, child2, child3, child4;
    int    i = 0;
    tasksPerProcess = (TOTAL_CARS + (FOUR_PROCESS-1))/FOUR_PROCESS;   
     
     if ((child1 = fork()) == 0) {//create child1
         getSortedCars( 0, tasksPerProcess );
         for (i=0; i<tasksPerProcess; i++) {
             fprintf(dataFile, "Year: %d Make: %s\n",
                     listOfCars[i].year,
                     listOfCars[i].make);
         } exit(0);
         
     }else {
         if ((child2 = fork()) == 0){//create child2
             getSortedCars( tasksPerProcess, 2*tasksPerProcess );
             for (i=tasksPerProcess; i<2*tasksPerProcess; i++) {
                 fprintf(dataFile, "Year: %d Make: %s\n",
                         listOfCars[i].year,
                         listOfCars[i].make);
             } exit(0);
             
         }else {
             if ((child3 = fork()) == 0) {//create child3
                 getSortedCars( 2*tasksPerProcess, 3*tasksPerProcess );
                 for (i=2*tasksPerProcess; i<3*(TOTAL_CARS/FOUR_PROCESS); i++) {
                     fprintf(dataFile, "Year: %d Make: %s\n",
                             listOfCars[i].year,
                             listOfCars[i].make);
                 }exit(0);
                 
             }else {
                 if ((child4 = fork()) == 0) {//create child4
                     getSortedCars( 3*tasksPerProcess, TOTAL_CARS );
                     for (i=3*tasksPerProcess; i<TOTAL_CARS; i++) {
                         fprintf(dataFile, "Year: %d Make: %s\n",
                                 listOfCars[i].year,
                                 listOfCars[i].make);
                     }exit(0);
                     
                 }else {//I'm the Parent
                     wait(&child1);
                     wait(&child2);
                     wait(&child3);
                     wait(&child4);
                     fclose(dataFile);
                     read4Processes(); 
                 }
             }
         }
     }
     printf("\n\n");
}

/**
 * Description:           10 processes read from listOfCars and writes it to a file output.txt
 * input--
 * none
 * return:                void 
 */
void tenProcesses(void){
    FILE   *dataFile = fopen( FILENAME, "a+");
    pid_t  child1, child2, child3, child4, child5;
    pid_t  child6, child7, child8, child9, child10;
    int    i = 0;
    tasksPerProcess = (TOTAL_CARS + (TEN_PROCESS-1))/TEN_PROCESS;
    
    if ( (child1 = fork()) == 0 ) {//Child1
        getSortedCars( 0, tasksPerProcess );
        for (i=0; i<tasksPerProcess; i++) {
            fprintf(dataFile, "Year: %d Make: %s\n",
                    listOfCars[i].year,
                    listOfCars[i].make);
        }exit(0);
        
    }else {
        if ( (child2 = fork()) == 0 ) {//Child2
            getSortedCars( tasksPerProcess, 2*tasksPerProcess );
            for (i=tasksPerProcess; i<2*tasksPerProcess; i++) {
                fprintf(dataFile, "Year: %d Make: %s\n",
                        listOfCars[i].year, listOfCars[i].make);
            }exit(0);
            
        }else {
            if ( (child3 = fork()) == 0 ) {//Child3
                getSortedCars(2*tasksPerProcess, 3*tasksPerProcess );
                for (i=2*tasksPerProcess; i<(3*tasksPerProcess); i++) {
                    fprintf(dataFile, "Year: %d Make: %s\n",
                            listOfCars[i].year,
                            listOfCars[i].make);
                }exit(0);
            }else {
                if ( (child4 = fork()) == 0 ) {//Child4
                    getSortedCars( (3*tasksPerProcess), 4*tasksPerProcess );
                    for (i=(3*tasksPerProcess); i<(4*tasksPerProcess); i++) {
                        fprintf(dataFile, "Year: %d Make: %s\n",
                                listOfCars[i].year, listOfCars[i].make);
                    }exit(0);
                }else {
                    if ( (child5 = fork()) == 0 ) {//Child5
                        getSortedCars( (4*tasksPerProcess), 5*tasksPerProcess );
                        for (i=(4*tasksPerProcess); i<(5*tasksPerProcess); i++) {
                            fprintf(dataFile, "Year: %d Make: %s\n",
                                    listOfCars[i].year,
                                    listOfCars[i].make);
                        }exit(0);
                        
                    }else {
                        if ( (child6 = fork()) == 0 ) {//Child6
                            getSortedCars( (5*tasksPerProcess), 6*tasksPerProcess );
                            for (i=(5*tasksPerProcess); i<(6*tasksPerProcess); i++) {
                                fprintf(dataFile, "Year: %d Make: %s\n",
                                        listOfCars[i].year,
                                        listOfCars[i].make);
                            }exit(0);
                            
                        }else {
                            if ( (child7 = fork()) == 0 ) {//Child7
                                getSortedCars( 6*tasksPerProcess, 7*tasksPerProcess );
                                for (i=(6*tasksPerProcess); i<(7*tasksPerProcess); i++) {
                                    fprintf(dataFile, "Year: %d Make: %s\n",
                                            listOfCars[i].year,
                                            listOfCars[i].make);
                                }exit(0);
                                
                            }else {
                                if ( (child8 = fork()) == 0 ) {//Child8
                                    getSortedCars( 7*tasksPerProcess, 8*tasksPerProcess );
                                    for (i=(7*tasksPerProcess); i<(8*tasksPerProcess); i++) {
                                        fprintf(dataFile, "Year: %d Make: %s\n",
                                                listOfCars[i].year,
                                                listOfCars[i].make);
                                    }exit(0);
                                    
                                }else {
                                    if ( (child9 = fork()) == 0 ) {//Child9
                                        getSortedCars( 8*tasksPerProcess, 9*tasksPerProcess );
                                        for (i=(8*tasksPerProcess); i<(9*tasksPerProcess); i++) {
                                            fprintf(dataFile, "Year: %d Make: %s\n",
                                                    listOfCars[i].year,
                                                    listOfCars[i].make);
                                        }exit(0);
                                        
                                    }else {
                                        if ( (child10 = fork()) == 0 ) {//Child10
                                            getSortedCars( 9*tasksPerProcess, (TOTAL_CARS) );
                                            for (i=(9*tasksPerProcess); i<(TOTAL_CARS); i++) {
                                                fprintf(dataFile, "Year: %d Make: %s\n",
                                                        listOfCars[i].year,
                                                        listOfCars[i].make);
                                            }exit(0);
                                            
                                        }else {//parent needs to wait for children to finish
                                            
                                            wait(&child1);
                                            wait(&child2);
                                            wait(&child3);
                                            wait(&child4);
                                            wait(&child5);
                                            wait(&child6);
                                            wait(&child7);
                                            wait(&child8);
                                            wait(&child9);
                                            wait(&child10);
                                            fclose(dataFile);
						                    printf("done with 10 process....\n");
                                            read10Processes(); 
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * Description:        Selection menu for how many processes to run.
 * input--
 * none
 * output:              void 
 */
void menu(void){
   int    numProcess;
   char * c_time_string;//string to hold time
   time_t current_time = time(NULL); //get current time stamp     
   c_time_string = ctime(&current_time);  
   time_t initialTime;
   time_t finalTime;
   
   
   
   printf("/////////////////////////////////////////////////////////////////");
   getTime( c_time_string, current_time );
   printf("/////////////////////////////////////////////////////////////////\n\n");
    
    
    //Prompt user for input
    printf("--------Provide How many processes you would like to run-------\n");
    printf(" Number of processes to choose from: 1,2,4,10\n");
    scanf("%d", &numProcess);
    
    switch(numProcess){
        case 1://run 1 process
            initialTime = time(NULL);
            printf("\n");
            printf("Running 1 Process....\n");
            oneProcesses();
            finalTime = time(NULL);
            printf("----------------------------------------------------\n");
            printf(">>>>>>> The total  time is %d seconds for one process<<<<<<<<<<\n",
                   (int)(finalTime - initialTime));
            break;
        case 2://run two processes
            initialTime = time(NULL);
            printf("\n");
            printf("Running 2 Processes....\n");
            twoProcesses();
            finalTime = time(NULL);
            printf("----------------------------------------------------\n");
            printf(">>>>>>> The total  time is %d seconds for two process<<<<<<<<<<\n",
                  (int)(finalTime - initialTime));
            break;
            
        case 4://run four processes
            initialTime = time(NULL);
            printf("\n");
            printf("Running 4 Processes....\n");	    
            fourProcesses();
            finalTime = time(NULL);
            printf("----------------------------------------------------\n");
            printf(">>>>>>> The total  time is %d seconds for four process<<<<<<<<<<\n",
                  (int)(finalTime - initialTime));
            break;
            
        case 10://run ten processes
            initialTime = time(NULL);
            printf("\n");
            printf("Running 10 Processes....\n");
            tenProcesses();
            finalTime = time(NULL);
            printf("----------------------------------------------------\n");
            printf(">>>>>>> The total  time is %d seconds for ten process<<<<<<<<<<\n",
                  (int)(finalTime - initialTime));
            break;
    }
    printf("\n\n");
}


