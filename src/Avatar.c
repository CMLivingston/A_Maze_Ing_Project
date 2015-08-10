#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>

// ---------------- Local includes  e.g., "file.h"
#include "./../lib/amazing.h"               		// Definition for error messages, etc
#include "./../lib/common.h"                		// Additional definitions, constants
#include "./../lib/myList.h"                		// List structure
#include "./../lib/graph.h"                 		// Graph structure
#include "./../lib/shmAvatar.h"                 	// Avatar IPC functions
#include "./../lib/shmAMStartup.h"
#include "./../lib/avatar_func.h"


// Struct used to send messages to the graphics program
typedef struct my_msg_st{
	long mtype;
	int info[5];
} my_msg_st;


int AvatarID;
int nAvatars; 						// Saves the number of avatars
int Difficulty; 					// Saves the difficulty
int MazePort; 						// Saves the TCP/IP Port Number
int MazeWidth;
int MazeHeight;
List** lists; 						// Array of lists of all the avatars
Graph* mazeGraph; 					// graph struct to represent the maze
int* keyArray; 						// Array of keys allowing avatars to access their move structs 
int* firstRun;

my_msg_st dataMsg;
int msgid;

void sendMsg();						// sends a message to the graphics
void makeAllLists(); 					// Makes the linked lists for each avatar
void clearAllLists(); 					// Frees all the memory used for lists
void updateLists(AM_Message msg, Graph *maze); 		//Updates all the positions of each avatar in the lists
int updateGraph(int i, int xpos, int ypos);		// Updates each avatars individual graph with information about the maze

