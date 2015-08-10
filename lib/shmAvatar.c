/*
* shm.c - Library of functions for Avatar.c to get and set members of the 'Move' struct shared by avatars in shared memory. 
*
* Author: Cing Is Believing 
*
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>

#include "shmAvatar.h"


/*
* getKeys - returns array of ints that represents keys 
* to memory segments of each individual avatar. 
*
*/
int* getKeys(int nAvatars){

  int shmid; 

  int * shm;
  int * shmCopy = calloc(1,(sizeof(int)*nAvatars)); // copy array

  key_t myKey = 1109; // location of key array
  size_t s = (sizeof(int)*nAvatars);

  // grab segment
  shmid = shmget(myKey, s, 0666);
  
  if(shmid<0){
  perror("shmget error");
  return NULL; 
  }

  shm = (int *)shmat(shmid, NULL, 0); 
  if(shm == (int *)-1)
  {
    perror("shmat in avatar");
    return NULL; 
  }
  
   // set each element in shared mem equal to matching indexes in key array passed in (arg 1)
  int i = 0;
  while (i < nAvatars) {
    shmCopy[i] = shm[i];
    i++;
  }

  shmdt(shm);

  return shmCopy;

}

/*
* getX - gets x value of 'Move' struct from avatar Id 'avatarId'
*
* returns value of 'x member of 'Move Struct'
*/
int getX(int avatarId, int* keys, int nAvatars){

  int shmid;

  int xCopy ;
  Move* shmMove;

  int keyInt = keys[avatarId];
  key_t memKey = keyInt;

  size_t memSize = (sizeof(Move));

  shmid = shmget(memKey, memSize, 0666);
  if(shmid<0){
    perror("shmget in avatar");
    return 1;
  }

  shmMove = (Move*)shmat(shmid, NULL, 0);
  if (shmMove == (Move*)-1){
    perror("shmat in avatar");
  }

  xCopy = (int)shmMove->x;

  shmdt(shmMove);

  return xCopy;

}

/*
* setX - sets x value of 'Move' struct to 'newVal'
*
*
*/
int setX(int newVal, int avatarId, int* keys, int nAvatars){

  int shmid;

  Move* shmMove;

  int keyInt = keys[avatarId];
  key_t memKey = keyInt;

  size_t memSize = (sizeof(Move));

  shmid = shmget(memKey, memSize, 0666);
  if(shmid<0){
    perror("shmget in avatar");
    return 1;
  }

  shmMove = (Move*)shmat(shmid, NULL, 0);
  if (shmMove == (Move*)-1){
    perror("shmat in avatar");
    return 1;
  }

  //printf("%d is member before\n",shmMove->x);

  shmMove->x = newVal;

  //printf("%d is member after\n",shmMove->x);

  shmdt(shmMove);

  return 0;

}

/*
* getY - gets y value of 'Move' struct from avatar Id 'avatarId'
*
* returns value of 'y' member of 'Move Struct'
*/
int getY(int avatarId, int* keys, int nAvatars){

  int shmid;

  int yCopy;
  Move* shmMove;

  int keyInt = keys[avatarId];
  key_t memKey = keyInt;

  size_t memSize = (sizeof(Move));

  shmid = shmget(memKey, memSize, 0666);
  if(shmid<0){
    perror("shmget in avatar");
    return 1;
  }

  shmMove = (Move*)shmat(shmid, NULL, 0);
  if (shmMove == (Move*)-1){
    perror("shmat in avatar");
  }

  yCopy = (int)shmMove->y;

  shmdt(shmMove);

  return yCopy;

}

/*
* setY - sets y value of 'Move' struct to 'newVal'
*
*
*/
int setY(int newVal, int avatarId, int* keys, int nAvatars){

  int shmid;

  Move* shmMove;

  int keyInt = keys[avatarId];
  key_t memKey = keyInt;

  size_t memSize = (sizeof(Move));

  shmid = shmget(memKey, memSize, 0666);
  if(shmid<0){
    perror("shmget in avatar");
    return 1;
  }

  shmMove = (Move*)shmat(shmid, NULL, 0);
  if (shmMove == (Move*)-1){
    perror("shmat in avatar");
    return 1;
  }

  shmMove->y = newVal;

  shmdt(shmMove);

  return 0;

}

/*
* getY - gets y value of 'Move' struct from avatar Id 'avatarId'
* @param - keys - the int array created from call to getKeys();
*
* returns value of 'dir' member of 'Move Struct'
*/
int getDir(int avatarId, int* keys, int nAvatars){

  int shmid;

  int dirCopy;
  Move* shmMove;

  int keyInt = keys[avatarId];
  key_t memKey = keyInt;

  size_t memSize = (sizeof(Move));

  shmid = shmget(memKey, memSize, 0666);
  if(shmid<0){
    perror("shmget in avatar");
    return 1;
  }

  shmMove = (Move*)shmat(shmid, NULL, 0);
  if (shmMove == (Move*)-1){
    perror("shmat in avatar");
  }

  dirCopy = (int)shmMove->dir;

  shmdt(shmMove);

  return dirCopy;

}

/*
* setDir - sets y value of 'Move' struct to 'newVal'
*
* returns 0 for success, 1 for failure
*/
int setDir(int newVal, int avatarId, int* keys, int nAvatars){

  int shmid;

  Move* shmMove;

  int keyInt = keys[avatarId];
  key_t memKey = keyInt;

  size_t memSize = (sizeof(Move));

  shmid = shmget(memKey, memSize, 0666);
  if(shmid<0){
    perror("shmget in avatar");
    return 1;
  }

  shmMove = (Move*)shmat(shmid, NULL, 0);
  if (shmMove == (Move*)-1){
    perror("shmat in avatar");
    return 1;
  }

  shmMove->dir = newVal;

  shmdt(shmMove);

  return 0;

}

/* freeKeys - cleanup for key array from call to getKeys() */
int freeKeys(int* keys){
  free(keys);
  return 0;
}

/*
int main(int argc, char const *argv[])
{
	test code 

	int* ints = getKeys(3); // WORKS

  setX(10, 0, ints, 3);
  int ret = getX(0, ints, 3);
	printf("this is int: %d\n", ret);

	return 0;
} */
