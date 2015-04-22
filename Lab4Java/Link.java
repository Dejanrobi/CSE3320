import java.io.Serializable;

/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 4 A Simple File System
 * Due Date: 4/12/2015
 *******************************/

/********************************************
 * Project Description located in main()
 * Class that represents Link
 * 
 *
 ********************************************/

public class Link implements Lab4Constants, Serializable{
  String linkName="";
  FileNameTable linkFNT =new FileNameTable(); //Points to the actual ABPT that holds the file
  
  public Link(){
  }
}