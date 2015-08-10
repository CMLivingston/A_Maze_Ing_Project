/* ========================================================================== */
/* File: hashtable.h - Hashtable used to store paths of avatars
 *
 * Author: Cing is Believing
 * Date: February 2015
 *
 * Input: None
 *
 * Command line options: None
 *
 * Output: None
 *
 * Error Conditions:
 *
 * Special Considerations:
 *
 * Description : 
 *              Implements hashtable which each avatar has.
 *              Inputs a point, and output is the point from which one came to this point.
 *              For example, if avatar moved (1,1)->(1,2)->(2,2), input of (2,2) will give (1,2)
 *              If there are duplicates, they are saved in order in a list, first elements first.
 *              For example, if avatar moved (1,1)->(1,2)->(2,2)->(1.2)->(1,3),
 *              Input of (1,2) will give (1,1) - (2,2)
 *
 * Pseudocode : 
 *
 *    ---------
 *    | Input | --------> HASHTABLE
 *    |(XYPos)|              |     
 *    ---------              |
 *                           |
 *                           |
 *                       ----------   ----------
 *                       | Output |---| Output |
 *                       | (XYPos)|   | (XYPos)|
 *                       ----------   ----------
 *   
 */
/* ========================================================================== */
// ---------------- Open Issues
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- System includes e.g., <stdio.h>
#define _GNU_SOURCE

#include <stdio.h>                           // printf
#include <string.h>                          // string functionality
#include <stdlib.h>                          // to read from file and write to file


// ---------------- Local includes  e.g., "file.h"
#include "./amazing.h"                      // Given constants
#include "./common.h"                       // Additional definitions, constants
#include "./graph.h"
// ---------------- Constant definitions
#define MAX_SLOT 10000                   // Maximum slot : could be modified/deleted

// ---------------- Macro definitions

// ---------------- Structures/Types


// Nodes that compose the hashtable
typedef struct HashTableNode{
  GraphNode *node;                             // Point of hashvalue
  struct HashTableNode* next;               // pointer to next node
} HashTableNode;

// Avatar-specific data
typedef struct HashTable{
  HashTableNode* Table[MAX_SLOT];           //actual hashtable
} HashTable;

// ---------------- Private variables
int width;                                //Width of the maze
int height;                               //height of the maze


// ---------------- Private prototypes


/* ========================================================================== */

//Saves the size of the maze. 
// Returns 0 if successful
void setSize(int w, int h);


// Compute a hashValue for a given point peculiar to the HashTable
// Returns the computed HashValue
// Assumption : the input point is valid
int computeHash(GraphNode *current);

// Add to HashTable the given point
// Returns 0 if successful
int addToHash(GraphNode *current, GraphNode *before, HashTable* userTable);

// Just looks at the point which was added most recently for this current point
// Returns a Null pointer if there is no value
GraphNode *peekLastPoint(GraphNode *current, HashTable* userTable);

// Retrieves the point which was added most recently for this current point
// Deletes the point from the table
// Returns a Null pointer if there is no value
GraphNode *getPoint(GraphNode *current, HashTable* userTable);

// Frees all the memory associated with the Hashtable
// Destroys the pointer to the Hashtable itself as well
// Returns 0 if successful
int destroyTable(HashTable* userTable);


#endif // #ifndef HASHTABLE_C