int main (int argc, char *argv[]) {
 
	/* Set up a message queue */
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if (msgid == -1) {
		fprintf(stderr, "msgget failed with error at Avatar\n");
		exit(EXIT_FAILURE);
	}

	char *FileName = calloc(strlen(argv[6]) + 1, sizeof(char));
	strcpy(FileName, argv[6]);

	/* Open the file with the name */
	FILE* fp = fopen(FileName, "a");
	VAL_CHECK(stderr, fp);
	free(FileName);

	if (argc != 9) {
		fprintf(fp, "number of inputs: %d Wrong number of inputs for avatar, please check ", argc);
		exit(EXIT_FAILURE);
	}


	/*
 	 * set arguments to variables but skip thorough argument checking (defensive coding) because
	 * all the information is passed on from AMStartup.c and should have been checked beforehand
	 */
	AvatarID = (int)atol(argv[1]);
	nAvatars = (int)atol(argv[2]);
	Difficulty = (int)atol(argv[3]);
	MazeWidth = (int)atoi(argv[7]);
	MazeHeight = (int)atoi(argv[8]);
	
	/* make the linked list for each avatar */
	makeAllLists();

	/* make the graph to represent the maze for each avatar */
	mazeGraph = createGraph(MazeWidth, MazeHeight); 

	keyArray = getKeys(nAvatars); 			// the array of shm keys
 
	firstRun = malloc((sizeof(int)*nAvatars)); 	// flag to tell updateGraph if it is first run

	/* initialize flags that indicate the start */
	for (int i = 0; i < nAvatars; i++) {
		firstRun[i] = 0;
		setX(-1, i, keyArray, nAvatars);
		setY(-1, i, keyArray, nAvatars);
		setDir(-1, i, keyArray, nAvatars);
	}

	
	char *IP = calloc(1, strlen(argv[4]) + 1);
	strcpy(IP, argv[4]);

	printf("%s\n", IP);

	MazePort = (int)atol(argv[5]);
	

	struct sockaddr_in servaddr;
	int sockfd;

	/* 
	 * Create a socket for the client
	 * If sockfd < 0, there is an error in the creation of the socket
	 */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(fp, "Problem in creating the socket");
		exit(EXIT_FAILURE);
	}

	/* Connect to the Socket */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(MazePort); //convert to big-endian order
	free(IP);

	/* Connect the socket to the server */
	if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0){
		fprintf(fp, "Problem in connecting to the server\n");
		exit(EXIT_FAILURE);
	}


	/* create ready message and send to the server */
	AM_Message RSendMsg;
	RSendMsg.type = htonl(AM_AVATAR_READY);
	RSendMsg.avatar_ready.AvatarId = htonl(AvatarID);
	send(sockfd, &RSendMsg, sizeof(RSendMsg.avatar_ready)+sizeof(RSendMsg.type), 0);

	/* initialize the array that mark which avatars have connected paths
	 * to this one; initially only connected to itself */
	int connected[nAvatars];

	for (int i = 0; i < nAvatars; i++) {
		if (i == AvatarID) {
			connected[i] = 1;
		}

		else {
			connected[i] = 0;
		}
	}

	/* declare/initialize variables */
	int tracker = 0;
	int quit = 0;

	List *bfsPath;
	List *currents = newList();

	int check = 0;
	int begin = 1;
	int previousDir = 0;
	int moveDir;

	if (AvatarID == 1) {
		fprintf(fp, "\nStarting Amazing log for Difficulty: %d, nAvatars: %d\n", Difficulty, nAvatars);
		fprintf(fp, "------------------------------------------------\n\n");
	}
	
	/* repeat until at least one of the flags report a quit */
	while (quit != 1) {

		AM_Message ReceiveMsg;

		/* receive a response message (size = the max size of a message) */
		if (recv(sockfd, &ReceiveMsg, sizeof(ReceiveMsg.type)+sizeof(ReceiveMsg.avatar_turn), 0) <= 0){

			fprintf(fp, "The server terminated prematurely\n");
			exit(EXIT_FAILURE);

		} else {

			uint32_t Msg = ntohl(ReceiveMsg.type);

			switch (Msg) {

				case AM_AVATAR_TURN:

					/* Move only when it is the avatar's turn */
					if (ntohl(ReceiveMsg.avatar_turn.TurnId) == AvatarID) {

						/* current avatar's current position */	
						int xPosition = (int) ntohl(ReceiveMsg.avatar_turn.Pos[AvatarID].x);
						int yPosition = (int) ntohl(ReceiveMsg.avatar_turn.Pos[AvatarID].y);

						fprintf(fp, "Avatar %d\n", AvatarID);

						fprintf(fp, "Current Location: (%d, %d)\n\n", xPosition, yPosition);

						if (check == 0) {

							/* Update position of each Avatar */
							updateLists(ReceiveMsg, mazeGraph);
							
							for(int i = 0; i < nAvatars; i++){

								/* each avatar's current position */
								int xpos = (int) ntohl(ReceiveMsg.avatar_turn.Pos[i].x);
								int ypos = (int) ntohl(ReceiveMsg.avatar_turn.Pos[i].y);

								/* 
 								 * Update my representation of the maze;
								 * stored in mazeGraph graph with the 'ReceiveMsg'
								 */
								if (updateGraph(i, xpos, ypos) > 0){
									perror("Error in Update Graph");
								}

								/* all connected avatars should share the same list */
								if(AvatarID != i && connected[i] != 1 && isConnected(lists[AvatarID], lists[i])) {
									connected[i] = 1;

									List *temp = lists[AvatarID];
									List *temp2 = lists[i];

									List *combinedList = combine(lists[AvatarID], lists[i]);

									for (int j = 0; j < nAvatars; j++) {
										if (lists[j] == temp || lists[j] == temp2) {
											lists[j] = combinedList;
										}
									}
								}
							}

							/* first switch is on if all avatars are connected */
							check = 1;

							for(int i = 0; i <nAvatars; i++) {
								if(connected[i] == 0) {
									check = 0;
									break;
								}
							}

							/* compute the most viable distance via the complex move method */
							if (check == 0) {
								XYPos *nearest = nearestUnvisitedPosition(mazeGraph -> nodes[xPosition][yPosition] -> position, AvatarID, nAvatars, lists, connected);

								moveDir = move(mazeGraph -> nodes[xPosition][yPosition] -> position, nearest, mazeGraph, previousDir);

								/* mod 10 for blockDeadEnd */
								if (moveDir % 10 == 0) {
									previousDir = 0;
								}

								if (moveDir % 10 == 1) {
									previousDir = 1;
								}

								if (moveDir % 10 == 2) {
									previousDir = 2;
								}

								if (moveDir % 10 == 3) {
									previousDir = 3;
								}
							} else {
								/* NULL move */
								moveDir = 8;

							}
							
							/* if it's an escape move, let all the avatars know */
							setDir(moveDir, AvatarID, keyArray, nAvatars);

							if (moveDir > 9) {
								moveDir = moveDir - 10;
							}	
							

						} else if (check == 1) {

							/* a NULL move phase for all graphs to catch up to each other */

							moveDir = 8;
							check = 2;

							for (int i = 0; i < nAvatars; i++) {
								int xpos = (int) ntohl(ReceiveMsg.avatar_turn.Pos[i].x);
								int ypos = (int) ntohl(ReceiveMsg.avatar_turn.Pos[i].y);

								if (updateGraph(i, xpos, ypos) > 0) {
									perror("Error in updateGraph\n");
								}

							}
	
						} else if(check == 2) {

							/* another NULL move phase, now along with the shortest path calculation */

							/* push all the current nodes in a list */
							for (int i = 0; i < nAvatars; i++) {
								push(lists[i]->tail->node, currents);
							}

							moveDir = 8;
							int myX = getX(AvatarID, keyArray, nAvatars);
							int myY = getY(AvatarID, keyArray, nAvatars);
							setDir(moveDir, AvatarID, keyArray, nAvatars);

							/* compute breadth first search */
							bfsPath = bfs(mazeGraph->nodes[myX][myY], mazeGraph->nodes[(int) ntohl(ReceiveMsg.avatar_turn.Pos[1].x)][(int) ntohl(ReceiveMsg.avatar_turn.Pos[1].y)], MazeWidth, MazeHeight);
							
							/* final check */
							check = 3;
							
							/* update graph */
							for(int i = 0; i < nAvatars; i++) {
								int xpos = (int) ntohl(ReceiveMsg.avatar_turn.Pos[i].x);
								int ypos = (int) ntohl(ReceiveMsg.avatar_turn.Pos[i].y);

								/*
 								 * Update my representation of the maze
								 * stored in mazeGraph graph with the 'ReceiveMsg'
								 */
								if (updateGraph(i, xpos, ypos) > 0) {
									perror("Error in updateGraph\n");
								}
							}
								
						} else {

							/* follow the returned bfs path */
							if (!isEmpty(bfsPath)) {
								moveDir = getDirection(mazeGraph -> nodes[xPosition][yPosition] -> position, pop(bfsPath) -> position);
							} else {
								/* remain still if arrived */
								moveDir = 8;
							}

							/* update graph */
							for (int i = 0; i < nAvatars; i++){
								int xpos = (int) ntohl(ReceiveMsg.avatar_turn.Pos[i].x);
								int ypos = (int) ntohl(ReceiveMsg.avatar_turn.Pos[i].y);

								/*
								 * fprintf(fp,"avatar %d is at (%d,%d)\n",i,xpos,ypos);
								 * Update my representation of the maze
								 * stored in mazeGraph graph with the 'ReceiveMsg'
								 */
								if (updateGraph(i, xpos, ypos) > 0){
									perror("Error in Update Graph");
								}
							}

							setDir(moveDir, AvatarID, keyArray, nAvatars);
						}

						/* if not first turn, send a NULL move to get initial positions */
						if (begin == 1) {
							moveDir = 8;
							begin = 0;
							setDir(moveDir, AvatarID, keyArray, nAvatars);
						}

						/* set values in key array */
						setX(xPosition, AvatarID, keyArray, nAvatars);
						setY(yPosition, AvatarID, keyArray, nAvatars);

						/* send the avatar's move */
						AM_Message sendMove;
						sendMove.type = htonl(AM_AVATAR_MOVE);
						sendMove.avatar_move.AvatarId = htonl(AvatarID);
						sendMove.avatar_move.Direction = htonl(moveDir);
						send(sockfd, &sendMove, sizeof(sendMove.avatar_move)+sizeof(sendMove.type), 0);
						tracker++;
					}

					break;
				      
				case AM_MAZE_SOLVED:		             
					if (AvatarID == 1) {
						/* Save all the information in the message */
						sendMsg(AvatarID, AvatarID, 0, 0, 0, 4);
						fprintf(fp, "Mazed solved in %d moves!\n%d difficulty\n%d avatars\n", ntohl(ReceiveMsg.maze_solved.nMoves), ntohl(ReceiveMsg.maze_solved.Difficulty), ntohl(ReceiveMsg.maze_solved.nAvatars));
					}
					quit = 1;
					break;

				case AM_NO_SUCH_AVATAR:
					fprintf(fp, "The server could not recognize the avatar");
					quit = 1;
					break;

				case AM_UNKNOWN_MSG_TYPE:
					fprintf(fp, "The received an unknwon message type from the avatars");
					quit = 1;
					break;
 
				case AM_AVATAR_OUT_OF_TURN:
					if(AvatarID == 0) {
						fprintf(fp, "The avatar %d made a move when it was not its turn!\n", AvatarID);
					}
					quit = 1;
					break;

				case AM_TOO_MANY_MOVES:
					if(AvatarID == 0) { 
						fprintf(fp, "The avatar %d has made too many moves!\n", AvatarID);
					}
					quit = 1;
					break;

				case AM_SERVER_DISK_QUOTA:
					if(AvatarID == 0) { 
						fprintf(fp, "SERVER_DISK_QUOTA\n");
					}
					quit = 1;
					break;

				case AM_SERVER_OUT_OF_MEM:
					if(AvatarID == 0) { 
						fprintf(fp, "SERVER_OUT_OF_MEM\n");
					}
					quit = 1;
					break;

				case AM_SERVER_TIMEOUT:
					if(AvatarID == 0) { 
						fprintf(fp, "SERVER_TIMEOUT");
					}
					quit = 1;
					break;

				case AM_UNEXPECTED_MSG_TYPE:
					if(AvatarID == 0) { 
						fprintf(fp, "UNEXPECTED MSG TYPE");
					}
					break;

				default:
					fprintf(fp, "unknown:0x%08x\n",Msg);
					break;	
			}
			
			fflush(fp);

		}
	}

	/* clean up */
	if (AvatarID == 1) {
		fclose(fp);

		deallocShm(keyArray, nAvatars);
        	deallocKeys(nAvatars);
		msgctl(msgid, IPC_RMID,NULL);
	}
	close(sockfd);
	destroyList(lists[AvatarID]);
	free(firstRun);
        clearGraph(mazeGraph);
	//destroyList(bfsPath);
	freeKeys(keyArray);

	exit(0);
}

