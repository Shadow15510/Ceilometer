#include <gtk/gtk.h>
#include <math.h>
#include <netcdf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "include/netcdf_api.h"
#include "include/sdl_api.h"


extern GtkBuilder *builder;


void netcdf_get_variables(const char *filename)
{
	int ncid, nvars, ndims;

	GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combo_vars"));
	gtk_combo_box_text_remove_all(combo);

	nc_open(filename, NC_NOWRITE, &ncid);
	nc_inq(ncid, NULL, &nvars, NULL, NULL);
	
	for (int varid = 0; varid < nvars; varid ++)
	{
		gchar varname[NC_MAX_NAME + 1];
		nc_inq_var(ncid, varid, varname, NULL, &ndims, NULL, NULL);
		if (ndims == 2)
			gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo), NULL, varname);
	}

	GtkAdjustment *fit_x_min = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "fit_x_min"));
	gtk_adjustment_configure(fit_x_min, 0, 0, 0, 1, 10, 0);

	GtkAdjustment *fit_x_max = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "fit_x_max"));
	gtk_adjustment_configure(fit_x_max, 0, 0, 0, 1, 10, 0);

	GtkAdjustment *fit_y_min = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "fit_y_min"));
	gtk_adjustment_configure(fit_y_min, 0, 0, 0, 1, 10, 0);

	GtkAdjustment *fit_y_max = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "fit_y_max"));
	gtk_adjustment_configure(fit_y_max, 0, 0, 0, 1, 10, 0);

	nc_close(ncid);
}


void netcdf_set_fits(const char *filename, const char *var)
{
	int ncid, varid, ndims;
	size_t x_axis, y_axis;
	int dimsid[NC_MAX_VAR_DIMS];

	nc_open(filename, NC_NOWRITE, &ncid);

	nc_inq_varid(ncid, var, &varid);
	nc_inq_var(ncid, varid, NULL, NULL, &ndims, dimsid, NULL);

	nc_inq_dimlen(ncid, dimsid[0], &x_axis);	
	nc_inq_dimlen(ncid, dimsid[1], &y_axis);

	nc_close(ncid);	

	GtkAdjustment *fit_x_min = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "fit_x_min"));
	gtk_adjustment_configure(fit_x_min, 0, 0, x_axis, 1, 10, 0);

	GtkAdjustment *fit_x_max = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "fit_x_max"));
	gtk_adjustment_configure(fit_x_max, x_axis, 0, x_axis, 1, 10, 0);

	GtkAdjustment *fit_y_min = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "fit_y_min"));
	gtk_adjustment_configure(fit_y_min, 0, 0, y_axis, 1, 10, 0);

	GtkAdjustment *fit_y_max = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "fit_y_max"));
	gtk_adjustment_configure(fit_y_max, y_axis, 0, y_axis, 1, 10, 0);

}

void netcdf_get_metadata(const char *filename, const char *var, size_t *x_axis, size_t *y_axis, char *y_unit, char *date)
{
	int ncid, varid, ndims;
	int dimsid[NC_MAX_VAR_DIMS];

	nc_open(filename, NC_NOWRITE, &ncid);

	nc_inq_varid(ncid, var, &varid);
	nc_inq_var(ncid, varid, NULL, NULL, &ndims, dimsid, NULL);

	nc_inq_dimlen(ncid, dimsid[0], x_axis);	
	nc_inq_dimlen(ncid, dimsid[1], y_axis);

	char y_name[NC_MAX_NAME + 1];

	int y_id;
	nc_inq_dim(ncid, dimsid[1], y_name, NULL);
	nc_inq_varid(ncid, y_name, &y_id);
	nc_get_att_text(ncid, y_id, "units", y_unit);

	int retval;
	if (nc_get_att_text(ncid, NC_GLOBAL, "date", date))
	{	
		int day, month, year;
		retval = nc_get_att_int(ncid, NC_GLOBAL, "year", &year);
		if (!retval)
			retval = nc_get_att_int(ncid, NC_GLOBAL, "month", &month);
		if (!retval)
			nc_get_att_int(ncid, NC_GLOBAL, "day", &day);
		if (!retval)
			sprintf(date, "%d-%d-%d", year, month, day);
	}
		
	nc_close(ncid);
}


void netcdf_get_data(const char *filename, const char *var, float *data, float *x_labels, float *y_labels, char dimsname[2][NC_MAX_NAME + 1])
{
	int ncid, varid;
	int dimsid[NC_MAX_VAR_DIMS];
	
	nc_open(filename, NC_NOWRITE, &ncid);
	
	nc_inq_varid(ncid, var, &varid);
	nc_inq_var(ncid, varid, NULL, NULL, NULL, dimsid, NULL);

	for (int index = 0; index < 2; index ++)
		nc_inq_dim(ncid, dimsid[index], dimsname[index], NULL);


	nc_get_var_float(ncid, varid, &data[0]);
		
	int xid, yid;
	nc_inq_varid(ncid, dimsname[0], &xid);
	nc_get_var_float(ncid, xid, &x_labels[0]);
	nc_inq_varid(ncid, dimsname[1], &yid);
	nc_get_var_float(ncid, yid, &y_labels[0]);
	
	nc_close(ncid);
}