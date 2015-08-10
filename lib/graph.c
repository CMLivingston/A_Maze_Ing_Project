
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "myList.h"
#include "math.h"

XYPos wallPosition = { -1, -1 };
GraphNode WALL = { &wallPosition, 1, 0, 1, NULL, NULL, NULL, NULL };

/* initialize graph given width and height */
Graph* createGraph(int width, int height) {

	/* initialize graph */
	Graph *newGraph = (Graph *)malloc(sizeof(Graph));
	newGraph->nodes = (GraphNode***)malloc(width*sizeof(GraphNode**));
	for(int i=0; i < width; i++){
	  newGraph->nodes[i] = (GraphNode**)malloc(height*sizeof(GraphNode*));
	}
	newGraph -> width = width;
	newGraph -> height = height;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			/* declare node */
			GraphNode *newNode = (GraphNode*)malloc(sizeof(GraphNode));

			/* initialize position */
			XYPos *newPosition = (XYPos *)malloc(sizeof(XYPos));
			newPosition -> x = i;
			newPosition -> y = j;

			/* initialize node */
			newNode -> position = newPosition;
			newNode -> visited = 0;
			newNode -> current = 0;
			newNode -> deadEnd = 0;
			newNode -> north = NULL;
			newNode -> east = NULL;
			newNode -> south = NULL;
			newNode -> west = NULL;

			/* surround the graph with wall */
			if (i == 0) {
				newNode -> west = &WALL;
			}

			if (i == width - 1) {
				newNode -> east = &WALL;
			}

			if (j == 0) {
				newNode -> north = &WALL;
			}

			if (j == height - 1) {
				newNode -> south = &WALL;
			}

			/* store node in corresponding slot of array */
			newGraph -> nodes[i][j] = newNode;
		}
	}

	return newGraph;
}

/* free memory allocated for graph */
int clearGraph(Graph *graph) {
	int width = graph -> width;
	int height = graph -> height;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			free(graph -> nodes[i][j] -> position);
			free(graph -> nodes[i][j]);
		}
		free(graph->nodes[i]);
	}
	free(graph->nodes);
	free(graph);

	return 1;
}

/* initial placement of avatar */
int initialVisitNode(int x, int y, Graph *graph) {

	/* visit node */
	GraphNode *node = graph -> nodes[x][y];
	node -> visited = 1;
	node -> current = 1;

	return 1;
}

/* movement from one node to another */
int visitNode(GraphNode *prev, int direction, Graph *graph) {

	/* x and y coordinates of the previous node */
	int prevX = prev -> position -> x;
	int prevY = prev -> position -> y;

	/* previous node is no longer the current node */
	prev -> current = 0;

	/* the node to move to */
	GraphNode *next;

	switch (direction) {
		case 0:
			next = graph -> nodes[prevX - 1][prevY];
			prev -> west = next;
			next -> east = prev;
			next -> visited = next -> visited + 1;
			next -> current = 1;
			break;
		case 1:
			next = graph -> nodes[prevX][prevY - 1];
			prev -> north = next;
			next -> south = prev;
			next -> visited = next -> visited + 1;
			next -> current = 1;
			break;
		case 2:
			next = graph -> nodes[prevX][prevY + 1];
			prev -> south = next;
			next -> north = prev;
			next -> visited = next -> visited + 1;
			next -> current = 1;
			break;
		case 3:
			next = graph -> nodes[prevX + 1][prevY];
			prev -> east = next;
			next -> west = prev;
			next -> visited = next -> visited + 1;
			next -> current = 1;
			break;
	}

	return 1;
}

/* add WALL node between two nodes */
int addWall(GraphNode *prev, int direction, Graph *graph) {
	
	/* x and y coordinates of the previous node */
	int prevX = prev -> position -> x;
	int prevY = prev -> position -> y;

	/* the node across the wall */
	GraphNode *next;

	switch (direction) {
		case 0:
			prev -> west = &WALL;
			if (prevX > 0) {
				next = graph -> nodes[prevX - 1][prevY];
				next -> east = &WALL;
			}
			break;
		case 1:
			prev -> north = &WALL;
			if (prevY > 0) {
				next = graph -> nodes[prevX][prevY - 1];
				next -> south = &WALL;
			}
			break;
		case 2:
			prev -> south = &WALL;
			if (prevY < graph -> height - 1) {
				next = graph -> nodes[prevX][prevY + 1];
				next -> north = &WALL;
			}
			break;
		case 3:
			prev -> east = &WALL;
			if (prevX < graph -> width - 1) {
				next = graph -> nodes[prevX + 1][prevY];
				next -> west = &WALL;
			}
			break;
	}

	return 1;
}

/* check if a node is at a dead end */
int isDeadEnd(GraphNode *node, Graph *graph) {

	int x = node -> position -> x;
	int y = node -> position -> y;

	/* count number of adjacent walls */
	int wallCount = 0;

	if (node -> north == &WALL || (y > 0 && graph -> nodes[x][y - 1] -> deadEnd == 1)) {
		wallCount++;
	}

	if (node -> east == &WALL || (x < graph -> width - 1 && graph -> nodes[x + 1][y] -> deadEnd == 1)) {
		wallCount++;
	}

	if (node -> south == &WALL || (y < graph -> height - 1 && graph -> nodes[x][y + 1] -> deadEnd == 1)) {
		wallCount++;
	}

	if (node -> west == &WALL || (x > 0 && graph -> nodes[x - 1][y] -> deadEnd == 1)) {
		wallCount++;
	}

	/* return 1 if dead end, 0 if not */
	if (wallCount > 2) {
		node -> deadEnd = 1;
		return 1;
	} else {
		return 0;
	}
}

/* escapes dead-end and creates a wall to block that node */
int blockDeadEnd(GraphNode *current, Graph *graph) {

	int x = current -> position -> x;
	int y = current -> position -> y;

	GraphNode *westNode;
	GraphNode *northNode;
	GraphNode *southNode;
	GraphNode *eastNode;

	if (x > 0) {
		westNode = graph -> nodes[x - 1][y];
	}

	if (y > 0) {
		northNode = graph -> nodes[x][y - 1];
	}

	if (y < graph -> height - 1) {
		southNode = graph -> nodes[x][y + 1];
	}

	if (x < graph -> width - 1) {
		eastNode = graph -> nodes[x + 1][y];
	}

	/* if current node is not a dead-end, return NULL */
	if (!isDeadEnd(current, graph)) {
		return -1;
	}

	/* move out of node, block with wall, and return direction */
	if (current -> west != &WALL && westNode -> deadEnd != 1) {
		return 0;
	}

	else if (current -> north != &WALL && northNode -> deadEnd != 1) {
		return 1;
	}

	else if (current -> south != &WALL && southNode -> deadEnd != 1) {
		return 2;
	}

	else if (current -> east != &WALL && eastNode -> deadEnd != 1) {
		return 3;
	}

	/* shouldn't happen */
	return -3;
}
