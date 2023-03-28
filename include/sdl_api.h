#ifndef _SDL_API_H
#define _SDL_API_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "include/netcdf_data.h"


// sdl_image : trace les données traitées et sauvegarde le résultat dans une image PNG à l'échelle 1:1
void sdl_image(struct netcdf_data *data);

// sdl_measure : affiche les données traitées avec un facteur 3 et sauvegarde les mesures au format CSV
void sdl_measure(struct netcdf_data *data);

// sdl_get_limits : analyse les données
void sdl_get_limits(struct netcdf_data *data);

// sdl_invert_sign : inverse le signe d'un flottant
float sdl_invert_sign(float a);

// sdl_loop : attend que la fenêtre soit fermée et gère les évènements
void sdl_loop(SDL_Renderer *renderer, const char *filename, struct netcdf_data *data);

// sdl_render_text : affiche le texte donné sur le rendu aux coordonnées communiquées
void sdl_render_text(SDL_Renderer *renderer, TTF_Font *font, const int x, const int y, const char *text, const bool alpha_bg);

// sdl_save_renderer : exporte le renderer en png
void sdl_save_renderer(const char *file_name, SDL_Renderer *renderer, const int width, const int height);


void sdl_convert_epoch(const time_t epoch, const char *format, char *date);

#endif /* _SDL_API_H */