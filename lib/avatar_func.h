#include "amazing.h"
#include "myList.h"
#include "graph.h"

List *bfs(GraphNode *source, GraphNode *goal, int MazeWidth, int MazeHeight);

GraphNode *closestNode(List *from, Graph *maze, int MazeWidth, int MazeHeight);

double getDist(XYPos* one, XYPos* two);

XYPos *nearestUnvisitedPosition(XYPos *currentPosition, int AvatarID, int nAvatars, List **lists, int *connected);

int getDirection(XYPos *source, XYPos *goal);

int sumAdjacentVisited(XYPos *position, int degree, Graph *graph);

int move(XYPos *currentPosition, XYPos *nearestPosition, Graph *graph, int previousDir);
