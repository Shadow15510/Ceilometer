#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "include/main.h"
#include "include/callbacks.h"


GtkBuilder *builder;
GdkWindow *G_WINDOW = NULL;
GdkCursor *G_WAIT_CURSOR = NULL;
GdkCursor *G_DEFAULT_CURSOR = NULL;

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *window;

	char gladepath[100];
	sprintf(gladepath, "%s/.local/bin/nevada_data/glade/nevada.glade", getenv("HOME"));
	builder = gtk_builder_new_from_file(gladepath);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_nevada"));
	
	gtk_builder_connect_signals(builder, NULL);
	g_signal_connect(window, "realize", G_CALLBACK(window_realize), NULL);

	gtk_widget_show_all(window);
	gtk_main();

	g_object_unref(builder);
	
	return 0;
}


static void window_realize(GtkWidget *window, gpointer data)
{
	G_WAIT_CURSOR = gdk_cursor_new_from_name(gdk_display_get_default(), "wait");
	G_DEFAULT_CURSOR = gdk_cursor_new_from_name(gdk_display_get_default(), "default");

    	G_WINDOW = gtk_widget_get_window(window);
}