/* NEVADA:netcdf_api

   This module contains all the interactions with NetCDF file.
*/

#ifndef _NETCDF_API_H
#define _NETCDF_API_H


/* netcdf_get_variables: gets the available variables
   
   This functions gets all the bidimensionnal variables of the given file. It
   also updates the combo box on GUI. */
void netcdf_get_variables(const char *filename);


/* netcdf_set_fits: fit the x and y selection to the selected variable
   
   According to the selected variable, this function adapts the x min, max and
   y min, max. This allows to extract a part of a larger data array. */
void netcdf_set_fits(const char *filename, const char *var);


/* netcdf_get_dim: extract metadata from the given file

   Extract metadata from the given file. Metadata are the axis' size ans units
   and the date. */
void netcdf_get_metadata(const char *filename, const char *var, size_t *x_axis, char *x_unit, size_t *y_axis, char *y_unit, char *date);


/* netcdf_get_data: extrait la variable donnée et les axes associés

   Extract the data itself and the corresponding axis' labels. Get also the
   dimensions. */
void netcdf_get_data(const char *filename, const char *var, float *data, float *x_labels, float *y_labels, char dimsname[2][NC_MAX_NAME + 1]);


#endif /* _NETCDF_API_H */