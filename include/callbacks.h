#ifndef _CALLBACKS_H
#define _CALLBACKS_H

#include <gtk/gtk.h>


G_MODULE_EXPORT void on_window_ceilometer_destroy(void);
G_MODULE_EXPORT void on_file_netcdf_file_set(void);
G_MODULE_EXPORT void on_combo_vars_changed(void);
G_MODULE_EXPORT void on_check_filter_toggled(void);
G_MODULE_EXPORT void on_radio_image_toggled(void);
G_MODULE_EXPORT void on_radio_measure_toggled(void);
G_MODULE_EXPORT void on_button_validation_clicked(void);

#endif /* _CALLBACKS_H */