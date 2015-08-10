/* ========================================================================== */
/* File: AMStartup.c - Client Program that initiates the Maze Program
 *
 * Author: Cing is Believing
 * Date: February 2015
 *
 * Input:  Initial : -n [nAvatars] -d [Difficulty] -h [Hostname]
 *
 * Command line options: -n [nAvatars] : (int) specifies the number of Avatars in the maze
 *                       -d [Difficulty] : (int) the difficulty level, from 0 to 9
 *                       -h [Hostname] : (char*) hostname of server : pierce.cs.dartmouth.edu
 *
 * Output: 
 *
 * Error Conditions:
 *         1. Any one of the options (-n,-d,-h) not specified 
 *         2. [nAvatars] : should be an integer between 2 and AM_MAX_AVATAR
 *         3. [Difficulty] : should be an integer between 0 and AM_MAX_DIFFICULTY
 *         4. [Hostname] : in this program, should be 3
 *
 * Special Considerations: The pseudocode can be found in query documentation (pdf)
 *
 * Description : This file is a client program that initiates the Maze program
 *
 * Pseudocode : 
 *
 * v   Check arguments
 *        - Check Number of arguments
 *        - Check the validity of number of Avatars (check range, data type)
 *        - Check the validity of difficulty input (check range, data type)
 *        - Check the validity of Hostname (check if it equals HOSTNAME(common.h))
 *
 * v   Sends AM_INITmessage to the Server
 *        - Specify nAvatars, Difficulty
 *    
 * v   Check for received message 
 *        - IF : Message = AM_INIT_FAILED
 *               Print out error message for the user and exit
 *        - ELSEIF : Message =  AM_INIT_OK	
 *               Extract MazePort from message
 *                    Saves the MAZEPORT (TCP/IP Port Number)
 *               Starts listening to Mazeport
 *               Save MazeWidth, MazeHeight
 *
 * v   Creates a logfile : Amazing_$USER_N_D.log
 *        - First line : $USER, Mazeport, date & time
 *  
 *    Starts Avatar process depending on specified parameters (fork)
 *      => ./avatar [AvatarId] [nAvatars] [Difficulty] [IP] [MazePort] [FileName]
 *        - AvatarId : starts at 0, increments until nAvatars
 *        - nAvatars : total number of Avatars
 *        - Difficulty : difficulty of the maze
 *        - IP : IP address of the server
 *        - MazePort : the TCP/Ip port number
 *        - Filename of log : Avatar will open in apend Moded
 *   
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#define _GNU_SOURCE

#include <stdio.h>                           // printf
#include <sys/stat.h>                        // check directory functionality
#include <string.h>                          // string functionality
#include <unistd.h>                          // check directory avaialability
#include <stdlib.h>                          // to read from file and write to file
#include <ctype.h>                           // Used for arugment Checking
#include <sys/types.h>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
// ---------------- Local includes  e.g., "file.h"
#include "./../lib/amazing.h"               // Definition for error messages, etc
#include "./../lib/common.h"                // Additional definitions, constants
#include "./../lib/shmAMStartup.h"          // IPC Server Functions 

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
int nAvatars; // Saves the number of avatars
int Difficulty; // Saves the difficulty
char* HostName; // 
int MazePort; // Saves the TCP/IP Port Number
int MazeWidth; // saves the width of the maze
int MazeHeight; // saves the height of the maze
int* keyArray; // Array of keys allowing avatars to access their move structs 

// ---------------- Private prototypes


/* ========================================================================== */

