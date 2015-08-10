# Makefile for Maze Program
# Description : The makefile is to build up the whole Maze

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
GFLAGS = `pkg-config --libs --cflags gtk+-2.0`
COPT = -lm #math library included for log function
SRCDIR = ./src/
LIBDIR = ./lib/
LIBFLAG = -lmaze
LIB = $(LIBDIR)libmaze.a
LIBC = $(LIBDIR)common.h $(LIBDIR)hashTable.h $(LIBDIR)myList.h $(LIBDIR)graph.h $(LIBDIR)shmAMStartup.h $(LIBDIR)shmAvatar.h $(LIBDIR)avatar_func.h
LIBH = $(LIBC:.c=.h)
TESTDIR = ./testing/


#Make the Maze Program
maze : $(SRCDIR)AMStartup.c $(LIBDIR)amazing.h avatar $(LIBH) $(LIB) graphics
	$(CC) $(CFLAGS) -o maze $(SRCDIR)AMStartup.c -L$(LIBDIR) $(LIBFLAG) $(COPT)
	@echo "Maze Built"


avatar : $(SRCDIR)Avatar.c $(LIBDIR)amazing.h $(LIB) $(LIBH)
	$(CC) $(CFLAGS) -o avatar $(SRCDIR)Avatar.c -L$(LIBDIR) $(LIBFLAG) $(COPT)
	@echo "avatar Built"


graphics : $(SRCDIR)Graphics.c $(SRCDIR)Graphics.h $(LIBH)
	$(CC) $(CFLAGS) -o graphics $(SRCDIR)Graphics.c $(GFLAGS) -L$(LIBDIR) $(LIBFLAG)

$(LIB) : $(LIBC) $(LIBH)
	cd $(LIBDIR); make;
	@echo "util library built"

.PHONY : clean

#clean function : deletes all temporary files
clean :
	rm -f *~
	rm -f *#
	rm -f *.o
	rm -f core.*
	rm -f vgcore.*
	rm -f *.gch
	rm -f Amazing*.log
	@echo "Unnecessary Files were deleted from the folder."
	cd $(LIBDIR); make clean;
	cd $(SRCDIR); make clean;
	cd $(TESTDIR); make clean;

#cleanLog function : deletes all previous logs
cleanLog : 
	cd $(TESTDIR); make cleanLog;



