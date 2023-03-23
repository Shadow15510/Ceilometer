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


void netcdf_get_metadata(const char *filename, size_t *time, size_t *range, int *year, int *month, int *day)
{
	int ncid, timeid, rangeid;

	nc_open(filename, NC_NOWRITE, &ncid);

	nc_inq_dimid(ncid, "time", &timeid);
	nc_inq_dimlen(ncid, timeid, time);
	
	nc_inq_dimid(ncid, "range", &rangeid);
	nc_inq_dimlen(ncid, rangeid, range);

	nc_get_att_int(ncid, NC_GLOBAL, "year", year);
	nc_get_att_int(ncid, NC_GLOBAL, "month", month);
	nc_get_att_int(ncid, NC_GLOBAL, "day", day);
	
	nc_close(ncid);
}


void netcdf_get_data(float *data, float *alt, const char *filename, const char *var)
{
	int ncid, dataid, altid;

	nc_open(filename, NC_NOWRITE, &ncid);
	
	nc_inq_varid(ncid, var, &dataid);
	nc_get_var_float(ncid, dataid, &data[0]);
	
	nc_inq_varid(ncid, "range", &altid);
	nc_get_var_float(ncid, altid, &alt[0]);
	
	nc_close(ncid);
}