/* make the array of lists */
void makeAllLists() {
	/* allocate necessary memory for the array */
	lists = malloc(nAvatars * sizeof(List*));
	MEM_CHECK(stderr, lists);

	/* create a list for each avatar */
	for(int i=0;i<nAvatars; i++){
		lists[i] = newList();
	}
}

/* clears all the memory used for the list */
void clearAllLists(){
	for(int i=0; i< nAvatars; i++){
		destroyList(lists[i]);
	}
	free(lists);
}


/* update all the locations of the avatars in the list */
void updateLists(AM_Message msg, Graph *maze){

	/* go through the message and save all the avatar's location */
	for(int i=0; i<nAvatars; i++){
		int x = (int) ntohl(msg.avatar_turn.Pos[i].x);
		int y = (int) ntohl(msg.avatar_turn.Pos[i].y);

		GraphNode *tempGraphNode = maze->nodes[x][y];

		/* Add the element only if the avatar had moved */
		GraphNode* tempTail = peekTail(lists[i]);
		if(tempTail){
			if(tempTail->position->x != tempGraphNode->position->x || tempTail->position->y != tempGraphNode->position->y) {
				push(tempGraphNode, lists[i]);
			}
		} else { 
			push(tempGraphNode, lists[i]);
		}
	}
}

 /*
 * updateGraph - Parameters: the xpos and ypos inside the recieved result
 * message from the server.
 */
