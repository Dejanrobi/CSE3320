/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 4 A Simple File System
 * Due Date: 4/12/2015
 *******************************/

/********************************************
 * Project Description located in main()
 * Interface that holds all constants required
 * 
 *
 ********************************************/

public interface Lab4Constants
 {
  int     BLOCK_SIZE        = 256;
  int     FILENAMESIZE      = 56;
  int     FNT_SIZE          = 60;  //56 chars=8bytes +1 int at 4 bytes
  int     ABPT_SIZE         = 71;  //fileSize+ date+dataPointer+PermCode+extPtr=(4+56+4+3+4)
}