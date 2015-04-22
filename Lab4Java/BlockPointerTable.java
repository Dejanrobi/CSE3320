import java.io.Serializable;

/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 4 A Simple File System
 * Due Date: 4/12/2015
 *******************************/

/********************************************
 * Project Description located in main()
 * Class that represents a Attribute Block Pointer Table(ABPT)
 * 
 *
 ********************************************/

public class BlockPointerTable implements Lab4Constants, Serializable{
  Integer fileSize=0;
  String modifiedDateTime="";
  Integer dataBlockPointer=-1;
  String permissionCode="777";
  Integer extentPointer=-1; //points to next index  ABPT Array if required
  
  public BlockPointerTable(){
    
  }
  
  
}