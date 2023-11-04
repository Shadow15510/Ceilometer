/* NEVADA:sdl_api

	This module covers all the functions which use the SDL. From the
	initialisation of the renderer to the representation.
*/

#ifndef _SDL_API_H
#define _SDL_API_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "include/netcdf_data.h"


/* sdl_render: manage SDL part

   This function serves as a sort of main function for SDL calls.

   It takes as input the variable to represent as a netcdf_data structure. And
   according to the opening mode, sdl_render will save the fig into a PNG file
   of display it to the screen. */
void sdl_measure(struct netcdf_data *data);


void sdl_render_var2d(struct netcdf_data *data);



/* sdl_labels: add axis' labels and legend to the renderer
   
   Once the renderer is initialized with the figure by sdl_render, we can add axis
   and legend to the renderer via sdl_rendrer. */
void sdl_labels(struct netcdf_data *data, SDL_Renderer *renderer, const int WIDTH, const int HEIGHT);


/* sdl_loop: records and save measures into a CSV file

   With the measure opening mode, you can make measure directly from the
   on-screen figure. This function allows to show the fig and to manage the
   input.

   All the measures are savec into a CSV, separated by semicolon (;). Here is
   the header:
   ```
   id; value; x-axis; y-axis
   ```
   The image with the id's is also save into a PNG file.

   All the saved files are stored into your personnal folder. */
void sdl_loop(SDL_Renderer *renderer, const char *filename, struct netcdf_data *data, const int HEIGHT);


/* sdl_get_limits: analyze data
   
   When no filter is given, NEVADA take the minimum value and maximum one to
   make the color scale. So this function browse the data and find these two
   values. */
void sdl_get_limits(struct netcdf_data *data);


// sdl_invert_sign: invert the sign of a float
float sdl_invert_sign(float a);

/* sdl_render_text: put the given text on the renderer

   This function allows to add text to the renderer. Font and colors are
   customizable. */
void sdl_render_text(SDL_Renderer *renderer, TTF_Font *font, const int x, const int y, const char *text, const bool alpha_bg);


/* sdl_save_renderer: export renderer

   Allow to save the renderer into a PNG file. */
void sdl_save_renderer(const char *file_name, SDL_Renderer *renderer, const int width, const int height);


/* sdl_convert_epoch: convert time

   Converts the given time in seconds since epoch into a human readable format.
   This function handles all the standard formats of the <time.h> module.

   The `char *date` parameter is the output. */
void sdl_convert_epoch(const time_t epoch, const char *format, char *date);


#endif /* _SDL_API_H */