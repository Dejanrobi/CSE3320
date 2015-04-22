/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 4 A Simple File System
 * Due Date: 3/1/2015
 *******************************/

/********************************************
 * Project Description: 
 * Develop and implement a small Unix-CPM/M like file system ("FS"). Your file sytem will not be part of an operating system,
 * but, similar to modern file systems. It will run in several different operating systems to procide a "portable" file system
 * The FS will use a file (for example "disk01"), rather than a physical flash or disk to store data. The data stored in disk01 will be
 * user programs, text files, other data files, or any type of binary information. IN addition to the data stored, your FS will need to 
 * store other meta-information, such as free space (blocks), directory details, and possibly other information. The FS directory
 * is flat (one level) fixed size, and has fixed size entries. You should use fixed size blocks *similar to disk blocks) of size 256 bytes
 * to store Files and all meta-data in your "disk".
 * Compile with command: gcc -o Lab4 Lab4Func.c Lab4Func.h Lab4.c
 * 
 * Features to be implemented.
 * 1.) Create File System -- # of blocks, 
 * 2.) Format files system -- #filenames #ABPTentries
 * 3.) Save file system  -- save disk by name
 * 4.) Open file system -- use an existing disk image
 * 5.) Remove file -- removes file from file system
 * 6.) Put -- puts external file OS file into disk
 * 7.) Get -- copy file form FS to OS
 * 8.) Link/Unlink -- Unix style file linking
 * 9.) Set Permissions -- Unix style r/w/x permissions
 *
 ********************************************/

/**************************HEADERS********************************************/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Lab4Func.h"
#include <string.h> 
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
//both needed for file statistics.
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

//needed for strcpy
/**************************CONSTANTS******************************************/

/***************************STRUCTS--DECLARATIONS*****************************/
struct FileSystem myFS;
struct BlockPointerTable myABPT;
struct FileNameTable myFNT;

/**
 * Description: Provides entrance menu for the file system program
 * input--
 * none
 * return:      void
 */
void menu(){
  int select=-1;
  char fileSystemName[FILENAMESIZE];
  char saveNameFS[FILENAMESIZE];
  char fileName[FILENAMESIZE];
  
  
  while(TRUE){
    printf("0:  Exit\n");
    printf("1:  Create File System\n");
    printf("2:  Format File System\n");
    printf("3:  Save File System\n");
    printf("4:  Open File System\n");
    printf("5:  List Files in System\n");
    printf("6:  Remove File from File System\n");
    printf("7:  Put File-- Host File to File System\n");
    printf("8:  Get File-- copy FS File to OS\n");
    printf("9:  Add Link--Create shortcut to file\n");
    printf("10: Remove Link-- remove shortcut to file\n");
    printf("11: Set permissions on file\n");
    scanf("%d",&select);
    switch(select)
    {      
      case 0:
	exit(0);
      case 1:
	printf("How many data blocks in the system\n");
	int numberOfBlocks;
	scanf("%d",&numberOfBlocks);	
	createFileSystem(numberOfBlocks);
	printf("Disk created\n");
	break;
      case 2:
	printf("Formatting file system..\n");	
	printf("Enter name of file system:\n");
	scanf("%s",fileSystemName);
	printf("Number of Block Pointers\n");
	int numBlockPointers;
	int numFiles;
	scanf("%d",&numBlockPointers);
	printf("Number of Files\n");
	scanf("%d",&numFiles);
	formatFileSystem(fileSystemName,numBlockPointers,numFiles);
	printf("Disk formatted\n");
	break;
      case 3:
	printf("Provide the name you want the file System to be saved as\n");
	scanf("%s",saveNameFS);
	saveFileSystem(saveNameFS);
	break;
      case 4:
	printf("Provide name of fileSystem you want to open\n");
	scanf("%s",saveNameFS);
	openFileSystem(saveNameFS);
	break;
      case 5:
	printf("Listing Files...\n");
	listFiles();
	break;
      case 6:
	printf("What File do you want to remove?\n");	
	scanf("%s",fileName);
	removeFile(fileName);
	break;
      case 7:
	listFilesInDir();
	printf("What File do you want to put to the File System\n");
	scanf("%s",fileName);
        putFile(fileName);
	break;
      case 8:
	printf("What File do you want to move to Host OS\n");
	scanf("%s",fileName);
	getFile(fileName);
	break;
      case 9:
	printf("Provide File Name to link to\n");
	scanf("%s",fileName);
	printf("What is the name of the link?\n");
	char linkName[FILENAMESIZE];
	scanf("%s",linkName);
	createLink(fileName,linkName);
	break;
      case 10:
	break;
      case 11:
	printf("Name file to change permission\n");
	scanf("%s",fileName);
	printf("File Size is: %d\n",getFileSize(fileName));
	printf("File Mod date:%s\n",getModDate(fileName));
	writeData(fileName,0);
	break;

    }
  }
}
/**
 * Description: intiaites the create of a file system build data blocks
 * input--
 * numBlocks:   number of data blocks being created
 * 
 * return:      void
 */
