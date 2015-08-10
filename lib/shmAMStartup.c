/*
* shm.c - Library of functions for AMStarup.c to allocate 
*         shared memory for the avatar subprocesses. 
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

#include "shmAMStartup.h"


/* Move struct with starting x,y pos and direction of move as defined in amazing.h */


////////////////////////////////

/* 
* createKeys - returns a int array of key values for the
* shared memory segment of each individual process
*
* NOTE: User is responsible for converting int keys to 'key_t' type for use in SHMGET calls
*/
int* createKeys(int nAvatars){

    int* keys = calloc(1, (sizeof(int)*nAvatars));
    
    // starting key of first avatar's shared mem...
    // subsequent avatrs will be 1111, 1112, etc. 
    key_t key = 1110;   

    // fill array of keys for each avatar...
    // n avatars will have n segment keys
    int j = 0;
    while (j < nAvatars) {
    	keys[j] = key;
    	key++;
    	j++;
    }
   	
    return keys;
}


/*
* shareKeys(int * keys, int nAvatars) - puts the array of int keys 
* into shared memory for access at key '1109'.
*
*/
int shareKeys(int * keys, int nAvatars){

  int shmid;
  int * keyArray; //= calloc(1, (sizeof(int)*nAvatars));
  key_t keyListKey = 1109; // key of the keyList
      
  // get handle id for shared segment 
  shmid = shmget(keyListKey, (sizeof(int) * nAvatars), IPC_CREAT | 0666);
  if(shmid<0){
    perror("shmget error");
    return 1;
  }
 
  // attatch pointer to shared memory segment
  keyArray = (int*)shmat(shmid, NULL, 0); 
  if(keyArray == (int *) -1)
  {
    perror("shmat error");
    return 1;
  }

  // set each element in shared mem equal to matching indexes in key array passed in (arg 1)
  int i = 0;
  while (i < nAvatars) {
    keyArray[i] = keys[i];
    i++;
  }

  // detatch pointer 'keyArray' from the shmid handle 
  shmdt(keyArray);

  return 0;

}

/*
* allocShm(int * keys, size_t size, int nAvatars) - attatches each key to a 
* shared memory space of a specified size.
*
* Returns status int for now. 0 = success, 1 = failure
*
* NOTE: size_t must be calculated BEFORE passing into function.
*
* Ex: 
*    size_t s =  (sizeof(Move));
*    allocShm(keys, s, 3);
*
*/
int allocShm(key_t * keys, size_t size, int nAvatars){

  int shmid; 
  Move * shmMove; // move to initialize... all members initialized to 0;

  for (int i = 0; i < nAvatars; ++i)
  {

    // get current avatar key to allocate to
    key_t myKey = keys[i];

    // allocate segment of size 'size' at 'myKey' location
    shmid = shmget(myKey, size, IPC_CREAT | 0666);
    if(shmid<0){
        perror("shmget error in allocShm");
        return 1; 
    }

    shmMove = (Move *)shmat(shmid, NULL, 0); 
    if(shmMove == (Move *)-1)
    {
      perror("shmat in allocShm");
      return 1; 
    }

    // initialize all values to -1
    shmMove->x = -1;
    shmMove->y = -1;
    shmMove->dir = -1; // indicating that this avatar has not yet updated the struct with its previous move 


    // detatch shared mem from 'shmMove' ptr
    shmdt(shmMove);

  }

  return 0; 

}

/*
* deallocShm(int * keys, int nAvatars) - deallocates shared memory
* associated with each key in 'keys'.
*
* Returns status int for now. 0 = success, 1 = failure
*
*/
int deallocShm(int * keys, int nAvatars){

  int shmid;

  for (int i = 0; i < nAvatars; ++i)
  {

    // get current avatar key to allocate to
    int t = keys[i];
    key_t myKey = t;

    size_t s = (sizeof(Move));

    // grab segment
    shmid = shmget(myKey, s, 0666);
    if(shmid<0){
        perror("shmget error");
        return 1; 
    }

    // call 'shmctl' to free shared memory segment at 'shmid'
    shmctl(shmid, IPC_RMID, 0);

  }

  return 0; 

}

/*
* deallocKeys(int nAvatars) - removes int array of keys from shared memory (cleanup).
*
* Returns 0 for success, 1 for fail
*
*/

int deallocKeys(int nAvatars){

  int shmid;
  key_t keyListKey = 1109; // key of the keyList
      
  size_t s = (sizeof(int) * nAvatars);

  // get handle id for shared segment 
  shmid = shmget(keyListKey, s, 0666);
  if(shmid<0){
    perror("shmget error");
    return 1;
  }
 
  // call 'shmctl' to free shared memory segment at 'shmid'
  shmctl(shmid, IPC_RMID, 0);

  return 0; 

}


/* testing function */
//int main(){

//	int* keys = createKeys(3);

//  shareKeys(keys, 3);
   
//  size_t s =  (sizeof(Move));
 // allocShm(keys, s, 3);
 
  //deallocShm(keys, 3);

  //deallocKeys(3);


  //return 0; 

//}

