/* NEVADA:callbarcks

   This module covers all the GUI's callbacks.
*/
#ifndef _CALLBACKS_H
#define _CALLBACKS_H

#include <gtk/gtk.h>

// on_window_ceilometer_destroy: exit NEVADA
G_MODULE_EXPORT void on_window_ceilometer_destroy(void);


// on_file_netcdf_set: is called when a NetCDF file is selected
G_MODULE_EXPORT void on_file_netcdf_file_set(void);


// on_combo_vars_changed: is called when a variable is selected
G_MODULE_EXPORT void on_combo_vars_changed(void);


// on_check_filter: is called when the filter check box is (un)checked
G_MODULE_EXPORT void on_check_filter_toggled(void);


// on_radio_image_toggled: is called when the user change the opening mode to `image`
G_MODULE_EXPORT void on_radio_image_toggled(void);


// on_radio_measure_toggled: is called when the user change the opening mode to `measure`
G_MODULE_EXPORT void on_radio_measure_toggled(void);


// on_button_validation_clicked: is called when the user click on validation
G_MODULE_EXPORT void on_button_validation_clicked(void);


#endif /* _CALLBACKS_H */