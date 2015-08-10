/* ========================================================================== */
/* File: Graphics.c - Graphics program that displays the maze
 *
 * Author: Cing is Believing
 * Date: March 2015
 *
 * Input: [Mazewidth] [Mazeheight] [Difficulty]
 *
 * Command line options: None
 *
 * Output: Graphical display of the maze
 *
 * Error Conditions:
 *
 * Special Considerations: The pseudocode can be found in the documentation (pdf)
 *
 * Description : This file is a graphics program called by AMStartup.c to show graphics
 *
 * Pseudocode : 
 *  
 */
/* ========================================================================== */
// ---------------- Open Issues
#define _GNU_SOURCE
// ---------------- System includes e.g., <stdio.h>

// ---------------- Local includes  e.g., "file.h"
#include "./Graphics.h"                 // Include header file

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

Graph* Maze;      // Graph used to store the maze
gint func_ref;    // Variable used to add timer
GtkWidget *window;// The window where everything is shown
GtkWidget *dialog, *label; // The popup box and message used to display that maze was solved
static char turnCount[20]; // Keeps track of the turncount

int* keyArray; // Array of keys allowing avatars to access their move structs
int* pastLocs; // Ints to remember past location of the Avatars
int totalSteps; // Total number of steps taken
int msgid; // Status of the message in message queue
my_msg_st dataMsg; // The message one sends to graphics
long int msg_to_receive=0; // Variable used for receiving message


// ---------------- Private prototypes


/* ========================================================================== */


//Create a message to show
void quick_message (gchar *message) {

  //to check if the object is floatin : can be deleted after debugging
  gboolean ref;
  
  // Create the dialog box and label
  dialog = gtk_dialog_new_with_buttons ("Tah-dah", (GtkWindow*)window, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_NONE, NULL);
  gtk_window_resize((GtkWindow*)dialog, 300, 300);
  label = gtk_label_new (message);
  g_object_ref_sink(label);

  // Code to check if objects are floating
  ref = g_object_is_floating(dialog);
  if(ref)
    g_print("Dialog is floating\n");
  ref = g_object_is_floating(label);
  if(ref)
    g_print("label is floating\n");

  // Modify the font of the message  
  gtk_widget_modify_font(label,pango_font_description_from_string("Symbol 30"));
  
   
  // Allows user to close the dialog
  g_signal_connect_swapped (dialog,"response", G_CALLBACK (gtk_widget_destroy), dialog);

  // Add the label to the container
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);
  g_object_unref(label);

  // Show everything
  gtk_widget_show_all (dialog);
}