void createFileSystem(int numBlocks){
  int i;
  int j;
  char **dataHolder;
  printf("%d DATABLOCKS SELECTED\n",numBlocks);
  myFS.numBlocks=numBlocks;
  //allocate memory rowsize 256 char
  dataHolder=(char**) malloc(BLOCKSIZE*sizeof(char*));
  for(i=0;i<numBlocks;i++){
   dataHolder[i]=(char*)malloc(numBlocks*sizeof(char));    
  } 
  myFS.dataBlockMap= malloc(numBlocks*sizeof(int*));
  myFS.dataBlocks=dataHolder;
  //i number of data blocks. j size of datablocks
  //,yFS.dataBlocks[i][j]
  
}

/**
 * Description:         formats a filesystem add name and allocates pointer Entries
 * input--
 * name:                name of File System
 * numPointerEntries:   number of ABPT entries
 * numFNT:              number of files in system
 * 
 * return:              void
 */
void formatFileSystem(char name[FILENAMESIZE],int numPointerEntries,int numFNT){
  //struct FileNameTable* myFT=malloc(numFNT*sizeof(struct FileNameTable);
  printf("Creating FS with..\n name:%s\n #pointers: %d\n#files:%d",name,numPointerEntries,numFNT);
  strcpy(myFS.fileSystemName,name);
  myFS.numFNT=numFNT;
  myFS.numABPT=numPointerEntries;
  
  myFS.files=malloc(numFNT*sizeof(struct FileNameTable));
  myFS.pointerTables=malloc(numPointerEntries*sizeof(struct BlockPointerTable));  
  myFS.FNTMap= malloc(numFNT*sizeof(int*));
  myFS.ABPTMap= malloc(numPointerEntries*sizeof(int*));
  
  
}

/**
 * Description:   saves a File System to a file locally.
 * input--
 * nameOfSave:    name of the File that saves the file System
 * 
 * return:        void
 */
void saveFileSystem(char *nameOfSave){//not finished should do set and get first
  /*
  int i,j;
  FILE *f=fopen(nameOfSave,"w");
  struct BlockPointerTable* cycle;
  if(f==NULL)
  {
   printf("Error opening file!\n");
   exit(1);
  }*/
  /**Saving sizes of file system metrics**/
  /*
  fprintf(f,"!%s",myFS.fileSystemName);
  fprintf(f,"!%d",myFS.numBlocks);
  fprintf(f,"!%d",myFS.numABPT);
  fprintf(f,"!%d",myFS.numFNT);
  fprintf(f,"","\n");
  
  for(i=0;i<myFS.numFNT;i++){
     fprintf(f,"!%s",myFS.files[i].fileName);
     //if it reaches this count it must have an iNode pointer base on numFNT
     fprintf(f,"!%d",myFS.files[i].iNode->fileSize);
     fprintf(f,"!%s",myFS.files[i].iNode->modifiedDateTime);
     fprintf(f,"!%d",myFS.files[i].iNode->dataBlockPointer);
     fprintf(f,"!%s",myFS.files[i].iNode->permissionCode);*/
     /**Save the actual Data of the file**/
     /*
     for(j=0;i<BLOCKSIZE;j++){
       myFS.dataBlocks[myFS.files[i].iNode->dataBlockPointer][j];
     }
     //it might have a extentPointer if big file
     if(myFS.files[i].iNode->extentPointer!=NULL){
       cycle=myFS.files[i].iNode->extentPointer;
       //it may have multiple extent pointers keep going to the chain stops
        while(cycle->extentPointer!=NULL){
	 fprintf(f,"!%s",myFS.files[i].fileName);
         //if it reaches this count it must have an iNode pointer base on numFNT
         fprintf(f,"!%d",myFS.files[i].iNode->fileSize);
         fprintf(f,"!%s",myFS.files[i].iNode->modifiedDateTime);
         fprintf(f,"!%d",myFS.files[i].iNode->dataBlockPointer);
         fprintf(f,"!%s",myFS.files[i].iNode->permissionCode);
	 fprintf(f,"","\n");
	 for(j=0;i<BLOCKSIZE;j++){
          myFS.dataBlocks[myFS.files[i].iNode->dataBlockPointer][j];
         }
         cycle=cycle->extentPointer;//Move to next block
       }
     }  
    }
    fclose(f);*/
  }
 
