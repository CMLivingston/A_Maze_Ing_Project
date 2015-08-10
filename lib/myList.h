// ---------------- Open Issues
#ifndef MYLIST_H
#define MYLIST_H


#include <stdio.h>                           // printf
#include <string.h>                          // string functionality
#include <stdlib.h>                          // to read from file and write to file

#include "common.h"                     // Additional definitions, constants
#include "graph.h"

/* Nodes that compose the list */
typedef struct ListNode{
  GraphNode* node;                             // Point in the list
  struct ListNode* next;                    // Pointer to next node
} ListNode;

/* List struct that contains the head and tail of the list */
typedef struct List{
  ListNode* head;                           // Head of the list
  ListNode* tail;                           // Tail of the list
} List;

/* methods */
List *newList();

int destroyList(List *list);

int push(GraphNode *node, List *list);

int addToHead(GraphNode *node, List *list);

GraphNode* peekTail(List* list);

int isEmpty(List *list);

GraphNode *pop(List *list);

List *combine(List *list1, List *list2);

int isConnected(List *currentList, List *otherList);




#endif// #ifndef MYLIST_H
