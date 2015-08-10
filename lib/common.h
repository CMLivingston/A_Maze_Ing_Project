/* ========================================================================== */
/* File: common.h
 *
 * Author: Cing is Believing
 * Date: February 2015
 *
 * Input:  NA
 * Command line options: NA
 * Output: NA
 * Error Conditions: NA
 * Special Considerations: NA
 *
 * Description : The file contains common variables and functions for all files
 *               (In Addition to amazing.h)
 */
/* ========================================================================== */
// ---------------- Open Issues

#ifndef COMMON_H
#define COMMON_H

// ---------------- System includes e.g., <stdio.h>

// ---------------- Local includes  e.g., "file.h"
#include "./amazing.h"                          //include given constants
// ---------------- Constant definitions
#define HOSTNAME "pierce.cs.dartmouth.edu"
// ---------------- Macro definitions

#define MEM_CHECK(fp,s) do{          \
  if((s) == NULL){                   \
    fprintf((fp), "Error: [%s:%d] out of memory,\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE);							\
  }					       \
  }while(0)

#define VAL_CHECK(fp,s) do{          \
  if((s) == NULL){                   \
    fprintf((fp), "Error: [%s:%d] invalid call,\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE);							\
  }					       \
  }while(0)


#define DIFFSQ(a,b) ((double)a-(double)b)*((double)a-(double)b)

// ---------------- Structures/Types

/* Move struct with starting x,y pos and direction of move as defined in amazing.h */
typedef struct Move {
    int x;
    int y;
    int dir;
} Move;

// Struct which saves a coordinate

// ---------------- Private variables

// ---------------- Private prototypes

// Returns the Euclidean distance between two coordinates
double getDist(XYPos* one, XYPos* two);

/* ========================================================================== */


#endif // #ifndef COMMON_H
