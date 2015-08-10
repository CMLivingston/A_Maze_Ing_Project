#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <time.h>
#include <math.h>
// ---------------- Local includes  e.g., "file.h"
#include "./../lib/amazing.h"               // Definition for error messages, etc
#include "./../lib/common.h"                // Additional definitions, constants
#include "./../lib/myList.h"                // List structure
#include "./../lib/graph.h"                 // Graph structure
#include "./../lib/avatar_func.h"           // Functions used by avatar
#include "./../lib/hashTable.h"             // Hashtable


// Struct used to send messages to the graphics program

int main (int argc, char *argv[]) {
  //Create a log file
  char* FileName = (char*)calloc(20, sizeof(*FileName));
  VAL_CHECK(stderr, FileName);
  sprintf(FileName, "./LibTest.log"); 

  // Calculate the current time
  time_t CurrentTime;
  char* Date;
  CurrentTime = time(NULL);
  Date = ctime(&CurrentTime);
  VAL_CHECK(stderr, Date);

  // Create the logfile
  FILE* fp = fopen(FileName, "w");
  VAL_CHECK(stderr, fp);
  fprintf(fp, "UID : %d, Date: %s", getuid(), Date);
  free(FileName);

  fprintf(fp, "We start the test of files in the library. First, we test the list.\n");
  fprintf(fp,"-----------------------------------------------------------------------");
  fprintf(fp, "Create three new lists, list1, list2, list3, and a combined list\n");
  // new list
  List *list1 = newList();
  List *list2 = newList();
  List *list3 = newList();
  List *combinedList;

  fprintf(fp, "Also create a new graph, whose nodes will be stores in the list.\n");
  // new graph
  Graph *graph = createGraph(3, 4);
  
  fprintf(fp, "We try push and addToHead, which will each add to end of the list and head of the list\n");
  // push to different lists
  push(graph -> nodes[1][1], list1);
  push(graph -> nodes[2][2], list2);
  push(graph -> nodes[2][2], list3);
  push(graph -> nodes[1][3], list1);
  addToHead(graph -> nodes[1][2], list1);
  push(graph ->nodes[1][2], list2);
  push(graph->nodes[0][2], list2);
  
  fprintf(fp, "We Check if any three of the list are connected. (They should not be : )\n");
  // see if any are connected
  fprintf(fp,"%d\n", isConnected(list1, list2));
  fprintf(fp,"%d\n", isConnected(list1, list3));
  fprintf(fp,"%d\n", isConnected(list3, list2));
  
  fprintf(fp, "We comebine List 1 and 3.\n");
  // combine list1 and list3
  combinedList = combine(list1, list3);
  
  fprintf(fp, "We check if the list was combined correctly. It should be : (1,2), (2,2).\n");
  // combined correctly? 
  fprintf(fp,"(%d, %d)\n", combinedList -> head -> node -> position -> x, combinedList -> head -> node -> position -> y);
  fprintf(fp,"(%d, %d)\n", combinedList -> tail -> node -> position -> x, combinedList -> tail -> node -> position -> y);

  fprintf(fp, "We now check if the combined list and list 2 are connected. (They should be).\n");
  // now connected? 
  fprintf(fp,"%d\n", isConnected(combinedList, list2));
  fprintf(fp, "We finish testing for list by cleaning up all the memory\n");
  
  // clean up 
  destroyList(list1);
  destroyList(list2);
  destroyList(list3);

  clearGraph(graph);
  
  fprintf(fp,"Testing for list was successful\n");
  fprintf(fp,"----------------------------------------------------------\n");


  fprintf(fp,"We now test Graph\n");
  fprintf(fp,"----------------------------------------------------------\n");

  
  fprintf(fp, "We first create a Graph\n");
  graph = createGraph(9, 9);
	  
  fprintf(fp, "We set initial nodes\n");
  initialVisitNode(1, 7, graph);
  initialVisitNode(3, 2, graph);
  initialVisitNode(4, 6, graph);


  fprintf(fp, "Creates 3 lists and sets up graph \n");
  List *list0 = newList();
  list1 = newList();
  list2 = newList();

  int connected0[3] = {1, 0, 0};
  int connected1[3] = {0, 1, 0};
  int connected2[3] = {0, 0, 1};

  List *lists[3] = {list0, list1, list2};

  push(graph -> nodes[1][7], list0);
  push(graph -> nodes[3][2], list1);
  push(graph -> nodes[4][6], list2);

  addWall(graph -> nodes[1][7], 3, graph);
  addWall(graph -> nodes[3][3], 2, graph);
  addWall(graph -> nodes[3][3], 0, graph);
  addWall(graph -> nodes[3][3], 3, graph);
  addWall(graph -> nodes[2][2], 0, graph);
  addWall(graph -> nodes[2][2], 2, graph);
  addWall(graph -> nodes[4][2], 3, graph);
  addWall(graph -> nodes[4][2], 2, graph);
  addWall(graph -> nodes[1][6], 3, graph);
  addWall(graph -> nodes[2][6], 3, graph);
  //addWall(graph -> nodes[3][6], 1, graph);


  fprintf(fp,"Compute the closes unvisited position near (1,7) : Should be (4,6)\n");
  XYPos *nearest = nearestUnvisitedPosition(graph -> nodes[1][7] -> position, 0, 3, lists, connected0);
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  fprintf(fp,"Decide which direction to go to get there. : \n");
  int finalDirection = move(graph -> nodes[1][7] -> position, nearest, graph, 0);
  fprintf(fp,"first direction: %d\n", finalDirection);
  visitNode(graph -> nodes[1][7], finalDirection, graph);
  push(graph -> nodes[1][6], list0);

  fprintf(fp,"Compute the closes unvisited position near (3,2) : Should be (4,6)\n");
  nearest = nearestUnvisitedPosition(graph -> nodes[3][2] -> position, 1, 3, lists, connected1);
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[3][2] -> position, nearest, graph, finalDirection);
  fprintf(fp,"second direction: %d\n", finalDirection);
  visitNode(graph -> nodes[3][2], finalDirection, graph);
  push(graph -> nodes[3][3], list1);


  fprintf(fp,"Compute the closes unvisited position near (1,7) : Should be (1,6 )\n");
  nearest = nearestUnvisitedPosition(graph -> nodes[4][6] -> position, 2, 3, lists, connected2);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[4][6] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[4][6] -> position, nearest, graph, finalDirection);
  fprintf(fp,"third direction: %d\n", finalDirection);
  visitNode(graph -> nodes[4][6], finalDirection, graph);
  push(graph -> nodes[3][6], list2);


  fprintf(fp,"Compute the closes unvisited position near (1,7) : Should be (3,6 )\n");
  nearest = nearestUnvisitedPosition(graph -> nodes[1][6] -> position, 0, 3, lists, connected0);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[1][6] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[1][6] -> position, nearest, graph,finalDirection);
  fprintf(fp,"first direction: %d\n", finalDirection);
  visitNode(graph -> nodes[1][6], finalDirection, graph);
  push(graph -> nodes[1][5], list0);


  fprintf(fp,"Compute the closes unvisited position near (1,7) : Should be (1,5 )\n");
  nearest = nearestUnvisitedPosition(graph -> nodes[3][3] -> position, 1, 3, lists, connected1);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[3][3] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  visitNode(graph -> nodes[3][3], finalDirection, graph);
  finalDirection = move(graph -> nodes[3][3] -> position, nearest, graph,finalDirection);
  fprintf(fp,"second direction: %d\n", finalDirection);
  push(graph -> nodes[3][2], list1);
  fprintf(fp,"%d\n", graph -> nodes[3][2] -> south -> position -> x);


  fprintf(fp,"Compute the closes unvisited position near (1,7) : Should be (1,6 )\n");
  nearest = nearestUnvisitedPosition(graph -> nodes[3][6] -> position, 2, 3, lists, connected2);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[3][6] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[3][6] -> position, nearest, graph,finalDirection);
  fprintf(fp,"third direction: %d\n", finalDirection);
  visitNode(graph -> nodes[3][6], finalDirection, graph);
  push(graph -> nodes[3][5], list2);


  fprintf(fp,"Compute the closes unvisited position near (1,7) : Should be (3,5 )\n");
  nearest = nearestUnvisitedPosition(graph -> nodes[1][5] -> position, 0, 3, lists, connected0);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[1][5] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[1][5] -> position, nearest, graph,finalDirection);
  fprintf(fp,"first direction: %d\n", finalDirection);
  visitNode(graph -> nodes[1][5], finalDirection, graph);
  push(graph -> nodes[2][5], list0);

  fprintf(fp,"Compute the closes unvisited position near (1,7) : Should be (3,5 )\n");
  nearest = nearestUnvisitedPosition(graph -> nodes[3][2] -> position, 1, 3, lists, connected1);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[3][2] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[3][2] -> position, nearest, graph,finalDirection);
  fprintf(fp,"second direction: %d\n", finalDirection);
  visitNode(graph -> nodes[3][2], finalDirection, graph);
  push(graph -> nodes[4][2], list1);

  fprintf(fp,"Compute the closes unvisited position near (1,7) : Should be (2,5 )\n");
  nearest = nearestUnvisitedPosition(graph -> nodes[3][5] -> position, 2, 3, lists, connected2);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[3][5] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[3][5] -> position, nearest, graph,finalDirection);
  fprintf(fp,"third direction: %d\n", finalDirection);
  visitNode(graph -> nodes[3][5], finalDirection, graph);
  push(graph -> nodes[2][5], list2);

  //list0 = combine(list0, list2);
  //list2 = combine(list0, list2);
  connected0[2] = 1;
  connected2[0] = 1;

  fprintf(fp,"We test similar cases as following: \n");
  nearest = nearestUnvisitedPosition(graph -> nodes[2][5] -> position, 0, 3, lists, connected0);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[2][5] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[2][5] -> position, nearest, graph,finalDirection);
  fprintf(fp,"first direction: %d\n", finalDirection);
  visitNode(graph -> nodes[2][5], finalDirection, graph);
  push(graph -> nodes[2][4], list0);


  nearest = nearestUnvisitedPosition(graph -> nodes[4][2] -> position, 1, 3, lists, connected1);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[4][2] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[4][2] -> position, nearest, graph,finalDirection);
  fprintf(fp,"second direction: %d\n", finalDirection);
  visitNode(graph -> nodes[4][2], finalDirection, graph);
  push(graph -> nodes[4][1], list1);


  nearest = nearestUnvisitedPosition(graph -> nodes[2][5] -> position, 2, 3, lists, connected2);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[2][5] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[2][5] -> position, nearest, graph,finalDirection);
  fprintf(fp,"third direction: %d\n", finalDirection);
  visitNode(graph -> nodes[2][5], finalDirection, graph);
  push(graph -> nodes[2][6], list2);


  nearest = nearestUnvisitedPosition(graph -> nodes[2][4] -> position, 0, 3, lists, connected0);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[2][4] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[2][4] -> position, nearest, graph,finalDirection);
  fprintf(fp,"first direction: %d\n", finalDirection);
  visitNode(graph -> nodes[2][4], finalDirection, graph);
  push(graph -> nodes[2][3], list0);


  nearest = nearestUnvisitedPosition(graph -> nodes[4][1] -> position, 1, 3, lists, connected1);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[4][1] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[4][1] -> position, nearest, graph,finalDirection);
  fprintf(fp,"second direction: %d\n", finalDirection);
  visitNode(graph -> nodes[4][1], finalDirection, graph);
  push(graph -> nodes[5][1], list1);


  nearest = nearestUnvisitedPosition(graph -> nodes[2][6] -> position, 2, 3, lists, connected2);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[2][6] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[2][6] -> position, nearest, graph,finalDirection);
  fprintf(fp,"third direction: %d\n", finalDirection);
  visitNode(graph -> nodes[2][6], finalDirection, graph);
  push(graph -> nodes[2][7], list2);


  nearest = nearestUnvisitedPosition(graph -> nodes[2][3] -> position, 0, 3, lists, connected0);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[2][3] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[2][3] -> position, nearest, graph,finalDirection);
  fprintf(fp,"first direction: %d\n", finalDirection);
  visitNode(graph -> nodes[2][3], finalDirection, graph);
  push(graph -> nodes[1][3], list0);


  nearest = nearestUnvisitedPosition(graph -> nodes[5][1] -> position, 1, 3, lists, connected1);
  fprintf(fp,"direction: %d\n", getDirection(graph -> nodes[5][1] -> position, nearest));
  fprintf(fp,"nearest unvisited position: (%d, %d)\n", nearest -> x, nearest -> y);
  finalDirection = move(graph -> nodes[5][1] -> position, nearest, graph,finalDirection);
  fprintf(fp,"second direction: %d\n", finalDirection);
  visitNode(graph -> nodes[5][1], finalDirection, graph);

  destroyList(list0);
  destroyList(list1);
  destroyList(list2);
  clearGraph(graph);
  fprintf(fp,"The functions for graph was carried out successfully!\n");
  fprintf(fp,"-----------------------------------------------------\n");
  fprintf(fp,"And now we test the avatar function, notably BFS\n");
  //AVATAR_func------------------------------------------

  fprintf(fp, "We first set up a graph.\n");
  graph = createGraph(10, 10);

  initialVisitNode(4, 2, graph);
  visitNode(graph -> nodes[4][2], 2, graph);
  visitNode(graph -> nodes[4][3], 2, graph);	
  visitNode(graph -> nodes[4][4], 2, graph);
  visitNode(graph -> nodes[4][5], 3, graph);
  visitNode(graph -> nodes[5][5], 3, graph);
  visitNode(graph -> nodes[6][5], 1, graph);

  addWall(graph -> nodes[4][3], 2, graph);

  visitNode(graph -> nodes[4][3], 0, graph);
  visitNode(graph -> nodes[3][3], 2, graph);
  visitNode(graph -> nodes[3][4], 3, graph);

  fprintf(fp,"The following is the BFS path:\n");
  List *path = bfs(graph -> nodes[4][2], graph -> nodes[6][4], 10, 10);
  if (path == NULL) {
    fprintf(fp,"There is no path.\n");
  } else {
    while (!isEmpty(path)) {
      GraphNode *node = pop(path);
      fprintf(fp,"(%d, %d)\n", node -> position -> x, node -> position -> y);
    }
  }

  destroyList(path);
  clearGraph(graph);
  fprintf(fp," unit testing for shortest path node\n");

  Graph *testMaze = createGraph(10, 10);

  initialVisitNode(3, 3, testMaze);
  initialVisitNode(6, 1, testMaze);
  initialVisitNode(2, 9, testMaze);
  
  visitNode(testMaze->nodes[6][1], 2, testMaze);
  visitNode(testMaze->nodes[6][2], 2, testMaze);
  visitNode(testMaze->nodes[6][3], 2, testMaze);
  visitNode(testMaze->nodes[6][4], 2, testMaze);
  visitNode(testMaze->nodes[6][5], 2, testMaze);
  visitNode(testMaze->nodes[6][6], 2, testMaze);
  visitNode(testMaze->nodes[6][7], 2, testMaze);	
  visitNode(testMaze->nodes[6][8], 2, testMaze);
  
  
  visitNode(testMaze->nodes[3][3], 3, testMaze);
  visitNode(testMaze->nodes[4][3], 3, testMaze);
  visitNode(testMaze->nodes[5][3], 3, testMaze);
  visitNode(testMaze->nodes[6][3], 3, testMaze);
  visitNode(testMaze->nodes[7][3], 3, testMaze);
  visitNode(testMaze->nodes[8][3], 3, testMaze);
     
  visitNode(testMaze->nodes[2][9], 1, testMaze);
  visitNode(testMaze->nodes[2][8], 1, testMaze);
  visitNode(testMaze->nodes[2][7], 3, testMaze);
  visitNode(testMaze->nodes[3][7], 3, testMaze);
  visitNode(testMaze->nodes[4][7], 3, testMaze);
  visitNode(testMaze->nodes[5][7], 3, testMaze);
  visitNode(testMaze->nodes[6][7], 3, testMaze);
  visitNode(testMaze->nodes[7][7], 3, testMaze);
  
  clearGraph(testMaze);
  
  fprintf(fp, "The functions for Avatar was testeed successfully.\n");
  //----------------------------test for Hashtable
  fprintf(fp,"---------------------------------------------------------------------------\n");
  fprintf(fp,"The following is the test for hashtables\n");

  GraphNode* A=calloc(1,sizeof(*A));
  GraphNode* B=calloc(1,sizeof(*B));
  GraphNode* C=calloc(1,sizeof(*C));
  A->position = calloc(1, sizeof(XYPos));
  B->position = calloc(1, sizeof(XYPos));
  C->position = calloc(1, sizeof(XYPos));
  A->position->x=1; A->position->y=1;
  B->position->x=1; B->position->y=2;
  C->position->x=2; C->position->y=2;

  HashTable* table = calloc(1,sizeof(*table));
  setSize(5,5);
  addToHash(B,A,table);
  addToHash(C,B,table);
  
  GraphNode* result = peekLastPoint(B,table);
  GraphNode* result2 = peekLastPoint(C,table);
  fprintf(fp,"Should be A(1,1) : (%d,%d)\n", result->position->x, result->position->y);
  fprintf(fp,"Should be B(1,2) : (%d,%d)\n", result2->position->x, result2->position->y);

  destroyTable(table);

  free(A->position);
  free(B->position);
  free(C->position);
  free(A);
  free(B);
  free(C);
  free(result);
  free(result2);
  fprintf(fp,"Everything was completed successfully\n");

  return 0;
  
}
