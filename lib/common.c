/* ========================================================================== */
/* File: common.c
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

#ifndef COMMON_C
#define COMMON_C

// ---------------- System includes e.g., <stdio.h>
#include "common.h"                          // header file
#include "math.h"                            // sqrt

// ---------------- Local includes  e.g., "file.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// Struct which saves a coordinate

// ---------------- Private variables

// ---------------- Private prototypes

// Returns the Euclidean distance between two coordinates

/* ========================================================================== */

/*
 * Calculates the Euclidean distance between two points
 */
double  getDist(XYPos* one, XYPos* two){
  double first = DIFFSQ(one->x, two->x);
  double second = DIFFSQ(one->y, two->y);
  return sqrt(first+second);
}


#endif // #ifndef COMMON_H
