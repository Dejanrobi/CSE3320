/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 4 A Simple File System
 * Due Date: 4/12/2015
 *******************************/

/********************************************
 * Project Description located in main()
 * Class that represents a file system
 * 
 *
 ********************************************/
import java.util.ArrayList;
import java.util.Arrays; //used to fill array of chars
import java.io.*;
import java.lang.Math;
import java.text.SimpleDateFormat;//used to formatFile mod date

public class FileSystem implements Lab4Constants, Serializable{
  String systemName;
  int numBlocks;
  int numABPT;
  int numFNT;
  ArrayList<Integer> FNTMap= new ArrayList<Integer>();
  ArrayList<Integer> ABPTMap= new ArrayList<Integer>();
  ArrayList<Integer> dataBlockMap= new ArrayList<Integer>();
  
  ArrayList<FileNameTable> files= new ArrayList<FileNameTable>();
  ArrayList<BlockPointerTable> ptrTable= new ArrayList<BlockPointerTable>();
  ArrayList<Character[]> datablock= new ArrayList<Character[]>();//Have to add 256 size later
  ArrayList<Link> link= new ArrayList<Link>();
  public FileSystem(){
	  
  }
  public FileSystem(String systemName, int blocks, int abpt, int fnt){
	
    this.numBlocks=blocks;
    this.numABPT=abpt;
    this.numFNT=fnt;
    Character[] emptyString = new Character[BLOCK_SIZE];
    Arrays.fill(emptyString, '0');
    //allocate 256 size for each element
    for(int i=0;i<this.numBlocks;i++){
      this.datablock.add(new Character[BLOCK_SIZE]);
      this.datablock.set(i,emptyString);
      this.dataBlockMap.add(0);  //format map 0= not occupied
    }
    for(int i=0;i<this.numABPT;i++){
      BlockPointerTable mytable= new BlockPointerTable();
      this.ABPTMap.add(0); //format map 0= not occupied
      ptrTable.add(mytable);//fill table with empty tables
    }
    
    for(int i=0;i<this.numFNT;i++){
    	FileNameTable myFNT=new FileNameTable();
    	this.FNTMap.add(0);//format map 0= not occupied
    	files.add(myFNT); //Fill with empty names.
    }
    this.setDataForFS();
    
    
  }
  
  private void setName(String name){
	  this.systemName=name;
	  
  }
  
  private void setData(int numBlocks){
	  this.numBlocks=numBlocks;
	  Character[] emptyString = new Character[BLOCK_SIZE];
	  Arrays.fill(emptyString, '0');
	  for(int i=0;i<this.numBlocks;i++){
	      this.datablock.add(new Character[BLOCK_SIZE]);
	      this.datablock.set(i,emptyString);
	      this.dataBlockMap.add(0);  //format map 0= not occupied
	    }
  }
  
