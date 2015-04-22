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

/**************************CONSTANTS******************************************/
#define TRUE         1    
#define FALSE        0 
#define BLOCKSIZE    256
#define FILENAMESIZE 56
#define TIMELENGTH   100
#define LINK_MAX     10
#define FILENAME "savedFS.txt"
/**************************STRUCTS--DECLARATIONS******************************/
typedef struct BlockPointerTable{
 int fileSize;                            //size of file
 char modifiedDateTime[TIMELENGTH];       //When last modified
 int dataBlockPointer;                    //index to dataBlock
 char permissionCode[3];                  //Permission for the File
 struct BlockPointerTable extentPointer; //incase of bigger files point to another file
}BlockPointerTable;

typedef struct FileNameTable{
  char fileName[FILENAMESIZE];           //name of file being stored
  BlockPointerTable iNode;       //iNode Pointer
}FileNameTable;

struct Link{
  char linkName[FILENAMESIZE];
  BlockPointerTable *linkedFile;
}Link;



struct FileSystem{
 char fileSystemName[FILENAMESIZE];       //Name of File System
 int numBlocks;                           //Number of Data blocks in System
 int numABPT;                             //Keep track of ABPT blocks avail
 int numFNT;                              //Keep track of FNT blocks avail
 int *FNTMap;                             //Maps available data slot for FNT
 int *ABPTMap;                            //Maps available data slot for ABPT
 int *dataBlockMap;                       //Maps available data slot for datablock CPM style 1 fiel 1 block
 struct FileNameTable *files;             //Structure for holding initial files
 struct BlockPointerTable *pointerTables;
 char **dataBlocks;                       //Data block for holding all info
 struct Link link[LINK_MAX];              //arritibtrary size of 10 links max
}FileSystem;

void menu();
void createFileSystem( int numBlocks);
void formatFileSystem(char name[FILENAMESIZE],int numPointerEntries,int numFNT);
void saveFileSystem(char *nameOfSave);
void openFileSystem(char *nameOfSave);
void listFiles();
void removeFile(char *nameOfFile);
void putFile(char *nameOfFile);
int buildFile(char* fileName);
int  findNextFreeSlot(int* arraySearch,int sizeOfArr);
int  freeBlocks(int* arraySearch,int sizeOfArr);
void updateSlot(int* arraySearch,int valChange, int index);
void listFilesInDir();
int getFileSize(const char *fileName);
char * getModDate(char* fileName);
void getFile(char *nameOfFile);
void createLink(char *shortCutName, char *nameOfFile);
void removeLink(char *shortCutName);
void changePermission(char *permissionCode, char *fileName);













