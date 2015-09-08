import java.net.*;
import java.io.*;
import java.nio.file.Files;

//needed for cryptography

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;
 
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;
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
public class Client {
 public static void main(String [] args)
    {
            menu();
    }
 
 static void menu(){
      int select=-1;
      BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
      while(true){
      System.out.println("0: Exit");
      System.out.println("1: Send File to Server--Will Encrypt Before Sending");
      System.out.println("2: Get File from server--WIll Decrypt after receving");
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
            String fileNameSending;
            System.out.println("What File do u want to send?");
            try{
             fileNameSending= br.readLine();             
             sendToServer(encryptFIle(fileNameSending));
                }catch(NumberFormatException nfe){
                 System.err.println("Invalid Format!\n");
                }catch(IOException nfee){
                 System.err.println("Something else went wrong");
                }
            
            break;
          case 2:
            System.out.println("What File do u want to get from Server?");
            String saveNameFS="";
            try{
             saveNameFS= br.readLine();
             decryptFIle(getFromServer(saveNameFS));             
                }catch(NumberFormatException nfe){
                 System.err.println("Invalid Format!\n");
                }catch(IOException nfee){
                 System.err.println("Something else went wrong");
                }
          break;
          
          case 3:
          String getFileName="";
           System.out.println("What is the name you want to save as?");
            
            try{
            getFileName= br.readLine();
            decryptFIle(getFileName);
            }catch(IOException nfee){
                System.err.println("Something else went wrong");
            }
            break;
                                            
        } 
      }
  }
 static int sendToServer(String fileName){
   //Get local file information
   File f = new File("Client\\"+fileName);
   if (!f.exists() || !f.isFile()) {
          System.out.println("File doesn\'t exist");
          return -1;
       }
   //put file into byte array to send over socket
   String serverName = "localHost";
   int port = 6066;
   //create initial connection for signal sending
   try{
   Socket client = new Socket(serverName, port);
   OutputStream outToServer = client.getOutputStream();
   DataOutputStream out = new DataOutputStream(outToServer);
   //Write Send signal so server knows what to do
   out.writeUTF("Send");
   client.close();
   }catch(IOException e) {
      System.out.println(e);
   }
   //Now send the data.
   try {
   Socket sock=new Socket(serverName,port);
   byte[] fileByte = new byte[(int) f.length()];
   BufferedInputStream bis = new BufferedInputStream(new FileInputStream(f));
   bis.read(fileByte,0,fileByte.length);
   OutputStream os=sock.getOutputStream();
   System.out.println("Size of Stream:"+fileByte);
   os.write(fileByte,0,fileByte.length);
   System.out.println("Size of Stream2222:"+fileByte);
   
   
   os.flush();
   sock.close();
   
   String serverResponse="";
   /************Connect to Server*************/
   
   
     
     
     /*     
     System.out.println("Connecting to " + serverName+ " on port " + port);
     Socket client = new Socket(serverName, port);
     System.out.println("Just connected to "+ client.getRemoteSocketAddress());
     OutputStream outToServer = client.getOutputStream();
     DataOutputStream out = new DataOutputStream(outToServer);
     //Write Send signal so server knows what to do
     out.writeUTF("Send");
     //Write the file in btye type now.
     outToServer.write(fileByte);
     //Create input stream from server
     InputStream inFromServer = client.getInputStream();
     DataInputStream in =new DataInputStream(inFromServer);
     serverResponse=in.readUTF();
     if(serverResponse.equals("Saved")){
       System.out.println("File Saved to Server");
     }
     client.close();
      */
    }catch (IOException e) {
      System.out.println(e);
   }
   return 1;
 }
 //Returns the name of the Decrypted File
 static String encryptFIle(String fileName){
   final String ALGORITHM = "AES";
   final String TRANSFORMATION = "AES";
   String tempD="";
   String key = "Mary has one cat";//must be 16bytes exactly
   //open file
   File f = new File("Client\\"+fileName);
   if (!f.exists() || !f.isFile()) {
          System.out.println("File doesn\'t exist");
          return "error";
    }
   
   try{
     Key secretKey=new SecretKeySpec(key.getBytes(),ALGORITHM);
     Cipher cipher = Cipher.getInstance(TRANSFORMATION);
     cipher.init(Cipher.ENCRYPT_MODE,secretKey);
     FileInputStream inputStream= new FileInputStream(f);
     byte[] inputBytes=new byte[(int)f.length()];
     inputStream.read(inputBytes);
     tempD="ENCRYPT_"+fileName;
     byte[] outputBytes=cipher.doFinal(inputBytes);
     FileOutputStream outputStream=new FileOutputStream("Client\\"+tempD);
     outputStream.write(outputBytes);
     System.out.println("test3");
     
     inputStream.close();
     outputStream.close();
     
    }catch (NoSuchPaddingException | NoSuchAlgorithmException
                | InvalidKeyException | BadPaddingException
                | IllegalBlockSizeException | IOException ex) {
            System.out.println("Error Decrypting"+ex);
    }
   
   return tempD;
 }
 
 static String decryptFIle(String fileName){
   final String ALGORITHM = "AES";
   final String TRANSFORMATION = "AES";
   String tempD="";
   String key = "Mary has one cat";//must be 16bytes exactly
   //open file
   System.out.println("Name of file:"+fileName);
   File f = new File("Client\\"+fileName);
   if (!f.exists() || !f.isFile()) {
          System.out.println("File doesn\'t exist");
          return "error";
    }
   System.out.println("Test1");
   try{
     Key secretKey=new SecretKeySpec(key.getBytes(),ALGORITHM);
     Cipher cipher = Cipher.getInstance(TRANSFORMATION);
     cipher.init(Cipher.DECRYPT_MODE,secretKey);
     FileInputStream inputStream= new FileInputStream(f);
     byte[] inputBytes=new byte[(int)f.length()];
     inputStream.read(inputBytes);
     tempD="DECRYPT_"+fileName;
     byte[] outputBytes=cipher.doFinal(inputBytes);
     FileOutputStream outputStream=new FileOutputStream("Client\\"+tempD);
     outputStream.write(outputBytes);
     System.out.println("test3");
     
     inputStream.close();
     outputStream.close();
     
    }catch (NoSuchPaddingException | NoSuchAlgorithmException
                | InvalidKeyException | BadPaddingException
                | IllegalBlockSizeException | IOException ex) {
            System.out.println("Error Decrypting"+ex);
    }
   
   return tempD;
 }
 
 static String getFromServer(String fileName){
   //Get local file information
   String fileFromServer="ReceivedFileFromServer.txt";
   
   //put file into byte array to send over socket
   String serverName = "localHost";
   int port = 6066;
   //create initial connection for signal sending
   try{
   Socket client = new Socket(serverName, port);
   OutputStream outToServer = client.getOutputStream();
   DataOutputStream out = new DataOutputStream(outToServer);
   //Write Send signal so server knows what to do
   out.writeUTF("Get");  
   out.writeUTF(fileName);
   //client.close();
   
   //Socket sock=new Socket(serverName,port);
   byte[] mybytearray = new byte[1024];
   InputStream is = client.getInputStream();
   FileOutputStream fos = new FileOutputStream("Client\\"+fileFromServer);
   BufferedOutputStream bos = new BufferedOutputStream(fos);
   int bytesRead = is.read(mybytearray, 0, mybytearray.length);
   bos.write(mybytearray, 0, bytesRead);
   bos.close();
   client.close();
   
   
    }catch (IOException e) {
      System.out.println(e);
   }
   return fileFromServer;
 }
 
 
}
