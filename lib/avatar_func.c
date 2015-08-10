#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "graph.h"
#include "myList.h"
#include "math.h"
#include "common.h"
/* returns the shortest path from one graph point to the other
 *
 * parameters:
 * 	1. GraphNode, source: the starting graphNode
 * 	2. GraphNode, goal: the goal graphNode
 *
 * return value:
 * 	a linked list that contains the shortest path from one graph node to the other 
 *
 * call example:
 * 	List *path = bfs(source, goal);
 *
 * special consideration:
 * 	note that it is the caller's responsibility to free up the malloc'ed memory this function returns
 *
 */
List *bfs(GraphNode *source, GraphNode *goal, int MazeWidth, int MazeHeight) {	

	/* set up/initialize variables used for bfs */
	setSize(MazeWidth, MazeHeight);

	List *frontierQueue = newList();
	push(source, frontierQueue);

	HashTable *backtracker = (HashTable *)malloc(sizeof(HashTable));
	GraphNode *nullNode = (GraphNode *)malloc(sizeof(GraphNode));
	addToHash(source, nullNode, backtracker);

	/* loop through the frontier until its empty */
	while (!isEmpty(frontierQueue)) {
		GraphNode *track = pop(frontierQueue);
		
		/* if we reached out goal node, then return the shortest path */
		if (track == goal) {
			List *shortestPath = newList();

			while (track != nullNode) {
				addToHead(track, shortestPath);
				track = peekLastPoint(track, backtracker);
			}
			/*push(goal, shortestPath);
			destroyTable(backtracker);
			destroyList(frontierQueue);
			free(nullNode);*/
			return shortestPath;
		}

		/* else, add the unvisitied adjacencies to the frontier */
		else {
			if (track -> north != NULL && track -> north != &WALL && peekLastPoint(track -> north, backtracker) == NULL) {
				push(track -> north, frontierQueue);
				addToHash(track -> north, track, backtracker);
			}

			if (track -> east != NULL && track -> east != &WALL && peekLastPoint(track -> east, backtracker) == NULL) {
				push(track -> east, frontierQueue);
				addToHash(track -> east, track, backtracker);
			}

			if (track -> south != NULL && track -> south != &WALL && peekLastPoint(track -> south, backtracker) == NULL) {
				push(track -> south, frontierQueue);
				addToHash(track -> south, track, backtracker);
			}

			if (track -> west != NULL && track -> west != &WALL && peekLastPoint(track -> west, backtracker) == NULL) {
				push(track -> west, frontierQueue);
				addToHash(track -> west, track, backtracker);
			}
		}
	}

	/* if we couldn't find the goal node, then return an empty list */
	return NULL;
}

/* return the euclidean distance between two nodes */
double getDist(XYPos* one, XYPos* two){
        double first = DIFFSQ(one->x, two->x);
        double second = DIFFSQ(one->y, two->y);
        return sqrt(first+second);
}

/*
 * If connected[i] == 1, then the current avatar is connected to avatar i. If 0, then current avatar is not connected to avatar i.
 * We should add this array onto Avatar.c later.
 */

/* return the nearest node that other avatars have visited and current avatar hasn't */
XYPos *nearestUnvisitedPosition(XYPos *currentPosition, int AvatarID, int nAvatars, List **lists, int *connected) {

        /* declare/initialize variables */
        List *notVisited = newList();
        XYPos *nearest;
        double shortestDistance = INFINITY;

        /* combine lists of unconnected avatars */
        for (int i = 0; i < nAvatars; i++) {
                if (connected[i] != 1) {
                        List *list = lists[i];

                        notVisited = combine(notVisited, list);
                }
        }

	/* iterate through the list and find nearest */
        ListNode *temp = notVisited -> head;

        while (temp != NULL) {
                double distance = getDist(currentPosition, temp -> node -> position);

                if (distance < shortestDistance) {
                        nearest = temp -> node -> position;
                        shortestDistance = distance;
                }

                temp = temp -> next;
        }

	/* clean up */
        destroyList(notVisited);

        return nearest;
}

