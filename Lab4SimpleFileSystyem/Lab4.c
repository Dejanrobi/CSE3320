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

/************************MAIN*************************************************/

int main(){
  menu();
 return EXIT_SUCCESS; 
}