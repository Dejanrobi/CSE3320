import java.net.*;
import java.io.*;
import java.nio.file.Files;
/********************************
 * Scott Nidell 
 * Class:CSE 3320
 * Assignment: Lab 5 Socket Programming
 * Due Date: 5/8/2015
 *******************************/

/********************************************
 * Project Description: 
 * Create a "Dropbox/SugarSync/GoogleDrive/MS/Apple..-like system to securely store files "in the cloud" (really on
 * the server). Users will place unencrupted (plain) files in a special directory, where they will be encrypted and
 * then sent to a server. Users will be able to selectively, or as a group retrieve (and decrypt) those files later.
 * You should provide both a client side and server side of you implementation
 * 
 * Features to be implemented.
 * 1.) Put File to server
 * 2.) Get File from server
 * 3.) Encrypt file before putting it to server
 * 4.) Decrypt file after downloaded from server
 * 
 * Must first run server then client. Server will wait for client to handshake. All interaction done from client side
 *
 ********************************************/
public class Server extends Thread{
 private ServerSocket serverSocket;
    
    public Server(int port) throws IOException
    {
       serverSocket = new ServerSocket(port);
       serverSocket.setSoTimeout(1000000);
    }

    public void run()
    {
       
       String clientSignal="";
       while(true)
       {
         
         try{
         Socket server = serverSocket.accept();
         DataInputStream in =new DataInputStream(server.getInputStream());
         clientSignal=in.readUTF();
         
         System.out.println("Client Signal is:"+clientSignal);
         if(clientSignal.equals("Send")){
           this.sendFromClient();
         }else if(clientSignal.equals("Get")){
           String tempFileName="";
           tempFileName=in.readUTF();
           System.out.println("In Get method!");
           this.getFromServer(tempFileName,server);
         }else{
          //need to do something with error if not sending 
           
         }
         server.close();
         }catch(IOException e){
             e.printStackTrace();
             break;
        }
         
         
         
       }
    }
    
   public void sendFromClient(){
      String fileNameServer="serverFile.txt";
      File myFile=new File("Server\\"+fileNameServer);
       try{
         Socket sock=serverSocket.accept();
         byte[] mybytearray = new byte[1024];
         InputStream is=sock.getInputStream();
         FileOutputStream fos=new FileOutputStream("Server\\"+fileNameServer);
         BufferedOutputStream bos= new BufferedOutputStream(fos);
         int bytesRead= is.read(mybytearray ,0,mybytearray .length);
         System.out.println("Size:"+mybytearray.length);
         bos.write(mybytearray,0,bytesRead);
         bos.close();
         sock.close();
         
         }catch(IOException e){
             e.printStackTrace();
         }
       
    }
   
   public int getFromServer(String filename,Socket server){
    //Get local file information
     System.out.println("Got the File name:"+filename);
   File f = new File("Server\\"+filename);
   if (!f.exists() || !f.isFile()) {
          System.out.println("File doesn\'t exist");
          return -1;
       }
   //put file into byte array to send over socket
   try{
   byte[] fileByte = new byte[(int) f.length()];
   BufferedInputStream bis = new BufferedInputStream(new FileInputStream(f));
   bis.read(fileByte,0,fileByte.length);
   OutputStream os=server.getOutputStream();
   os.write(fileByte,0,fileByte.length);
   os.flush();
   }catch (IOException e) {
      System.out.println(e);
   }
   
   return 0;
   }
    
    
    
      
    
    public static void main(String [] args)
    {
       int port = 6066;
       try
       {
          Thread t = new Server(port);
          t.start();
       }catch(IOException e)
       {
          e.printStackTrace();
       }
    } 
    
 }

