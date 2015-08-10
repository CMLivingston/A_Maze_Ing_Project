# Makefile for ./lib
# Description : The makefile is to build up the library and clean folder

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
COPT = -lm
SRCDIR = ./../src/
TEST = ./../testing/
LIB = libmaze.a

#specify which file will be created using which sources

$(LIB) : avatar_func.o common.o hashTable.o graph.o myList.o shmAMStartup.o shmAvatar.o
	ar -cvq $(LIB) *.o

avatar_func.o : avatar_func.c avatar_func.h myList.h graph.h
	$(CC) $(CFLAGS) -c avatar_func.c $(COPT)

myList.o : myList.c myList.h amazing.h common.h graph.h
	$(CC) $(CFLAGS) -c myList.c

hashTable.o : hashTable.c hashTable.h amazing.h common.h graph.h
	$(CC) $(CFLAGS) -c hashTable.c

graph.o : graph.c graph.h amazing.h 
	$(CC) $(CFLAGS) -c graph.c

common.o : common.c common.h
	$(CC) $(CFLAGS) -c common.c $(COPT)

shmAMStartup.o : shmAMStartup.c shmAMStartup.h common.h
	$(CC) $(CFLAGS) -c shmAMStartup.c common.h

shmAvatar.o : shmAvatar.c shmAvatar.h common.h
	$(CC) $(CFLAGS) -c shmAvatar.c common.h


all : avatar_func myList hashTable graph 

avatar_func : avatar_func.h avatar_func.c common.h graph.h hashTable.h
	$(CC) $(CFLAGS) -o avatar_func avatar_func.c avatar_func.h graph.h myList.h common.h

.PHONY : clean

#clean function : deletes all temporary files
clean :
	rm -f *~
	rm -f *#
	rm -f *.a
	rm -f *.o
	rm -f core.*
	rm -f vgcore.*
	rm -f *.gch
	@echo "Unnecessary Files were deleted from ./lib."
