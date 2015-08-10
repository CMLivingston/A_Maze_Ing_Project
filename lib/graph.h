
#ifndef GRAPH_H
#define GRAPH_H

#include "amazing.h"

// structures/types
typedef struct GraphNode {
	XYPos *position;
	int visited;
	int current;
	int deadEnd;
	struct GraphNode *north;
	struct GraphNode *east;
	struct GraphNode *south;
	struct GraphNode *west;
} GraphNode;

typedef struct Graph {
	int width;
	int height;
	GraphNode*** nodes;
} Graph;

// contants
extern XYPos wallPosition;
extern GraphNode WALL;

// methods
Graph* createGraph(int width, int height);

int clearGraph(Graph *graph);

int initialVisitNode(int x, int y, Graph *graph);

int visitNode(GraphNode *prev, int direction, Graph *graph);

int addWall(GraphNode *prev, int direction, Graph *graph);

int isDeadEnd(GraphNode *node, Graph *graph);

int blockDeadEnd(GraphNode *current, Graph *graph);


#endif