/* find most viable direction */
int getDirection(XYPos *source, XYPos *goal) {

        /* compute x and y distances */
        int dx = goal -> x - source -> x;
        int dy = goal -> y - source -> y;

	/* if same position, return NULL move */
	if(dx ==  0 && dy == 0)
	  return 8;

        /* compute direction based on distances */
        if (dx >= 0) {
                if (dy >= 0) {
                        if (dx >= dy) {
                                return 3;
                        } else {
                                return 2;
                        }
                } else {
                        if (dx >= abs(dy)) {
                                return 3;
                        } else {
                                return 1;
                        }
                }
        } else {
                if (dy >= 0) {
                        if (abs(dx) >= dy) {
                                return 0;
                        } else {
                                return 2;
                        }
                } else {
                        if (abs(dx) >= abs(dy)) {
                                return 0;
                        } else {
                                return 1;
                        }
                }
        }

        /* this shouldn't happen */
        return -1;
}


/* This function decides the move for the avatar. Again, think of a better idea. 
 Also, find errors. */

/* final decision; returns direction */
int move(XYPos *currentPosition, XYPos *nearestPosition, Graph *graph, int previousDir) {

	/* declare/initialize variables */
        int currentX = currentPosition -> x;
        int currentY = currentPosition -> y;

	GraphNode *currentNode = graph -> nodes[currentX][currentY];
	GraphNode *westNode = NULL;
	GraphNode *northNode = NULL;
	GraphNode *southNode = NULL;
	GraphNode *eastNode = NULL;

	if (currentX > 0) {
		westNode = graph -> nodes[currentX - 1][currentY];
	}

	if (currentY > 0) {
		northNode = graph -> nodes[currentX][currentY - 1];
	}

	if (currentY < graph -> height - 1) {
		southNode = graph -> nodes[currentX][currentY + 1];
	}

	if (currentX < graph -> width - 1) {
		eastNode = graph -> nodes[currentX + 1][currentY];
	}

        /* if currently in dead-end, move out */
        if (isDeadEnd(currentNode, graph)) {
		int escapeDirection = blockDeadEnd(currentNode, graph);
		switch (escapeDirection) {
			case 0:
				return 10;
			case 1:
				return 11;
			case 2:
				return 12;
			case 3:
				return 13;
		}

                return -1; 
        }

        int direction = getDirection(currentPosition, nearestPosition);

        /* if nearest unvisited node is close enough, 
           try to move towards that direction */
        if (getDist(currentPosition, nearestPosition) < 1.1) {

                if (direction == 0 && currentNode -> west != &WALL) {
                        return direction;
                }

                if (direction == 1 && currentNode -> north != &WALL) {
                        return direction;
                }

                if (direction == 2 && currentNode -> south != &WALL) {
                        return direction;
                }

                if (direction == 3 && currentNode -> east != &WALL) {
                        return direction;
                }
        }

	/* try to move to a node that hasn't been visited,
 	 * preferably in the direction of the nearest unvisited node */
        switch (direction) {
                case 0:
                        if (currentNode -> west != &WALL && westNode -> visited == 0) {
                                return 0;
                        }

			if (nearestPosition -> y > currentPosition -> y) {
				if (currentNode -> north != &WALL && northNode -> visited == 0) {
					return 1;
				}

				if (currentNode -> south != &WALL && southNode -> visited == 0) {
					return 2;
				}
			} else {
				if (currentNode -> south != &WALL && southNode -> visited == 0) {
					return 2;
				}

				if (currentNode -> north != &WALL && northNode -> visited == 0) {
					return 1;
				}
			}

                        if (currentNode -> east != &WALL && eastNode -> visited == 0) {
                                return 3;
                        }

			break;

                case 1:
                        if (currentNode -> north != &WALL && northNode -> visited == 0) {
                                return 1;
                        }

			if (nearestPosition -> x > currentPosition -> x) {
				if (currentNode -> east != &WALL && eastNode -> visited == 0) {
					return 3;
				}

				if (currentNode -> west != &WALL && westNode -> visited == 0) {
					return 0;
				}
			} else {
				if (currentNode -> west != &WALL && westNode -> visited == 0) {
					return 0;
				}

				if (currentNode -> east != &WALL && eastNode -> visited == 0) {
					return 3;
				}
			}

			if (currentNode -> south != &WALL && southNode -> visited == 0) {
				return 2;
			}

                case 2:
                        if (currentNode -> south != &WALL && southNode -> visited == 0) {
                                return 2;
                        }

			if (nearestPosition -> x > currentPosition -> x) {
				if (currentNode -> east != &WALL && eastNode -> visited == 0) {
					return 3;
				}

				if (currentNode -> west != &WALL && westNode -> visited == 0) {
					return 0;
				}
			} else {
				if (currentNode -> west != &WALL && westNode -> visited == 0) {
					return 0;
				}

				if (currentNode -> east != &WALL && eastNode -> visited == 0) {
					return 3;
				}
			}

			if (currentNode -> north != &WALL && northNode -> visited == 0) {
				return 1;
			}

                case 3:
                        if (currentNode -> east != &WALL && eastNode -> visited == 0) {
                                return 3;
                        }

			if (nearestPosition -> y > currentPosition -> y) {
				if (currentNode -> north != &WALL && northNode -> deadEnd != 1 && northNode -> visited == 0) {
					return 1;
				}

				if (currentNode -> south != &WALL && southNode -> deadEnd != 1 && southNode -> visited == 0) {
					return 2;
				}
			} else {
				if (currentNode -> south != &WALL && southNode -> deadEnd != 1 && southNode -> visited == 0) {
					return 2;
				}

				if (currentNode -> north != &WALL && northNode -> deadEnd != 1 && northNode -> visited == 0) {
					return 1;
				}
			}

                        if (currentNode -> west != &WALL && westNode -> deadEnd != 1 && westNode -> visited == 0) {
                                return 0;
                        }

			break;

        }

	/* try to go to a direction that haven't been tried before */	
	if (direction == 0) {
		if (currentNode -> west == NULL) {
			return 0;
		}

		if (nearestPosition -> y > currentPosition -> y) {
			if (currentNode -> south == NULL) {
				return 2;
			}

			if (currentNode -> north == NULL) {
				return 1;
			}
		} else {
			if (currentNode -> north == NULL) {
				return 1;
			}

			if (currentNode -> south == NULL) {
				return 2;
			}
		}

		if (currentNode -> east == NULL) {
			return 3;
		}
	}

	if (direction == 1) {
		if (currentNode -> north == NULL) {
			return 1;
		}

		if (nearestPosition -> x > currentPosition -> x) {
			if (currentNode -> east == NULL) {
				return 3;
			}

			if (currentNode -> west == NULL) {
				return 0;
			}
		} else {
			if (currentNode -> west == NULL) {
				return 0;
			}

			if (currentNode -> east == NULL) {
				return 3;
			}
		}

		if (currentNode -> south == NULL) {
			return 2;
		}
	}


	if (direction == 2) {
		if (currentNode -> south == NULL) {
			return 2;
		}

		if (nearestPosition -> x > currentPosition -> x) {
			if (currentNode -> east == NULL) {
				return 3;
			}

			if (currentNode -> west == NULL) {
				return 0;
			}
		} else {
			if (currentNode -> west == NULL) {
				return 0;
			}

			if (currentNode -> east == NULL) {
				return 3;
			}
		}

		if (currentNode -> north == NULL) {
			return 1;
		}
	}

	if (direction == 3) {
		if (currentNode -> east == NULL) {
			return 3;
		}

		if (nearestPosition -> y > currentPosition -> y) {
			if (currentNode -> south == NULL) {
				return 2;
			}

			if (currentNode -> north == NULL) {
				return 1;
			}
		} else {
			if (currentNode -> north == NULL) {
				return 1;
			}

			if (currentNode -> south == NULL) {
				return 2;
			}
		}

		if (currentNode -> west == NULL) {
			return 0;
		}
	}


	/* try to hug the left wall */
	if (previousDir == 1) {
		if (currentNode -> west != &WALL) {
			return 0;
		}

		if (currentNode -> north != &WALL) {
			return 1;
		}

		if (currentNode -> east != &WALL) {
			return 3;
		}

		if (currentNode -> south != &WALL) {
			return 2;
		}
	}

	if (previousDir == 0) {
		if (currentNode -> south != &WALL) {
			return 2;
		}

		if (currentNode -> west != &WALL) {
			return 0;
		}

		if (currentNode -> north != &WALL) {
			return 1;
		}

		if (currentNode -> east != &WALL) {
			return 3;
		}
	}

	if (previousDir == 2) {
		if (currentNode -> east != &WALL) {
			return 3;
		}

		if (currentNode -> south != &WALL) {
			return 2;
		}

		if (currentNode -> west != &WALL) {
			return 0;
		}

		if (currentNode -> north != &WALL) {
			return 1;
		}
	}

	if (previousDir ==  3) {
		if (currentNode -> north != &WALL) {
			return 1;
		}

		if (currentNode -> east != &WALL) {
			return 3;
		}

		if (currentNode -> south != &WALL) {
			return 2;
		}

		if (currentNode -> west != &WALL) {
			return 0;
		}
	}

	/* if everything fails, return random number */
	return rand() % 4;
}