// Prints out the current maze
static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  
  if(updateGraph()==4){
    // Tells that the maze was solved 
    char temp[100];
    sprintf(temp,"The Maze was Solved\n       in  %d steps \n   Cing is Believing!",totalSteps);
    quick_message(temp);
    // Quits the timer
    g_source_remove(func_ref);

  }else{
    totalSteps++;
    
  //Traverse through the maze and draw all the Components
  for(int i = 0; i< MazeWidth; i++){
    for(int j=0; j< MazeHeight; j++){

      //Colors the visited nodes depending on the number of visits
      if(Maze->nodes[i][j]->visited == 1){
	gdk_draw_rectangle(widget->window, widget->style->fg_gc[GTK_STATE_NORMAL], TRUE, i*Factor+Space,j*Factor+Space, Factor,Factor);
      }
      if(Maze->nodes[i][j]->visited == 2){
	gdk_draw_rectangle(widget->window, widget->style->fg_gc[GTK_STATE_SELECTED], TRUE, i*Factor+Space,j*Factor+Space, Factor,Factor);
      }
      if(Maze->nodes[i][j]->visited == 3){
	gdk_draw_rectangle(widget->window, widget->style->fg_gc[GTK_STATE_ACTIVE], TRUE, i*Factor+Space,j*Factor+Space, Factor,Factor);
      }
      if(Maze->nodes[i][j]->visited >= 4){
	gdk_draw_rectangle(widget->window, widget->style->fg_gc[GTK_STATE_PRELIGHT], TRUE, i*Factor+Space,j*Factor+Space, Factor,Factor);
      }

      //Draw the deadEnd
      if(Maze->nodes[i][j]->deadEnd == 1){
	gdk_draw_rectangle(widget->window, widget->style->black_gc, TRUE, i*Factor+Space,j*Factor+Space, Factor,Factor);
      }


      // Draws the walls
      if(Maze->nodes[i][j]->north == &WALL){
	  gdk_draw_line(widget->window, widget->style->fg_gc[GTK_STATE_INSENSITIVE], i*Factor+Space,j*Factor+Space, (i+1)*(Factor)+Space, j*Factor+Space);
      }
      if(Maze->nodes[i][j]->west == &WALL){
	gdk_draw_line(widget->window, widget->style->fg_gc[GTK_STATE_INSENSITIVE], i*Factor+Space,j*Factor+Space, i*Factor+Space, (j+1)*Factor+Space);
      }
      if(Maze->nodes[i][j]->east == &WALL){
	gdk_draw_line(widget->window, widget->style->fg_gc[GTK_STATE_INSENSITIVE], (i+1)*Factor+Space,j*Factor+Space, (i+1)*Factor+Space, (j+1)*Factor+Space);
      }
      if(Maze->nodes[i][j]->south == &WALL){
	gdk_draw_line(widget->window, widget->style->fg_gc[GTK_STATE_INSENSITIVE], i*Factor+Space,(j+1)*Factor+Space, (i+1)*Factor+Space, (j+1)*Factor+Space);
      }

      //Draw the avatar;
      if(Maze->nodes[i][j]-> current == 1){
	gdk_draw_arc(widget->window,widget->style->black_gc, TRUE,(int)((i)*Factor)+Space,(int)((j)*Factor)+Space, Factor, Factor, 0, 64*360);
      }
      
    }
  }
  
  //use cairo to indicate the number of steps
  cairo_t *cr;
  cairo_text_extents_t extents; // calculates the width of text
  cr = gdk_cairo_create(widget->window); 
  gint width, height;

  // Get the size of the window
  GtkWidget *window = gtk_widget_get_toplevel(widget);
  gtk_window_get_size(GTK_WINDOW(window), &width, &height);
 
  //Keeps the count0
  char* temp = "Turn Count : ";
  sprintf(turnCount,"%s%d",temp,totalSteps);
  
  //Sets the font size
  cairo_set_font_size(cr, 20);
  cairo_text_extents(cr, turnCount, &extents);

  //Display the text
  cairo_move_to(cr,width/2 - extents.width/2, Space / 2);
  cairo_show_text(cr, turnCount);
  cairo_destroy(cr);
  }
  return FALSE;
}

// After each interval calls the window to repaint
static gboolean time_handler(GtkWidget *widget)
{
  if (widget->window == NULL) 
    return FALSE; 

  
  // Draw the window
  gtk_widget_queue_draw(widget);
  return TRUE;
}