  private void setABPT(int numABPT){
	  this.numABPT=numABPT;
	  for(int i=0;i<this.numABPT;i++){
	      BlockPointerTable mytable= new BlockPointerTable();
	      this.ABPTMap.add(0); //format map 0= not occupied
	      ptrTable.add(mytable);//fill table with empty tables
	    }
	  for(int i=0;i<this.numFNT;i++){
	    	FileNameTable myFNT=new FileNameTable();
	    	this.FNTMap.add(0);//format map 0= not occupied
	    	files.add(myFNT); //Fill with empty names.
	    }
	  
  }
  private void setFNT(int numFNT){
	  this.numFNT=numFNT;
	  for(int i=0;i<this.numFNT;i++){
	    	FileNameTable myFNT=new FileNameTable();
	    	this.FNTMap.add(0);//format map 0= not occupied
	    	files.add(myFNT); //Fill with empty names.
	    }
	  this.setDataForFS();
	  
  }
public void setDataForFS(){
	int total=0;
	int blocksReq=0;
	total=(this.numABPT*ABPT_SIZE+this.numFNT*FNT_SIZE);
	blocksReq=(int) Math.ceil(total/BLOCK_SIZE+1);
	for(int i=0;i<blocksReq;i++){
		this.dataBlockMap.set(getNextFreeSlot(this.dataBlockMap),1);//set map according to size
	}
}

public void menu(){
    int select=-1;
    int numBlocks=-1;
    int numABPT=-1;
    int numFNT=-1;
    String fileName="";
    //get user info
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    
    while(true){
      System.out.println("0: Exit");
      System.out.println("1: Create File System");
      System.out.println("2: Format File System");
      System.out.println("3: Save File System");
      System.out.println("4: Open File System");
      System.out.println("5: List Files");
      System.out.println("6: Remove File");
      System.out.println("7: Put File-- Host Fiel to File System");
      System.out.println("8: Get File-- copy FS File to OS");
      System.out.println("9: Add Link--Create a shortcut to file");
      System.out.println("10: Remove Link-- remove Shortcut");
      System.out.println("11: Set Permmissions on File");
      System.out.println("12: Check disc utility. Print all info in FileSystem");
      try{
             select = Integer.parseInt(br.readLine());
        }catch(NumberFormatException nfe){
            System.err.println("Invalid Format!");
        }catch(IOException nfee){
            System.err.println("Something else went wrong");
        }
        switch(select){
          
          case 0: System.exit(0);
          case 1: 
            System.out.println("How many data blocks in the system");
            try{
             numBlocks = Integer.parseInt(br.readLine());
                }catch(NumberFormatException nfe){
                 System.err.println("Invalid Format!\n");
                }catch(IOException nfee){
                 System.err.println("Something else went wrong");
                }
            
            break;
          case 2:
            String saveNameFS="";
            int numPointers=-1;
            int numFiles=-1;
            System.out.println("Formatting file system..");
            System.out.println("Enter name of file system:");
            try{
            saveNameFS= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            System.out.println("How many block pointers? (ABPT)?");
            try{
             numPointers = Integer.parseInt(br.readLine());
                }catch(NumberFormatException nfe){
                 System.err.println("Invalid Format!\n");
                }catch(IOException nfee){
                 System.err.println("Something else went wrong");
                }
             System.out.println("How many files? (FNT)");
             try{
              numFiles = Integer.parseInt(br.readLine());
                }catch(NumberFormatException nfe){
                 System.err.println("Invalid Format!\n");
                }catch(IOException nfee){
                 System.err.println("Something else went wrong");
                }
            System.out.println("got "+saveNameFS+"with "+numBlocks+"blocks "+"ABPT "+numPointers+"files "+numFiles);
            this.setName(saveNameFS);
            this.setData(numBlocks);;
            this.setABPT(numPointers);
            this.setFNT(numFiles);//has to be called last
            //FileSystem myFS= new FileSystem(saveNameFS,numBlocks,numPointers,numFiles);
          break;
          
          case 3:
        	 String saveNameFS1="";
        	  System.out.println("What is the name you want to save as?");
            
            try{
            saveNameFS1= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            saveFS(saveNameFS1);
            break;
            
          case 4:
            String saveNameFS2="";
        	  System.out.println("Open File system What is the Name of file?");
            try{
            saveNameFS2= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            openFileSystem(saveNameFS2);
            printMaps();
            break;
          case 5:
            System.out.println("Listing files in FS....");            
            listFiles();
            break;
          case 6:
            String remove="";
        	System.out.println("What File do you want to remove?");
            try{
            	remove= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            removeFile(remove);
            break;
          case 7:
        	  String temp="";
        	  System.out.println("What file name?");
        	  try{
        		  temp= br.readLine();
                  }catch(IOException nfee){
                      System.err.println("Something else went wrong");
                  }
        	  getFileSize(temp);
        	  System.out.println("Free slots in FNT: "+getFreeMapIndex(this.FNTMap));
        	  System.out.println("Free slots in Data: "+getFreeMapIndex(this.dataBlockMap));
        	  System.out.println("Free slots in ABPT: "+getFreeMapIndex(this.ABPTMap));
        	  putFile(temp);
        	  printMaps();
            break;
          case 8:
        	String select1="";
            System.out.println("What File do you want to move to Host OS?");
            try{
            	select1= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            getFile(select1);
            break;
          case 9:
            String linkName="";
            String saveNameFS3="";
            System.out.println("What is the name of Link you want to create?");
            try{
            linkName= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            System.out.println("What file do you want to link to?");
            try{
            	saveNameFS3= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            createLink(linkName,saveNameFS3);
            break;
          case 10:
        	String saveNameFS4="";
            System.out.println("What Link do you want to remove?");
            try{
            	saveNameFS4= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            removeLink(saveNameFS4);
            break;
          case 11:
            String permission="";
            String saveNameFS5="";
            System.out.println("What File do you want to set permissions on?");
            System.out.println("Note--Permissions effect Get and remove commands");
            try{
            	saveNameFS5= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            System.out.println("What is the permission code?");
            try{
            permission= br.readLine();
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            changePermission(permission,saveNameFS5);
            break;
          case 12:
        	  System.out.println("Checking disc....");
        	  this.printMaps();
        	  break;
                                 
        }
      
    }
  }
    


	/**
     * Description:   saves a File System to a file locally.
     * input--
     * savedName:    name of the File that saves the file System
     * 
     * return:        void
     */
    public void saveFS(String savedName){
      FileSystem bitch= new FileSystem("hey",10,4,10);
    	try {
			ObjectOutputStream os =new ObjectOutputStream(new FileOutputStream(savedName));
			os.writeObject(this);
			os.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
      
    }
    /**
     * Description:   opens fileSystem passes to loadFS()
     * input--
     * savedName:    name of file to be opened
     * 
     * return:        void
     */
    public FileSystem openFileSystem(String savedName){
      FileSystem newBitch=new FileSystem();
      try {
		ObjectInputStream is= new ObjectInputStream(new FileInputStream(savedName));
		newBitch= (FileSystem)is.readObject();
		is.close();
		this.loadFS(newBitch);
	} catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	} catch (ClassNotFoundException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
	return newBitch;
    }
    /**
     * Description:   opens fileSystem passes to builFS()
     * input--
     * savedName:    name of file to be opened
     * 
     * return:        void
     */
    private void loadFS(FileSystem loadedFS){
    	//possibly delete file System before loading
    	
    	this.setName(loadedFS.systemName);
    	this.numBlocks=loadedFS.numBlocks;
    	this.numABPT=loadedFS.numABPT;//set sizes
    	this.numFNT=loadedFS.numFNT;
    	//set maps
    	this.FNTMap=loadedFS.FNTMap;
    	this.ABPTMap=loadedFS.ABPTMap;
    	this.dataBlockMap=loadedFS.dataBlockMap;
    	
    	
    	this.setData(loadedFS.numBlocks);
    	this.setABPT(loadedFS.numABPT);
    	this.setFNT(loadedFS.numFNT);
    	
    	this.files=loadedFS.files;
    	this.ptrTable=loadedFS.ptrTable;    	
    	this.link=loadedFS.link;
    	
    	//load data in blocks
    	for(int i=0;i<loadedFS.datablock.size();i++){
    		this.datablock.set(i,loadedFS.datablock.get(i));
    	}
    	
    	
    	
    	
    	//set data in FNT ABPT and data blocks by looping.
    }
  
  /**
   * Description:     Lists files in File System
   * input--
   * none
   * return:          void
   */
   public void listFiles(){
      System.out.println("Files:----------");
      for(int i=0;i<this.files.size();i++){
    	  if(!this.files.get(i).fileName.equals("")){
    		  int ABPTused;
    		  ABPTused=this.files.get(i).iNode;
    		  System.out.print(this.ptrTable.get(ABPTused).permissionCode+"   "+this.ptrTable.get(ABPTused).modifiedDateTime+"   "+
    		  this.ptrTable.get(ABPTused).fileSize);
    		  System.out.println("    "+this.files.get(i).fileName);
    	  }
      }
      System.out.println("");
      System.out.println("Links:----------");
      for(int i=0;i<this.link.size();i++){
    	  if(!this.link.get(i).linkName.equals("")){
    		  System.out.println(this.link.get(i).linkName+" links to :"+this.link.get(i).linkFNT.fileName);
    	  }
      }
  
   }
   
   /**
     * Description:   Removes a File from File System
     * input--
     * nameOfFile:    File name to be removed
     * 
     * return:        void
     */
   public void removeFile(String nameOfFile){
	   int nextIndex=findFile(nameOfFile);
	   if(findFile(nameOfFile)<0){
		   System.out.println("File not Found");
		   return;
	   }
	   
	   System.out.println("Permission? "+this.testPermission(this.ptrTable.get(this.files.get(nextIndex).iNode).permissionCode));
	   
	   if(!this.testPermission(this.ptrTable.get(this.files.get(nextIndex).iNode).permissionCode)){
		   System.out.println("You do not have permission for this file");
		   return;
	   }
	   
	   Character[] emptyString = new Character[BLOCK_SIZE];
	   Arrays.fill(emptyString, '0');
	   System.out.println("File Found. Removing...");
	   
	   //format all ABPTs related to FNT
	   for(int i=0;i<this.files.get(nextIndex).usedABPT.size();i++){
		   int presentDataBlockPointer=-1;
		   //Clear data at block
		   presentDataBlockPointer=this.ptrTable.get(this.files.get(nextIndex).usedABPT.get(i)).dataBlockPointer;
		   this.datablock.set(presentDataBlockPointer,emptyString);
		   //set map to unused
		   this.dataBlockMap.set(presentDataBlockPointer,0);
		   
		   this.ptrTable.get(this.files.get(nextIndex).usedABPT.get(i)).fileSize=0;
		   this.ptrTable.get(this.files.get(nextIndex).usedABPT.get(i)).modifiedDateTime="";
		   this.ptrTable.get(this.files.get(nextIndex).usedABPT.get(i)).dataBlockPointer=-1;
		   this.ptrTable.get(this.files.get(nextIndex).usedABPT.get(i)).permissionCode="777";
		   this.ptrTable.get(this.files.get(nextIndex).usedABPT.get(i)).extentPointer=-1;
		   //set ABPTMap
		   this.ABPTMap.set(this.files.get(nextIndex).usedABPT.get(i),0);
	   }
	   
	   //Format FNT meta-data
	   this.FNTMap.set(nextIndex,0);
	   this.files.get(nextIndex).fileName="";
	   this.files.get(nextIndex).iNode=-1;
	   this.files.get(nextIndex).usedABPT.clear();
	   
	   
   }
   
   /**
    * Description:    Adds a file from host OS to simulated File System
    * input--
    * nameOfFile:     file name being transferred
    * 
    * return:         void
    */
   public void putFile(String nameOfFile){
	   
	   int ABPTrequired=(int) Math.ceil(getFileSize(nameOfFile)/BLOCK_SIZE+1);
	   //do you have permissions do do this?
	   
	   
	   if(getFileSize(nameOfFile)>(getFreeMapIndex(this.dataBlockMap)*BLOCK_SIZE)){
		   //System.out.println("File size: "+getFileSize(nameOfFile)+" free data"+(getFreeMapIndex(this.dataBlockMap)*BLOCK_SIZE));
		   System.out.println("File too big to put");
		   return;
	   }
	   if(getFreeMapIndex(this.FNTMap)==0){
		   System.out.println("No FNT's left to allocate");
		   return;
	   }
	   //ABPT used 254=1 257=2
	   if(ABPTrequired>getFreeMapIndex(this.ABPTMap)){
		   System.out.println("No ABPT's left to allocate");
		   return;
	   }
	   //set a free index to used =1
	   
	   //this.ABPTMap.add(usedABPT,1);
	   setFNT(nameOfFile,ABPTrequired);//also sets the ABPT on first occurrence more maybe added when data is written.
	   
	   
     
   }
/**
    * Description:    Counts how many free slots are avail for data, ABPT and FNT
    * input--
    * myList:         list being searched
    * 
    * return:         void
    */
   private int getFreeMapIndex(ArrayList<Integer> myList) {
	   int count=0; 
	   for(int i=0;i<myList.size();i++){
	    	if(myList.get(i)==0){
	    		count++;
	    	}
	    }
	   return count;	
    }
   
   /**
    * Description:    Counts how many free slots are avail for data, ABPT and FNT
    * input--
    * myList:         list being searched
    * 
    * return:         void
    */
   private int getFreeMapIndexLink(ArrayList<Link> myList) {
	   int count=0; 
	   for(int i=0;i<myList.size();i++){
	    	if(myList.get(i).linkName.equals("")){
	    		count++;
	    	}
	    }
	   return count;	
    }


/**
    * Description:    Gets file size that is sent to it.
    * input--
    * fileName:       name of file to be calculated.
    * 
    * return:         size of file in bytes
    */
   private int getFileSize(String fileName) {
		File f =new File(fileName);
		if (!f.exists() || !f.isFile()) {
	         System.out.println("File doesn\'t exist");
	         return -1;
	      }
		return (int) f.length();
		
	}
 
   /**
    * Description:    Finds first free slot in List
    * input--
    * myList:         list being searched
    * 
    * return:         index of free slot
    */
   private int getNextFreeSlot(ArrayList<Integer> myList) {
	   
	   for(int i=0;i<myList.size();i++){
	    	if(myList.get(i)==0){
	    		return i;
	    	}
	    }
	   return -1;
    }
   /**
    * Description:    adds a new FNT to File system
    * input--
    * nameOfFile:     File name being added to FNT
    * aBPTrequired:   ABPT to required to write file.
    * 
    * return:         void
    */
   private int setFNT(String nameOfFile, int ABPTrequired){
		int usedABPT=-1;
		int fileStatus[]={0,0};
		int FNTused=-1;
		FNTused=getNextFreeSlot(this.FNTMap);
		this.FNTMap.set(FNTused,1);
	    FileNameTable myFNT= new FileNameTable();
	    myFNT.setName(nameOfFile);
	    myFNT.iNode=FNTused;
	    usedABPT=getNextFreeSlot(this.ABPTMap);//get index of next free ABPT
		myFNT.iNode=usedABPT;
		fileStatus[1]=getFileSize(nameOfFile);
		this.files.set(FNTused,myFNT);
		
		//Update ABPT map 1=used
		//first call to setABPT so offset =0;
		return setABPT(usedABPT,nameOfFile,ABPTrequired,fileStatus,this.files.set(FNTused,myFNT)); //will return index of next data block
   }
   
   /**
    * Description:    adds new ABPT to File system
    * input--
    * usedABPT:       index of ABPT map being used.
    * nameOfFile:     File name being added to ABPT used to get size
    * ABPTrequired:   ABPT to required to write file.
    * offSet:         Offset for file pointer to start reading from
    * 
    * return:         index of data slot to start writing
 * @param ABPTrequired 
    */
   private int setABPT(int usedABPT,String nameOfFile, int ABPTrequired, int[] offset,FileNameTable myFNT) {
	   myFNT.usedABPT.add(usedABPT);
	   
	   BlockPointerTable myptr= new BlockPointerTable();
	   File file = new File(nameOfFile);
	   int dataBlockUsed=-1;
	   //first index says how many read, second is how many left to read
	   System.out.println("Read: "+offset[0]+" How many left "+offset[1]);
	   //Set this index to being used =1
	   this.ABPTMap.set(usedABPT,1);
	   myptr.fileSize=getFileSize(nameOfFile);
	   SimpleDateFormat sdf = new SimpleDateFormat("MM/dd/yyyy HH:mm:ss");
	   myptr.modifiedDateTime=sdf.format(file.lastModified());
	   //Permissions already set to 777
	   dataBlockUsed=getNextFreeSlot(this.dataBlockMap);
	   System.out.println("Data index: "+dataBlockUsed);
	   myptr.dataBlockPointer=dataBlockUsed;
	   //Write data to first block
	   offset=writeData(dataBlockUsed,file,offset);
	   this.dataBlockMap.set(dataBlockUsed,1);
	   if((ABPTrequired-1)>0){
	     myptr.extentPointer=getNextFreeSlot(this.ABPTMap);
	   }
	   //Now add it to file system
	   this.ptrTable.set(usedABPT,myptr);
	   //Take care of extent pointers if needed recursive call
	   if((ABPTrequired-1)>0){
		   usedABPT=getNextFreeSlot(this.ABPTMap);//get index of next free ABPT
		   this.setABPT(usedABPT, nameOfFile, ABPTrequired-1, offset,myFNT);		   
	   }
	   return dataBlockUsed;//will be used to write to array of characters.
   }
   
   /**
    * Description:    writes data to character array
    * input--
    * dataIndex:      index to start writing
    * file:           File being read
    * 
    * return:         index of file to start next block of writing
 * @param file 
    */
   private int[] writeData(int dataIndex, File file, int[] offset) {
	   
	   try{
	FileInputStream fis=new FileInputStream(file);
	   int count=0;
	   int dummy=-1;
	   Character dumm2=' ';
	   String dumm3="";
	   //first index says how many read, second is how many left to read
	   System.out.println("Read:"+offset[0]+" how much left: "+offset[1]);
	   
	   //if this is second time writing you need to offset read
	   //to second+ blocks
	   
	   
	   
	   //do until you reach block size (in case of big file) or hit
	   //end of file
	   Character[] chBuff = new Character[BLOCK_SIZE];
	   
	   while(fis.available()>0 &&count<BLOCK_SIZE&&offset[1]>0){
		   dummy++;
		   if(dummy<offset[0]){
		   dumm2=(char)fis.read();
		   }
		   
		   if(dummy>=offset[0]){
			   //System.out.println(dumm2);
		   chBuff[count]=(char)fis.read();
			//dumm3+=(char)fis.read();   
		   //this.printCharArray(chBuff);
		   count++;
		   offset[0]++;		   
		   offset[1]--;
		   }
	   }
	   this.datablock.set(dataIndex,chBuff);
	   fis.close();
	   return offset;
	   }catch (IOException e){
		   e.printStackTrace();
	   }
	   int fileStatus[]={0,0};
	   return fileStatus;
	   
	   
	}


/**
    * Description:    Moves file from simulated File System to Host OS.
    * input--
    * nameOfFile:     file name being transferred
    * 
    * return:          void
    */

   public void getFile(String nameOfFile){
	   if(this.findFile(nameOfFile)<0){
		   System.out.println("File Doesn't exist");
		   return;
	   }
	   
	   //do you have permissions do do this?
	   int nextIndex=findFile(nameOfFile);
	   if(!this.testPermission(this.ptrTable.get(this.files.get(nextIndex).iNode).permissionCode)){
		   System.out.println("You do not have permission for this file");
		   return;
	   }
	   File f = new File("SavedFiles\\"+nameOfFile);
	   try {
		f.createNewFile();
	   } catch (IOException e) {
		System.out.println("Somethign wrong creating file");
		e.printStackTrace();
	   }
	   FileWriter fileOut=null;
	   //Open File
	   try {
		fileOut =new FileWriter("SavedFiles\\"+nameOfFile);
	    } catch (IOException e) {
		System.out.println("Something wrong with writing to file");
		e.printStackTrace();
	    }
	   int FNTused;
	   int ABPTused;
	   //Find FNT
	   FNTused=this.findFile(nameOfFile);
	   //What ABPT is it tied to?
	   ABPTused=this.files.get(FNTused).iNode;	  
	   writeBlock(fileOut,ABPTused);
	   try {
		fileOut.close();
	   } catch (IOException e) {
		System.out.println("Can't close file goodluck");
		e.printStackTrace();
	   }
	    
	    
   }
   private void writeBlock(FileWriter fileOut, int ABPTused) {
       String tempString="";
	   int dataBlockused;
	   //Find data block pointed to
	   dataBlockused = this.ptrTable.get(ABPTused).dataBlockPointer;
	   //write that data char by char
	   for(int i=0;i<this.datablock.get(dataBlockused).length&&this.datablock.get(dataBlockused)[i]!=null;i++){
		   tempString+=this.datablock.get(dataBlockused)[i];
	   }
	   try { 
			 fileOut.write(tempString);
		   } catch (IOException e) {
			 System.out.println("Failed writing data");
			 e.printStackTrace();
		   }		   
	   
	   //does this ABPT have a extent pointer ie point not -1? yay more recursion
	   if(this.ptrTable.get(ABPTused).extentPointer!=-1){
		   ABPTused=this.ptrTable.get(ABPTused).extentPointer;
		   writeBlock(fileOut,ABPTused);
	   }	   
	   //if so go to that ABPT and write.
	   
	   
	   
	
}
/**
    * Description:    Search FS for file named
    * input--
    * nameOfFile:     name being searched for
    * 
    * return:          if found returns index else -1
    */
   private int findFile(String nameOfFile) {
	 int flag=-1;
	   for(int i=0;i<this.files.size();i++){
		   if(this.files.get(i).fileName.equals(nameOfFile)){
			   flag=i;
			   return flag;
		 }
	 }
	   return flag;
	
}
public void printCharArray(Character[] ch){
	   
	   for(int i=0;i<ch.length;i++){
		   System.out.print(ch[i]);		   
	   }
   }
   
   /**
    * Description:    Adds a shortcut to a file
    * input--
    * shortCutName:   name of link being created;
    * nameOfFile:       name of File the link points to.
    * 
    * return:         void
    */

   public void createLink(String shortCutName, String nameOfFile){
	   FileNameTable tempFNT= new FileNameTable();
	   boolean flag=false;
	   int index=0;
	   Link tempLink=new Link();
	   for(int i=0;i<this.files.size();i++){
		   if(this.files.get(i).fileName.equals(nameOfFile)){
			   flag=true;
			   index=i;
			   System.out.println("Index: "+i);
			   break;
		   }
	   }
	   
	   if(flag){
		   System.out.println("File Found...linking");
		   tempFNT=this.files.get(index);
	   }else{
		   System.out.println("File Not Found");
		   return;		   
	   }
	   tempLink.linkName=shortCutName;
	   tempLink.linkFNT=tempFNT;
	   
	   int nextIndex=-1;
	   nextIndex=getFreeMapIndexLink(this.link);
	   this.link.add(tempLink);
  
   }
   
   /**
    * Description:     Removes a link from the file system
    * input--
    * shortCutName:    Name of shortcut
    * 
    * return:          void
    */

   public void removeLink(String shortCutName){
	   boolean flag=false;
	   int index=0;
	   for(int i=0;i<this.link.size();i++){
		   System.out.println("LinkName:"+this.link.get(i).linkName+" search"+shortCutName);
		   System.out.println("Equal? "+this.link.get(i).linkName.equals(shortCutName));
		   if(this.link.get(i).linkName.equals(shortCutName)){
			   index=i;
			   flag=true;
		   }
	   }
	   if(flag){
		   this.link.remove(index);
		   System.out.println("Link Removed");
	   }else{
		   System.out.println("Link not found");
	   }
	   
   }
   
   
   /**
    * Description:     Sets the permissions of the FileNameTable
    * input--
    * permissionCode:  permission to be set to
    * fileName:        name of file with permissions being changed
    * 
    * return:          void
    */
   public void changePermission(String permissionCode, String fileName){
	   FileNameTable tempFNT= new FileNameTable();
	   boolean flag=false;
	   int index=0;
	   Link tempLink=new Link();
	   for(int i=0;i<this.files.size();i++){
		   if(this.files.get(i).fileName.equals(fileName)){
			  this.ptrTable.get(this.files.get(i).iNode).permissionCode=permissionCode;
			   break;
		   }
	   }
	   
	   
   }
   
   /**
    * Description:    tests permissions
    * input--
    * permissionCode:  permission code of the file being considered
    * 
    * return:          true if permission is write/read(6) or write/read/execute
    */
   
   boolean testPermission(String permissionCode){
	   boolean myPerm=false;
	   
	   if(permissionCode.substring(0,1).equals("7")){
		   return true;
		   
	   }else if(permissionCode.substring(0,1).equals("6")){
		   return true;
	   }else{
		   return false;
	   }
	   
   }
   /**
    * Description:     helper method to show data maps
    * input--
    * 
    * return:          printed out maps of data, ABPT, FNT
    */
   
  
   private void printMaps(){
	   System.out.println("NumData: "+this.numBlocks+" numABPT: "+this.numABPT+" numFNT: "+ this.numFNT);	   
	   System.out.print("ABPTMAP: ");
	   for(int i=0;i<this.ABPTMap.size();i++){
		   System.out.print(+this.ABPTMap.get(i));
	   }
	   System.out.println("");
	   System.out.print("dataBlockMap: ");
       for(int i=0;i<this.datablock.size();i++){
    	   System.out.print(+this.dataBlockMap.get(i));
	   }
       System.out.println("");
       System.out.print("FNT MAP: ");
       for(int i=0;i<this.numFNT;i++){
    	   System.out.print(+this.FNTMap.get(i));
	   }
       System.out.println("");
       System.out.println("Printing datablock:");
       for(int i=0;i<this.datablock.size();i++){
    	   System.out.println("Block: "+i);
    	   for(int j=0;j<this.datablock.get(i).length;j++){
    		   if(this.datablock.get(i)[j]==null){
    			   break;
    		   }
    		   System.out.print(this.datablock.get(i)[j]);
    	   }
    	   System.out.println("");
       }
       
       System.out.println("");
       
       System.out.println("Printing ABPT details");
       for(int i=0;i<this.ptrTable.size();i++){
    	   System.out.println(i+"size: "+this.ptrTable.get(i).fileSize+" Date: "+this.ptrTable.get(i).modifiedDateTime+" Datablockpointer: "
         +this.ptrTable.get(i).dataBlockPointer+" Permission: "+this.ptrTable.get(i).permissionCode+" extenPTR: "+this.ptrTable.get(i).extentPointer);
       }
       
       System.out.println("Printing FNT details");
       for(int i=0;i<this.files.size();i++){
    	   System.out.println("FileName: "+this.files.get(i).fileName+" BlockPointer: "+this.files.get(i).iNode);
    	   System.out.println("USED ABPT:");
    	   for(int j=0;j<this.files.get(i).usedABPT.size();j++){
    		   System.out.print(this.files.get(i).usedABPT.get(j));
    	   }
    	   System.out.println("");
       }
       System.out.println("\nLinks: ");
       for(int i=0;i<this.link.size();i++){
    	   System.out.println(i+": File Name "+this.link.get(i).linkName+"FNT info: Name: "+this.link.get(i).linkFNT.fileName+
    			   " FNT iNode pointer: "+this.link.get(i).linkFNT.iNode);
       }
   }
   
   


  
public static void main(String args []){
    FileSystem myFS= new FileSystem();
    String one="777";
    String one1="666";
    String one2="555";
    String one3="444";
    String one4="333";
    String one5="222";
    String one6="111";
    String one7="000";
    
    String two="6";
    
    System.out.println("Do you have permission?:"+myFS.testPermission(one));
    System.out.println("Do you have permission?:"+myFS.testPermission(one1));
    System.out.println("Do you have permission?:"+myFS.testPermission(one2));
    System.out.println("Do you have permission?:"+myFS.testPermission(one3));
    System.out.println("Do you have permission?:"+myFS.testPermission(one4));
    System.out.println("Do you have permission?:"+myFS.testPermission(one5));
    System.out.println("Do you have permission?:"+myFS.testPermission(one6));
    System.out.println("Do you have permission?:"+myFS.testPermission(one7));
    
    
    
    System.out.println("hey"+(two.compareTo(one)));
    myFS.menu();
  }
  
  
  } 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
