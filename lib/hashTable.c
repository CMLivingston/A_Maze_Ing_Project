/* ========================================================================== */
/* File: hashtable.c - Hashtable used to store paths of avatars
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
#ifndef HASHTABLE_C
#define HASHTABLE_C

// ---------------- System includes e.g., <stdio.h>
#define _GNU_SOURCE

#include <stdio.h>                           // printf
#include <string.h>                          // string functionality
#include <stdlib.h>                          // to read from file and write to file


// ---------------- Local includes  e.g., "file.h"
#include "./hashTable.h"                  // Header file

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes5B

/* ========================================================================== */


// Given the width and height of the maze
// Save the values.
// Needs to be called only once.
void setSize(int w, int h){
  width = w;
  height = h;
}

// Compute the HashIndex for a given point
/*
 *  0  1  2  3  4
 *  5  6  7  8  9
 *  10 ...
 *
 * i.e. computeHash(1,3)=8, computeHash(0,2)=2
 */
int computeHash(GraphNode *current){
  return current -> position -> x * height + current -> position -> y; 
}

// Adds the given point to the table
int addToHash(GraphNode *current, GraphNode *before, HashTable* userTable){
	//compute the hash index
	int index = computeHash(current);

	HashTableNode* newNode = calloc(1,sizeof(*newNode));
	MEM_CHECK(stderr, newNode);
	newNode->node = before;
	newNode->next = NULL;

	//If the value is not saved yet, 
	if(!(userTable->Table[index])){ 
		userTable->Table[index]=newNode;
	}else{
		//there is already an element for given hash
		HashTableNode* temp = userTable->Table[index];

		//Go through the list and add the new element in the end of the list
		while(temp->next){
			temp = temp->next;
		}
		temp->next = newNode; 
	}

	return 0;
}


// Frees all the memory associated with the Hashtable
// Destroys the pointer to Hashtable itself as well
// Returns 0 if successful
int destroyTable(HashTable* userTable){

	//Goes through hashtable and find all values that have values
	for(int i = 0; i < MAX_SLOT; i++){
		if(userTable->Table[i]){
			//Per hash value, delete all the unnecessary nodes
			HashTableNode* temp = userTable->Table[i];
			HashTableNode* prev;
			while(temp){
				prev = temp;
				temp = temp->next;
				free(prev);
			}
		}
	}
	free(userTable);
	return 0;
}


// Allows user/avatar to look at the point which was added most recently
// for the input point
GraphNode *peekLastPoint(GraphNode *current, HashTable* userTable){
	//compute the hash index
	int index = computeHash(current);

	GraphNode *tempNode = NULL;

	//If the value is not saved yet, 
	if(!(userTable->Table[index])){ 
		return tempNode;
	}else{
		//there is already an element for given hash
		HashTableNode* temp = userTable->Table[index];

		//Go through the list and add the new element in the end of the list
		while(temp->next){
			temp = temp->next;
		}

		tempNode = temp->node;

		return tempNode;
	}
}

#endif // #ifndef HASHTABLE_C
