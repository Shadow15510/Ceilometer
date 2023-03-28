#include <netcdf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/callbacks.h"
#include "include/netcdf_api.h"
#include "include/netcdf_data.h"
#include "include/sdl_api.h"


extern GtkBuilder *builder;
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


G_MODULE_EXPORT void on_check_filter_toggled(void)
{
	if (filter)
		filter = false;
	else
		filter = true;
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
	// Récupération du nom du fichier
	const char *filename = NULL;
	if (is_file_selected)
	{
		GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_netcdf"));
		filename = gtk_file_chooser_get_filename(file_chooser);
	}
	else return;

	// Récupération de la variable
	GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combo_vars"));
	const char *var = gtk_combo_box_text_get_active_text(combo);
	if (var == NULL)
		return ;

	// Récupération des dimensions et des métadonnées
	size_t X_AXIS, Y_AXIS;
	int year, month, day;

	netcdf_get_metadata(filename, var, &X_AXIS, &Y_AXIS, &year, &month, &day);
	char date[11];
	sprintf(date, "%d-%d-%d", day, month, year);

	// Récupération des données
	float *data = malloc(X_AXIS * Y_AXIS * sizeof(float));
	float *x_labels = malloc(X_AXIS * sizeof(float));
	float *y_labels = malloc(Y_AXIS * sizeof(float));
	char dimsname[2][NC_MAX_NAME + 1];
	netcdf_get_data(filename, var, data, x_labels, y_labels, dimsname);

	// Récupération du mode d'ouverture et traitement des données
	float minimum = 0, maximum = 0;
	if (filter)
	{
		GtkSpinButton *spin_minimum = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_minimum"));
		minimum = (float) gtk_spin_button_get_value(spin_minimum);

		GtkSpinButton *spin_maximum = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_maximum"));
		maximum = (float) gtk_spin_button_get_value(spin_maximum);

		if (maximum <= minimum) return;
	}

	// Mise à jour du statut
	GtkLabel *label_status = GTK_LABEL(gtk_builder_get_object(builder, "label_status"));
	gtk_label_set_text(label_status, "en cours de traitement");
	gtk_main_iteration();
	sleep(0.1);
	gtk_main_iteration();

	struct netcdf_data netcdf_data = {
		data,
		var,

		X_AXIS,
		x_labels,
		dimsname[0],

		Y_AXIS,
		y_labels,
		dimsname[1],

		date,

		minimum,
		maximum,
		filter
	};

	// Boucle principale
	if (image_mode)
		sdl_image(&netcdf_data);
	else
		sdl_measure(&netcdf_data);

	// Désallocation des ressources
	free(data);
	free(x_labels);
	free(y_labels);

	// Mise à jour du statut
	gtk_label_set_text(label_status, "en attente de validation");
	gtk_main_iteration();
	sleep(0.1);
	gtk_main_iteration();

}