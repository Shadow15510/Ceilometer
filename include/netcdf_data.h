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
	const float factor_x;

	const size_t Y_AXIS;
	const float *y_labels;
	const char *y_name;
	const float factor_y;
	const char *y_unit;

	const char *date;

	float minimum;
	float maximum;
	const bool filter;
};


#endif /* _NETCDF_DATA_H */