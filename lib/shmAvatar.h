#ifndef SHMAVATAR_H
#define SHMAVATAR_H
#define _GNU_SOURCE
/* Header file for shmAvatar.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>

#include "./common.h"


/* Prototypes */

/*
* getKeys - returns array of ints that represents keys 
* to memory segments of each individual avatar. 
*
*/
int* getKeys(int nAvatars);


/*
* getX - gets x value of 'Move' struct from avatar Id 'avatarId'
*
* returns value of 'x member of 'Move Struct'
*/
int getX(int avatarId, int* keys, int nAvatars);


/*
* setX - sets x value of 'Move' struct to 'newVal'
*
*
*/
int setX(int newVal, int avatarId, int* keys, int nAvatars);


/*
* getY - gets y value of 'Move' struct from avatar Id 'avatarId'
*
* returns value of 'y' member of 'Move Struct'
*/
int getY(int avatarId, int* keys, int nAvatars);


/*
* setY - sets y value of 'Move' struct to 'newVal'
*
*
*/
int setY(int newVal, int avatarId, int* keys, int nAvatars);


/*
* getY - gets y value of 'Move' struct from avatar Id 'avatarId'
* @param - keys - the int array created from call to getKeys();
*
* returns value of 'dir' member of 'Move Struct'
*/
int getDir(int avatarId, int* keys, int nAvatars);


/*
* setDir - sets y value of 'Move' struct to 'newVal'
*
* returns 0 for success, 1 for failure
*/
int setDir(int newVal, int avatarId, int* keys, int nAvatars);


/* freeKeys - cleanup for key array from call to getKeys() */
int freeKeys(int* keys);


#endif
