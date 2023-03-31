#ifndef _SDL_API_H
#define _SDL_API_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "include/netcdf_data.h"


// sdl_image : trace les données traitées
void sdl_render(struct netcdf_data *data, const bool image_mode);

// sdl_labels : affiche les axes et la légende
void sdl_labels(struct netcdf_data *data, SDL_Renderer *renderer, const int WIDTH, const int HEIGHT);

// sdl_loop : enregistre les mesures et les sauvegarde au format CSV
void sdl_loop(SDL_Renderer *renderer, const char *filename, struct netcdf_data *data, const int HEIGHT);

// sdl_get_limits : analyse les données
void sdl_get_limits(struct netcdf_data *data);

// sdl_invert_sign : inverse le signe d'un flottant
float sdl_invert_sign(float a);

// sdl_render_text : affiche le texte donné sur le rendu aux coordonnées communiquées
void sdl_render_text(SDL_Renderer *renderer, TTF_Font *font, const int x, const int y, const char *text, const bool alpha_bg);

// sdl_save_renderer : exporte le renderer en png
void sdl_save_renderer(const char *file_name, SDL_Renderer *renderer, const int width, const int height);

// sdl_convert_epoch : convertit le epoch donné dans le format spécifié
void sdl_convert_epoch(const time_t epoch, const char *format, char *date);

#endif /* _SDL_API_H */