int main (int argc, char *argv[])
{

  //Set up a message queue
  msgid = msgget((key_t)1234,0666 | IPC_CREAT);
  if(msgid == -1){
    fprintf(stderr, "Msgget failed with error at Graphics\n");
    exit(EXIT_FAILURE);
  }



  //Since this is called by program, we do not check for arguments
  MazeWidth = (int)atol(argv[1]);
  MazeHeight = (int)atol(argv[2]);
  Difficulty = (int)atol(argv[3]);
  nAvatars = (int)atol(argv[4]);
  Factor = MIN((int)(700/MazeWidth), (int)(700/MazeHeight));
  
  //Gets the array of shm Keys
  keyArray = getKeys(nAvatars);

  // Create the maze with given dimensions
  Maze = createGraph(MazeWidth, MazeHeight);  
    

  //Drawing area on which the maze will be displayed
  GtkWidget *darea;

  gtk_init(&argc, &argv);
 
  //Create a new window, set size
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Cing is Believing");
  gtk_window_set_default_size(GTK_WINDOW(window), MazeWidth * Factor+Space*2, MazeHeight * Factor+Space*2);
  
  //Create a drawing pane and add to the window
  darea = gtk_drawing_area_new();

  // Make the floating ref into normal ref
  g_object_ref_sink(darea);

  //Check if the ref is floating ( delete after debugging)
  gboolean ref;
  ref = g_object_is_floating(darea);
  if(ref)
    g_print("Drawing area is floating");

  gtk_container_add(GTK_CONTAINER (window), darea);


  //Define the colors scheme
  //To change colors, refer to : hans.fugal.net/vim/rgbtxt.html
  GdkColor color1, color2, color3, color4, color5;
  gdk_color_parse("LightSkyBlue1", &color1);
  gtk_widget_modify_fg(GTK_WIDGET(darea),GTK_STATE_NORMAL, &color1);
  gdk_color_parse("SkyBlue1", &color2);
  gtk_widget_modify_fg(GTK_WIDGET(darea),GTK_STATE_SELECTED, &color2);
  gdk_color_parse("DeepSkyBlue1", &color3);
  gtk_widget_modify_fg(GTK_WIDGET(darea),GTK_STATE_ACTIVE, &color3);
  gdk_color_parse("DodgerBlue1", &color4);
  gtk_widget_modify_fg(GTK_WIDGET(darea),GTK_STATE_PRELIGHT, &color4);

  //Color for walls
  gdk_color_parse("firebrick1", &color5);
  gtk_widget_modify_fg(GTK_WIDGET(darea),GTK_STATE_INSENSITIVE, &color5);


  //Connect the signal to allow redrawing
  g_signal_connect(darea, "expose-event",
		   G_CALLBACK(on_expose_event), NULL);
  //Connect the signal to allow quitting
  g_signal_connect(window, "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);

  //Start timer so that the maze is redrawn every certain interval.
  func_ref = g_timeout_add(1, (GSourceFunc) time_handler, (gpointer) window);
  gtk_widget_show_all(window);
  
  //Update action as time goes on
  time_handler(window);

  g_object_unref(darea);
  //Show the GTK window pane
  gtk_main();


  //Clear the graph
  clearGraph(Maze);
  return 0;
}



 /*
 * updateGraph - Parameters: the xpos and ypos inside the recieved result
 * message from the server.
 *
 */
int updateGraph(){

  //Receives the Message from the Avatars
  if(msgrcv(msgid, (void*)&dataMsg, sizeof(struct my_msg_st)-sizeof(long), msg_to_receive, 0)==-1){
    //fprintf(stderr,"msgrcv failed\n");
    //exit(EXIT_FAILURE);
  } 
  int id = dataMsg.info[0];
  int currentX = dataMsg.info[1];
  int currentY = dataMsg.info[2];
  int currentDir = dataMsg.info[3];
  int msgType = dataMsg.info[4];


  //Return 4 if the maze was solved
  if(msgType == 4){
    // Close the message queue
    if(msgctl(msgid,IPC_RMID,0) == -1){
      fprintf(stderr,"msgctl failed\n");
      exit(EXIT_FAILURE);
    }
    return 4;
  }

  GraphNode* node = Maze->nodes[currentX][currentY];    
  // If we add a wall
  if(msgType == 3){
    addWall(node, currentDir, Maze);
    return 3;
  }

  // If it is a simple move or block dead end
  else if(msgType == 1 || msgType == 2){
      visitNode(node, currentDir, Maze);

    if(msgType == 2){
      node->deadEnd = 1;
    }
    return 1;
  }


  // The avatar is first placed here
  else if(msgType == 0){
    initialVisitNode(currentX, currentY, Maze);
    return 0;
  }

  // The avatar does NULL move
  else if(msgType == 5){
    return 5;
  }


  else{
    fprintf(stderr,"ERROR : Unknown message received from Avatar %d, message ID : %d\n",id, msgType);
  }


  return 0;
}
