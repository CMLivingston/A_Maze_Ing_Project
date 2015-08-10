#ifndef MYLIST_C
#define MYLIST_C

#define _GNU_SOURCE

#include <stdio.h>                           // printf
#include <string.h>                          // string functionality
#include <stdlib.h>                          // to read from file and write to file
#include "myList.h"

/* create a new list with null head and tail */
List *newList() {
	List *list = malloc(sizeof(*list));
	MEM_CHECK(stderr, list);
	list->head = NULL;
	list->tail = NULL;

	return list;
}

/* frees all memory not related to the GraphNode */
int destroyList(List *list) {
	
	/* pop every node in list */
	while (!isEmpty(list)) {
		pop(list);
	}

	/* free List */
	free(list);

	return 0;
}

/* add a listNode at the end of the queue with the given graphNode */
int push(GraphNode *node, List *list) {
	ListNode *tempNode = (ListNode *)malloc(sizeof(ListNode));
	MEM_CHECK(stderr, tempNode);

	tempNode->node = node;
	tempNode->next = NULL;

	if (!list -> head) {
		list -> head = tempNode;
	} else {
		list -> tail -> next = tempNode;
	}

	list -> tail = tempNode;

	return 0;
}

/* add a listNode to the front of the queue */
int addToHead(GraphNode *node, List *list) {
	ListNode *tempNode = (ListNode *)malloc(sizeof(ListNode));
	MEM_CHECK(stderr, tempNode);

	tempNode -> node = node;

	if (!list -> tail) {
		list -> tail = tempNode;
	} else {
		tempNode -> next = list -> head;
	}

	list -> head = tempNode;

	return 0;
}

/* returns 1 if list is empty, 0 otherwise */
int isEmpty(List *list) {
	if (list -> head == NULL) {
		return 1;
	} else {
		return 0;
	}
}

// Peeks at the tail
// Returns NULL if empty
GraphNode *peekTail(List* list){
  
  GraphNode *tempTail = NULL;
  if(!isEmpty(list)){
    tempTail = list->tail->node;
  }

  return tempTail;

}

/* pops the head GraphNode */
GraphNode *pop(List *list) {

	ListNode *tempListNode = NULL;
	GraphNode *tempGraphNode = NULL;

	/* return NULL is list is empty */
	if (isEmpty(list) != 1) {

		/* one node in list */
		if (list -> head == list -> tail) {
			tempListNode = list -> head;
			tempGraphNode = tempListNode -> node;
			list -> head = NULL;
			list -> tail = NULL;
		} 

		/* more than one node in list */
		else {
			tempListNode = list -> head;
			tempGraphNode = tempListNode -> node;
			list -> head = list -> head -> next;
		}

		/* free ListNode */
		free(tempListNode);
	}

	return tempGraphNode;
}

/* combine two lists into one */
List *combine(List *list1, List *list2) {
	List *combinedList = newList();
	ListNode *tempNode = list1 -> head;

	while (tempNode != NULL) {
		push(tempNode -> node, combinedList);
		tempNode = tempNode -> next;
	}

	tempNode = list2 -> head;

	while (tempNode != NULL) {
		push(tempNode -> node, combinedList);
		tempNode = tempNode -> next;
	}

	return combinedList;
}

/* check if two avatars have a common visited */
int isConnected(List *currentList, List *otherList) {

	/* declare/initialize variables */
	ListNode *checkCurrentNode, *checkOtherNode;
	checkCurrentNode = currentList -> head;

	/* iterate through currentList */
	while (checkCurrentNode != NULL) {
		
		checkOtherNode = otherList -> head;

		/* iterate through otherList */
		while (checkOtherNode != NULL) {

			/* return 1 if common node is found */
			if (checkCurrentNode -> node == checkOtherNode -> node) {
				return 1;
			}
			checkOtherNode = checkOtherNode -> next;
		}
		checkCurrentNode = checkCurrentNode -> next;
	}

	/* return 0 if no common node is found */
	return 0;
}


/* test */

/*
int main() {
	// new list
	List *list1 = newList();
	List *list2 = newList();
	List *list3 = newList();
	List *combinedList;

	// new graph
	Graph *graph = createGraph(3, 4);

	// push to different lists
	push(graph -> nodes[1][1], list1);
	push(graph -> nodes[2][2], list2);
	push(graph -> nodes[2][2], list3);

	push(graph -> nodes[1][3], list1);

	addToHead(graph -> nodes[1][2], list1);

	push(graph ->nodes[1][2], list2);
	push(graph->nodes[0][2], list2);

	//GraphNode *temp = pop(list1);
	//printf("%d, %d\n", temp -> position -> x, temp -> position -> y);


	// see if any are connected
	printf("%d\n", isConnected(list1, list2));
	printf("%d\n", isConnected(list1, list3));
	printf("%d\n", isConnected(list3, list2));

	// combine list1 and list2 
	combinedList = combine(list1, list3);

	// combined correctly? 
	printf("(%d, %d)\n", combinedList -> head -> node -> position -> x, combinedList -> head -> node -> position -> y);
	printf("(%d, %d)\n", combinedList -> tail -> node -> position -> x, combinedList -> tail -> node -> position -> y);

	// now connected? 
	printf("%d\n", isConnected(combinedList, list2));


	// clean up 
	destroyList(list1);
	destroyList(list2);
	destroyList(list3);
	//destroyList(combinedList);
	clearGraph(graph);


	return 0;
}
*/

#endif // #ifndef MYLIST_C
