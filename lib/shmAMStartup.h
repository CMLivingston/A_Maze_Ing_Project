#ifndef SHMAMSTARTUP_H
#define SHMAMSTARTUP_H
#define _GNU_SOURCE


/* Header file for shmAMStartup.c */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>


#include "./amazing.h"
#include "./common.h"

/* Functions */
int* createKeys(int nAvatars);
int shareKeys(int * keys, int nAvatars);
int allocShm(key_t * keys, size_t size, int nAvatars);
int deallocShm(int * keys, int nAvatars);
int deallocKeys(int nAvatars);

#endif
