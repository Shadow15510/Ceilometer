#include <stdio.h>
#include <gtk/gtk.h>
#include "include/main.h"
#include "include/callbacks.h"

GtkBuilder *builder;

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *window;

	builder = gtk_builder_new_from_file("/usr/bin/ceilometer_data/glade/ceilometer.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_ceilometer"));
	
	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(window);
	gtk_main();

	g_object_unref(builder);
	return 0;
}