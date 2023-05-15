/* NEVADA:netcdf_data

   This module define the structure `netcdf_data`. `netcdf_data` contains all
   the important informations about the NetCDF variable the user want to
   extract.
*/

#ifndef _NETCDF_DATA_H
#define _NETCDF_DATA_H

#include <stdbool.h>


struct netcdf_data
{
	float *var;
	const char *varname;

	const size_t X_AXIS;
	const float *x_labels;
	const char *x_name;
	const int x_min;
	const int x_max;
	float x_factor;

	const size_t Y_AXIS;
	const float *y_labels;
	const char *y_name;
	const int y_min;
	const int y_max;
	float y_factor;
	const char *y_unit;

	const char *date;

	float minimum;
	float maximum;
	const bool filter;
};


#endif /* _NETCDF_DATA_H */