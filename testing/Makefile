# Makefile for Maze Program
# Description : The makefile is to clean testing files

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIBDIR = ./../lib/
LIBC = $(LIBDIR)common.h $(LIBDIR)hashTable.h $(LIBDIR)myList.h $(LIBDIR)graph.h $(LIBDIR)shmAMStartup.h $(LIBDIR)shmAvatar.h $(LIBDIR)avatar_func.h
LIBH = $(LIBC:.c=.h)
LIBFLAG = -lmaze
LIB = $(LIBDIR)libmaze.a

tester : $(LIBH) $(LIBC) $(LIB) tester.c 
	$(CC) $(CFLAGS) -o tester tester.c -L$(LIBDIR) $(LIBFLAG) -lm

$(LIBDIR)libmaze.a : $(LIBH) $(LIBC)
	cd $(LIBDIR); make;

.PHONY : clean

#clean function : deletes all temporary files
clean :
	rm -f *~
	rm -f *#
	rm -f *.o
	rm -f core.*
	rm -f vgcore.*
	rm -f *.gch
	@echo "Unnecessary Files were deleted from ./testing."


#cleanLog function : cleans all the logs
cleanLog : 
	rm -f Amazing_*.log
	rm -f MazeTestLog.*
	@echo "All the previous logs were deleted."