int updateGraph(int i, int xpos, int ypos){

	/* get values */
	int myX = getX(i, keyArray, nAvatars);
	int myY = getY(i, keyArray, nAvatars);
	int myDir = getDir(i, keyArray, nAvatars);

	GraphNode *node;

	/* get nodes if not first run; else call initial visit */
	if (firstRun[i] == 1) {
		node = mazeGraph -> nodes[myX][myY];
	} else {
		initialVisitNode(xpos, ypos, mazeGraph);
		sendMsg(AvatarID, i, xpos, ypos, myDir, 0);
		firstRun[i] = 1;
	}

	/*
  	 * Add the new visited graph node to each avatars graph
	 * only if the avatar has moved
	 */
	if (myDir != 8 && xpos == myX && ypos == myY) {
		addWall(node, myDir, mazeGraph);
		sendMsg(AvatarID, i, node->position->x, node->position->y, myDir, 3);

	} else {	
		switch (myDir) {

			/* normal cases */
			case 0:
				if (node -> west != &WALL) {
					visitNode(node, myDir, mazeGraph);
					sendMsg(AvatarID, i, myX, myY, myDir,1);
				}
				break;
			case 1:
				if (node -> north != &WALL) {
					visitNode(node, myDir, mazeGraph);
					sendMsg(AvatarID, i, myX, myY, myDir,1);
				}
				break;
			case 2:
				if (node -> south != &WALL) {
					visitNode(node, myDir, mazeGraph);
					sendMsg(AvatarID, i, myX, myY, myDir,1);
				}
				break;
			case 3:
				if (node -> east != &WALL) {
					visitNode(node, myDir, mazeGraph);
					sendMsg(AvatarID, i, myX, myY, myDir,1);
				}
				break;

			/* NULL move */
			case 8:
				sendMsg(AvatarID, i, myX, myY, myDir, 5);
				break;

			/* break away from dead end cases */
			case 10:
				visitNode(node, 0, mazeGraph);
				node -> west -> east = &WALL;
				sendMsg(AvatarID, i, myX, myY, 0,2);
				break;
			
			case 11:
				visitNode(node, 1, mazeGraph);
				node -> north -> south = &WALL;
				sendMsg(AvatarID, i, myX, myY, 1, 2);
				break;

			case 12:
				visitNode(node, 2, mazeGraph);
				node -> south -> north = &WALL;
				sendMsg(AvatarID, i, myX, myY, 2, 2);
				break;	

			case 13:
				visitNode(node, 3, mazeGraph);
				node -> east -> west = &WALL;
				sendMsg(AvatarID, i, myX, myY, 3, 2);
				break;	
		}
			
	}

	return 0; 
}


/*
  check : checks if it is updating its own location 
       (sends the message only if check == first)
  first : avatarID
  second : x loc 
  third :  y loc
  fourth : dir
  fifth : 0 (initial) 1 (move) 2 (block dead end) 3 (add wall) 4 (quit)

 */
void sendMsg(int check, int first, int second, int third, int fourth, int fifth){

	if(check == first){
		/* save all the information in the message */
		dataMsg.mtype = 1;
		dataMsg.info[0] = first;
		dataMsg.info[1] = second;
		dataMsg.info[2] = third;
		dataMsg.info[3] = fourth;
		dataMsg.info[4] = fifth; 

		/* Send the message to graphics via MessageQueue */
		if(msgsnd(msgid, (void*) &dataMsg, sizeof(my_msg_st) - sizeof(long), 0) == -1){
			fprintf(stderr, "msgsnd failed at Avatar with error : %d\n", errno);
			exit(EXIT_FAILURE);
		}
	}
}
