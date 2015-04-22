import java.io.Serializable;
import java.util.ArrayList;

/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 4 A Simple File System
 * Due Date: 4/12/2015
 *******************************/

/********************************************
 * Project Description located in main()
 * Class that represents a File Name Table
 * 
 *
 ********************************************/


public class FileNameTable implements Lab4Constants, Serializable{
  String fileName; //limit to 56chars
  int iNode=-1;//points to a ABPT
  ArrayList<Integer> usedABPT= new ArrayList<Integer>();
  
  public FileNameTable(String newFileName){
   this.fileName=newFileName;   
  }
  public FileNameTable(){
	  this.fileName="";
	  
  }
  
  public void setName(String name){
	  if(name.length()<=FILENAMESIZE){
	  this.fileName=name;
	  }else{
		System.out.println("error Name too long keep under 56 chars"); 
	  }
	  
  }
  
  
}