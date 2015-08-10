/* ========================================================================== */
/* File: Graphics.h - Header for graphics program that displays the maze
 *
 * Author: Cing is Believing
 * Date: March 2015
 *
 * Input: 
 *
 * Command line options: None
 *
 * Output: Graphical display of the maze
 *
 * Error Conditions:
 *        
 *
 * Special Considerations: The pseudocode can be found in the documentation (pdf)
 *
 * Description : This file is the header file for the graphics program called by AMStartup.c to show graphics
 *
 * Pseudocode : 
 *  
 */
/* ========================================================================== */
// ---------------- Open Issues
#define _GNU_SOURCE
// ---------------- System includes e.g., <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/msg.h>
// ---------------- Local includes  e.g., "file.h"
#include "./../lib/graph.h"
#include "./../lib/amazing.h"
#include "./../lib/common.h"
#include "./../lib/shmAvatar.h"
#include "./../lib/shmAMStartup.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// Struct used to send messages to the graphics program
typedef struct my_msg_st{
  long mtype;
  int info[5];
} my_msg_st;

// ---------------- Private variables
int MazeWidth;    // Stores the width of the maze
int MazeHeight;   // Stores the height of the maze
int Difficulty;   // Stores the difficulty of the maze
int nAvatars;     // Stores the number of the avatars
int Factor;       // Factor used to calculate size of the maze
int Space = 50; // Used to make the gap between window and the maze

// ---------------- Private prototypes

/* ========================================================================== */



/*
 * Creates a pop-up windows that contains
 * the message in the parameter
 */
void quick_message(gchar* message); 


/*
 * Action to be done every specified interval.
 * In this case, it reads in the graph and displays the maze
 * When the maze is solved, show the message that it was solved
 * 
 */
static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);


/*
 * After every specified interval, calls the certain widget to redraw itself.
 */
static gboolean time_handler(GtkWidget *widget);

/*
 * Deallocates all the used keys
 *
int deallocKeys(int nAvatars); 
*/

/*
 * Deallocates all the shared memory
 *
int deallocShm(int *keys, int nAvatars);
*/
/*
 * Updates the graph
 */
int updateGraph();