/**
 * Description:   opens and rebuilds a file system.
 * input--
 * nameOfSave:    name of file tochangePermission be opened
 * 
 * return:        void
 */
void openFileSystem(char *nameOfSave){
  
}


/**
 * Description:     Lists files in File System
 * input--
 * none
 * return:          void
 */
void listFiles(){
  
  
}

/**
 * Description:   Removes a File from File System
 * input--
 * nameOfFile:    File name to be removed
 * 
 * return:        void
 */
void removeFile(char *nameOfFile){
  
  
}

/**
 * Description:    Adds a file from host OS to simulated File System
 * input--
 * nameOfFile:     file name being transferred
 * 
 * return:         void
 */
void putFile(char *nameOfFile){
  printf("Selected file:%s\n",nameOfFile);
  int i=0;
  DIR *d;
  struct dirent *de;  
  d=opendir("."); 
    
  while((de=readdir(d))){
     if(((de->d_type) & DT_REG)){
       //if there is a file match start building the file
       if(!strcmp(nameOfFile,de->d_name)){
	 printf("Found file...\n");
	 buildFile(nameOfFile);
	 i=1;
	 closedir(d);
	 return;
       }
     }
    }
    if(i==0){
     printf("File doesn't Exits\n");
    }
    closedir(d);
}

/**
 * Description:   builds FNT and ABPT if a file name is found
 * input--
 * fileName:      name of new file being added to FS
 * FNTIndex:      next available FNT index
 * ABPTIndex:     
 * 
 * return:        void
 */
int buildFile(char* fileName){
  int usedFNTIndex;
  int usedABPTIndex;
  int usedDataBlockIndex;
  if(findNextFreeSlot(myFS.FNTMap,myFS.numFNT)==(-1)){
   printf("No free files left\n");
   return -1;    
  }else if(findNextFreeSlot(myFS.dataBlockMap,myFS.numBlocks)==(-1)){
    printf("All data blocks full\n");
    return -1;
  }else if(findNextFreeSlot(myFS.ABPTMap,myFS.numABPT)==(-1)){
     printf("No ABPT's available\n");
    return -1;
  }else if((freeBlocks(myFS.dataBlockMap,myFS.numBlocks)*BLOCKSIZE)<(getFileSize(fileName))){
    printf("Not enough data to store new file\n");
    return -1;
  }else{
    printf("Test1\n");
   //update FS attributes
   usedFNTIndex=findNextFreeSlot(myFS.FNTMap,myFS.numFNT);
   usedABPTIndex=findNextFreeSlot(myFS.ABPTMap,myFS.numABPT);
   usedDataBlockIndex=findNextFreeSlot(myFS.dataBlockMap,myFS.numBlocks);
   myFS.FNTMap[usedFNTIndex]=1;
   myFS.dataBlockMap[usedDataBlockIndex]=1;
   myFS.ABPTMap[usedABPTIndex]=1;
   printf("Test2\n");
   //update FNT attributes name and iNode to next avail ABPT
   strcpy(myFS.files[findNextFreeSlot(myFS.FNTMap,myFS.numFNT)].fileName,fileName);
   myFS.files[usedFNTIndex].iNode=myFS.pointerTables[usedABPTIndex];
   printf("Test3\n");
   //Update ABPT attributes.
   myFS.files[usedFNTIndex].iNode->fileSize=getFileSize(fileName);
   strcpy(myFS.files[usedFNTIndex].iNode->modifiedDateTime,getModDate(fileName));
   myFS.files[usedFNTIndex]->iNode.dataBlockPointer=usedDataBlockIndex;
   strcpy(myFS.files[usedFNTIndex].iNode->permissionCode,"777");
   printf("Test4\n");
   //begin to write data to blocks
   writeData(fileName,usedDataBlockIndex,usedFNTIndex,usedABPTIndex);
   
   return 1;
  }
}

/**
 * Description:    Find next available index FNT/ABPT tables 
 * input-- 
 * arraySearch:    Char array being searched
 * sizeOfArr:      Size of array for boundry check
 * 
 * return:        index of array available, or -1 if nothing available
 */
int findNextFreeSlot(int* arraySearch,int sizeOfArr){
  int i;
  for(i=0;i<sizeOfArr;i++){
   if(arraySearch[i]==0){
    return i; 
   } 
  }
  //no available slot return -1;
  return -1;
}

/**
 * Description:    Gets number of frees lots in map
 * input-- 
 * arraySearch:    Char array being searched
 * sizeOfArr:      Size of array for boundry check
 * 
 * return:        index of array available, or -1 if nothing available
 */
