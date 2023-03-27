#ifndef _NETCDF_API_H
#define _NETCDF_API_H


void netcdf_get_variables(const char *filename);

// netcdf_get_dim : extrait les dimensions temporelles et spatiales ainsi que la date
void netcdf_get_metadata(const char *filename, const char *var, size_t *time, size_t *range, int *year, int *month, int *day);

// netcdf_get_data : extrait les données et les altitudes de mesures
void netcdf_get_data(float *data, float *alt, const char *filename, const char *var);


#endif /* _NETCDF_API_H */