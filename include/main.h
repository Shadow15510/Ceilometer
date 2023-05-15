/* NEVADA:main
	
   Ths main module of NEVADA.
*/

#ifndef _MAIN_H
#define _MAIN_H

// main: the main function
int main(int argc, char *argv[]);


/* window_realize: bind to the "realize" signal

   Allow to change the cursor. */
static void window_realize(GtkWidget *window, gpointer data);


#endif /* _MAIN_H */