int freeBlocks(int* arraySearch,int sizeOfArr){
  int i;
  int total=0;
  for(i=0;i<sizeOfArr;i++){
   if(arraySearch[i]!=1){
    total=(total+1);;
   } 
  }
  return total;
}

/**
 * Description:    updates available FNT/ABPT tables   
 * input--
 * arraySearch:    Char array being searched
 * valChange:      value being updated
 * index:          index being updated
 * 
 * return:        index of array available, or -1 if nothing available
 */
void updateSlot(int* arraySearch,int valChange, int index){
  arraySearch[index]=valChange;
}

/**
 * Description:   prints file in current direcoty
 * input--
 * 
 * return:        void
 */
void listFilesInDir(){
  DIR *d;
  struct dirent *de;
  printf("\n\n\n\n\n");  
  printf("Files in directory to add:\n");
  
  d=opendir("."); //open current working directory
    
  while((de=readdir(d))){
     if(((de->d_type) & DT_REG)){
       printf("%s\n",de->d_name);
     }
    }
    closedir(d);
}

/**
 * Description:   gets file size
 * input--        file name
 * 
 * return:        files Size in Bytes
 */
int getFileSize(const char *fileName){
  const char *fd =fileName;
  struct stat *buf; //helps get file statistics
  buf =malloc(sizeof(struct stat));
  
  stat(fd,buf);
  int size=buf->st_size;
  return size;
  free(buf);
}

/**
 * Description:   getFile modification date
 * input--        file name
 * 
 * return:        formatted char string of mod time
 */
char * getModDate(char* fileName){
  char *fd =fileName;
  struct stat *buf; //helps get file statistics
  buf =malloc(sizeof(struct stat));
  
  stat(fd,buf);
  time_t myTime=buf->st_mtime;
  return ctime(&myTime);
  free(buf);
}


/**
 * Description:     Write data to data block. Finds next data block if file is over 256
 * input--  
 * filename:        File name being considered for operation
 * dataBlockIndex:  The first data block to be written to
 * FNTindex:        Index of start file 
 * ABPTindex:       Index of next ABPT 
 * return:          1 success -1 failure
 */

int writeData(const char* fileName,int dataBlockIndex,int FNTindex, int ABPTindex){//----__Stopping Point---------------
  FILE *fp;
  int usedFNTIndex;
  int usedABPTIndex;
  int usedDataBlockIndex;
  int dataLeft= getFileSize(fileName);
  
  if((fp=fopen(fileName,"r"))==NULL){
   printf("cannot open file.\n");
   return -1;
  }
  do{
    //read file in
  if((fread(myFS.dataBlocks[dataBlockIndex],sizeof(char),BLOCKSIZE,fp))){
     printf("Whats in file:\n%s\n", myFS.dataBlocks[0]);
     //calc remaing data left if any
     dataLeft=(dataLeft-BLOCKSIZE);
     //set block to used
     myFS.dataBlockMap[dataBlockIndex]=1;
  }
  //if file is bigger than 256
  if(dataLeft>0){
    
    //update extent pointer to next block of data
    usedABPTIndex=findNextFreeSlot(myFS.ABPTMap,myFS.numABPT);
    myFS.files[FNTindex]->iNode->extentPointer=myFS.pointerTables[usedABPTIndex];
    //update ABPT map with occupied value =1
    myFS.ABPTMap[findNextFreeSlot(myFS.ABPTMap,myFS.numABPT)]=1;
    //update datablock index for next iteration of file reading
    dataBlockIndex=findNextFreeSlot(myFS.dataBlockMap,myFS.numBlocks);
    //update data block pointer
    myFS.files[FNTindex]->iNode->extentPointer->dataBlockPointer=dataBlockIndex;
  }
  }while(dataLeft>0);//will only work if 2 data blocks or less are used
  return 1;
}

/**
 * Description:    Moves file from simulated File SYstem to Host OS.
 * input--
 * nameOfFile:     file name being transferred
 * 
 * return:          void
 */

void getFile(char *nameOfFile){
  
}

/**
 * Description:    Adds a shortcut to a file
 * input--
 * shortCutName:   name of link being created;
 * nameOfFile:       name of File the link points to.
 * 
 * return:         void
 */

void createLink(char *shortCutName, char *nameOfFile){
  
}

/**
 * Description:     Removes a link from the file system
 * input--
 * shortCutName:    Name of shortcut
 * 
 * return:          void
 */

void removeLink(char *shortCutName){
  
}

/**
 * Description:     Sets the permissions of the FileNameTable
 * input--
 * permissionCode:  permission to be set to
 * fileName:        name of file with permissions being changed
 * 
 * return:          void
 */
void changePermission(char *permissionCode, char *fileName){
  
  
}






