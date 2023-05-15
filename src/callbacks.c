#include <gdk/gdk.h>
#include <netcdf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/callbacks.h"
#include "include/netcdf_api.h"
#include "include/netcdf_data.h"
#include "include/sdl_api.h"


extern GtkBuilder *builder;
extern GdkWindow *G_WINDOW;
extern GdkCursor *G_WAIT_CURSOR;
extern GdkCursor *G_DEFAULT_CURSOR;
bool is_file_selected = false;
bool filter = true;
bool image_mode = true;


G_MODULE_EXPORT void on_window_nevada_destroy(void)
{
	gtk_main_quit();
}


G_MODULE_EXPORT void on_file_netcdf_file_set(void)
{
	is_file_selected = true;

	const char *filename = NULL;
	GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_netcdf"));
	filename = gtk_file_chooser_get_filename(file_chooser);

	netcdf_get_variables(filename);
}


G_MODULE_EXPORT void on_combo_vars_changed(void)
{
	GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_netcdf"));
	const char *filename = gtk_file_chooser_get_filename(file_chooser);

	GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combo_vars"));
	const char *var = gtk_combo_box_text_get_active_text(combo);

	netcdf_set_fits(filename, var);	
}


G_MODULE_EXPORT void on_check_filter_toggled(void)
{
	GtkWidget *sp_min = GTK_WIDGET(gtk_builder_get_object(builder, "spin_minimum"));
	GtkWidget *sp_max = GTK_WIDGET(gtk_builder_get_object(builder, "spin_maximum"));

	if (filter)
	{
		gtk_widget_set_sensitive(sp_min, false);
		gtk_widget_set_sensitive(sp_max, false);
		filter = false;
	}
	else
	{
		gtk_widget_set_sensitive(sp_min, true);
		gtk_widget_set_sensitive(sp_max, true);
		filter = true;
	}
}


G_MODULE_EXPORT void on_radio_image_toggled(void)
{
	image_mode = true;
}


G_MODULE_EXPORT void on_radio_measure_toggled(void)
{
	image_mode = false;
}


G_MODULE_EXPORT void on_button_validation_clicked(void)
{
	// Get the filename
	const char *filename = NULL;
	if (is_file_selected)
	{
		GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_netcdf"));
		filename = gtk_file_chooser_get_filename(file_chooser);
	}
	else return;

	// Get the variable
	GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combo_vars"));
	const char *var = gtk_combo_box_text_get_active_text(combo);
	if (var == NULL)
		return ;

	// Get the dim and metadatas
	size_t X_AXIS, Y_AXIS;
	int year, month, day;
	char y_unit[NC_MAX_NAME + 1] = {0};
	char date[11] = {0};
	netcdf_get_metadata(filename, var, &X_AXIS, &Y_AXIS, y_unit, date);

	// Get data
	float *data = malloc(X_AXIS * Y_AXIS * sizeof(float));
	float *x_labels = malloc(X_AXIS * sizeof(float));
	float *y_labels = malloc(Y_AXIS * sizeof(float));
	char dimsname[2][NC_MAX_NAME + 1];
	netcdf_get_data(filename, var, data, x_labels, y_labels, dimsname);

	// Get the opening mode and analyse the data (if needed)
	float minimum = 0, maximum = 0;
	if (filter)
	{
		GtkSpinButton *spin_minimum = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_minimum"));
		minimum = (float) gtk_spin_button_get_value(spin_minimum);

		GtkSpinButton *spin_maximum = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_maximum"));
		maximum = (float) gtk_spin_button_get_value(spin_maximum);

		if (maximum <= minimum) return;
	}

	// Get the axis limits
	GtkSpinButton *spin_x_min = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_x_min"));
	int x_min = (int) gtk_spin_button_get_value(spin_x_min);
	GtkSpinButton *spin_x_max = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_x_max"));
	int x_max = (int) gtk_spin_button_get_value(spin_x_max);
	GtkSpinButton *spin_y_min = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_y_min"));
	int y_min = (int) gtk_spin_button_get_value(spin_y_min);
	GtkSpinButton *spin_y_max = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_y_max"));
	int y_max = (int) gtk_spin_button_get_value(spin_y_max);
	if (x_min >= x_max || y_min >= y_max)
		return ;

	GtkProgressBar *pbar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));
	gtk_progress_bar_set_text(pbar, "Work in progress");
	gdk_window_set_cursor(G_WINDOW, G_WAIT_CURSOR);

	float factor_x = 1, factor_y = 1;
	GtkSpinButton *spin_factor_x = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_factor_x"));
	factor_x = (float) gtk_spin_button_get_value(spin_factor_x);
	GtkSpinButton *spin_factor_y = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_factor_y"));
	factor_y = (float) gtk_spin_button_get_value(spin_factor_y);

	struct netcdf_data netcdf_data = {
		data,
		var,

		X_AXIS,
		x_labels,
		dimsname[0],
		x_min,
		x_max,
		factor_x,

		Y_AXIS,
		y_labels,
		dimsname[1],
		y_min,
		y_max,
		factor_y,
		y_unit,

		date,

		minimum,
		maximum,
		filter
	};

	// Main render function
	sdl_render(&netcdf_data, image_mode);


	// Free the ressources
	free(data);
	free(x_labels);
	free(y_labels);

	gdk_window_set_cursor(G_WINDOW, G_DEFAULT_CURSOR);
	gtk_progress_bar_set_text(pbar, "Waiting for validation");
	gtk_progress_bar_set_fraction(pbar, 0);
}