int main (int argc, char *argv[]){

  // Check arguments
  // Check Number of arguments
  if(argc != 7){
    fprintf(stderr,"Please input : -n [Number of Avatars(2~10)] -d [Difficulty(0~9)] -h [Hostname]. \n");
    exit(EXIT_FAILURE);
  }
  
  int Opt;
  int NFlag=0,DFlag=0,HFlag=0;
  char* temp;
  while ((Opt = getopt (argc, argv, "n:d:h:")) != -1){

    switch(Opt){
    case 'n': // the input for the number of avatars
      temp = optarg;
      //Check if the argument is a number
      while (*temp){
	if(!isdigit(*temp++)){
	  fprintf(stderr,"Argument for -n is not a number!\n");
	  exit(EXIT_FAILURE);
	}
      }

      //Check if we have correct number of avatars
       nAvatars = (int)atol(optarg);
       if(nAvatars < 2 || nAvatars > AM_MAX_AVATAR){
	 fprintf(stderr, "Argument for -n should be between 2 and %d", AM_MAX_AVATAR);
	 exit(EXIT_FAILURE);
       }

      NFlag++;
      break;
    case 'd': // the input for the level of difficulty
      temp = optarg;
      //Check if the argument is a number
      while (*temp){
	if(!isdigit(*temp++)){
	  fprintf(stderr,"Argument for -d is not a number!\n");
	  exit(EXIT_FAILURE);
	}
      }

      //Check if we have correct difficulty
       Difficulty = (int)atol(optarg);
       if(Difficulty < 0 || Difficulty > AM_MAX_DIFFICULTY){
	 fprintf(stderr, "Argument for -d should be between 0 and %d", AM_MAX_DIFFICULTY);
	 exit(EXIT_FAILURE);
       }

      DFlag++;
      break;
    case 'h': // the input for the hostname
      //Check if the argument for hostname is correct
      HostName = (char*)calloc(strlen(optarg)+1,sizeof(*optarg));
      MEM_CHECK(stderr,HostName);
      strcpy(HostName, optarg);

      // For the program, the hostname should be same as predefined HOSTNAME
      if(strcmp(HostName, HOSTNAME)!=0){
	fprintf(stderr,"Please input %s as your argument", HOSTNAME);
	exit(EXIT_FAILURE);
      }
      HFlag++;                               
      break;
    case '?':
      perror("Please input : -n [Number of Avatars(2~10)] -d [Difficulty(0~9)] -h [Hostname]. \n");
      exit(EXIT_FAILURE);
      break;
    }

  }

  
  // Check if all the arguments were input
  if(NFlag == 0){
    perror("ERROR : You have not specified number of Avatars!\n");
    exit(EXIT_FAILURE);
  }else if(DFlag ==0){
    perror("ERROR : You have not specified the difficulty level!\n");
    exit(EXIT_FAILURE);
  }else if(HFlag ==0){
    perror("ERROR : You have not specified the Hostname!\n");
    exit(EXIT_FAILURE);
  }


  int sockfd;
  struct sockaddr_in servaddr;

  // Create a socket for the client
  // If sockfd<0 there was an error in the creation of the socket
  if((sockfd = socket (AF_INET, SOCK_STREAM,0))<0){
    perror("Problem in creating the socket");
    exit(EXIT_FAILURE);
  }


  // Get the IP address of the Host
  struct hostent* host;
  struct in_addr **addr_list;
  char IP[20];
  host = gethostbyname(HostName);
  free(HostName);

  // Attempt to obtain IP address of the HOst
  if(host  == NULL){ //If IP address is not retrieved, exit
    perror("Error while gethostbyname\n");
    exit(EXIT_FAILURE); 
  }
  else{//If IP address is obtained, save it
    addr_list = (struct in_addr **)host->h_addr_list;
    
    for(int i=0; addr_list[i]!=NULL; i++){
      strcpy(IP, inet_ntoa(*addr_list[i]));
    }
  }

  printf("IP : %s\n", IP);


  // Creation of the socket
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(IP);
  servaddr.sin_port = htons(AM_SERVER_PORT); //convert to big-endian order


  // Connection to the socket
  if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr))<0){
    perror("Problem in connecting to the server\n");
    exit(EXIT_FAILURE);
  }

  printf("Connected to server\n");
  printf("Requesting with %d avatars, %d difficulty\n", nAvatars, Difficulty);


  // Create a new message to send
  AM_Message SendMsg;
  SendMsg.type = htonl(AM_INIT);
  SendMsg.init.nAvatars = htonl(nAvatars);
  SendMsg.init.Difficulty = htonl(Difficulty);
  send(sockfd, &SendMsg, sizeof(SendMsg.type) + sizeof(SendMsg.init),0);
 
  //Receive a message back
  AM_Message ReceivedMsg;
  if(recv(sockfd, &ReceivedMsg, AM_MAX_MESSAGE,0)==0){
    perror("The server terminated prematurely");
    exit(EXIT_FAILURE);
  }

  
  //Determine whether the init was successful or not
  uint32_t Msg = ntohl(ReceivedMsg.type);
  if(Msg == AM_INIT_OK){ // If init was successful, save the variables
    printf("Init Success!\n");
    MazeWidth = ntohl(ReceivedMsg.init_ok.MazeWidth);
    MazeHeight = ntohl(ReceivedMsg.init_ok.MazeHeight);
    MazePort = ntohl(ReceivedMsg.init_ok.MazePort);
    printf("MazePort : %d, MazeWidth : %d, MazeHeight : %d\n", MazePort, MazeWidth, MazeHeight);
  }else if(Msg == AM_INIT_FAILED){ // If init was not successful, print out error code and return 
    fprintf(stderr,"Init Failed\n");
    fprintf(stderr,"Error code : 0x%x\n", ntohl(ReceivedMsg.init_failed.ErrNum));
    exit(EXIT_FAILURE);
  }else{
    fprintf(stderr,"Init returned undefined error");
    fprintf(stderr,"Error code : 0x%x\n", Msg);
  }

  // Create a log file and print out initial information

 
  // Allocate enough memory for fileName : use log to allocate just the right amount.
  // For example, if uid is 4 digits, log10(uid)=4
  int UID = getuid();
  int Size = 20+(int)log10((double)UID);
  char* FileName = (char*)calloc(Size, sizeof(*FileName));
  VAL_CHECK(stderr, FileName);
  sprintf(FileName, "./Amazing_%d_%d_%d.log", UID, nAvatars, Difficulty); 
  printf("Filename : %s\n", FileName);

  // Calculate the current time
  time_t CurrentTime;
  char* Date;

  CurrentTime = time(NULL);
  Date = ctime(&CurrentTime);
  VAL_CHECK(stderr, Date);

  // Create the logfile
  FILE* fp = fopen(FileName, "w");
  VAL_CHECK(stderr, fp);
  fprintf(fp, "UID : %d, MazePort : %d, %s", UID, MazePort, Date);
  fflush(fp);

  //Close the file  => might be used by inidividuals
  fclose(fp);  

  // change all the non-String, common arguments back into strings that can be passed on to Avatar command line

  // Argument1: AvatarID is unique to each Avatar so will be done in the loop

  // Argument2: number of avatars
  char NAvatars[5];
  snprintf(NAvatars, 5, "%d", nAvatars);
	
  // Argument3: difficulty
  char MazeDifficulty[5];
  snprintf(MazeDifficulty, 5, "%d", Difficulty);
	
  // Argument4: IP address is already in a form of a string
	
  // Argument5: MazePort
  char MPort[30];
  snprintf(MPort, 30, "%d", MazePort);

  // Argument6: FileName is already in a form of a string

  // Argument7: MazeWidth
  char Width[5];
  snprintf(Width, 5, "%d", MazeWidth);

  // Argument8: MazeHeight
  char Height[5];
  snprintf(Height, 5, "%d", MazeHeight);

  // Create and share a key array for avatars to access
  keyArray = createKeys(nAvatars);
  shareKeys(keyArray, nAvatars);
   
  // Allocate 'nAvatar' memory segments containing a single 'Move' struct
  // to reference last move made 
  size_t s =  (sizeof(Move));
  allocShm(keyArray, s, nAvatars);

 

  // create graphics
  int pid = fork();
  if(pid<0){
    fprintf(stderr, "Fork was unsuccessful");
  }
  else if(pid == 0){
    execl("./graphics", "./graphics", Width, Height, MazeDifficulty, NAvatars, NULL);
  }
  
  

  // Use fork to genereate nAvatars number of child processes
  for(int Avatar = 0; Avatar < nAvatars; Avatar++) {

    char Avatars[5];
    snprintf(Avatars, 5, "%d", Avatar);
		
    int pid = fork();

    // check for fork error
    if(pid < 0) {
      fprintf(stdout, "Fork was unsuccessful");
    }
		
    // Assumed that the executive file of Avatar is made in ../ (subject to change)
    // pid of a child process
    if(pid == 0) {
      fprintf(stdout, "making Avatar %d pid: %d\n", Avatar, pid);
      // change the child process to and instance of Avatar with given parameters
      execl("./avatar", "./avatar", Avatars, NAvatars, MazeDifficulty, IP, MPort, FileName, Width, Height, NULL);

      exit(0);
    }

    // pid of a parent process
    if (pid > 0) {

      /*			int status;

				if (wait(&status) < 0) {
				fprintf(stdout, "error has occured in the child process\n");
				}
      */
      //			else {
      //				fprintf(stdout, "child existed normally, exit status: %d\n", status);
      //			}
      sleep(1);
    }
  }
  
  free(FileName);

  return 0;
}
