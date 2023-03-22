#ifndef _SDL_API_H
#define _SDL_API_H

#include <stdbool.h>
#include<SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


// sdl_image : trace les données traitées et sauvegarde le résultat dans une image PNG à l'échelle 1:1
void sdl_image(float *data, float *alt, float minimum, float maximum, const bool filter, const char *var, char *date, const size_t NTIME, const size_t NRANGE);

// sdl_measure : affiche les données traitées avec un facteur 3 et sauvegarde les mesures au format CSV
void sdl_measure(float *data, float *alt, float minimum, float maximum, const bool filter, const char *var, char *date, const size_t NTIME, const size_t NRANGE);

// sdl_get_limits : analyse les données
void sdl_get_limits(float *data, float *minimum, float *maximum, const size_t NTIME, const size_t NRANGE);

// sdl_invert_sign : inverse le signe d'un flottant
float sdl_invert_sign(float a);

// sdl_loop : attend que la fenêtre soit fermée et gère les évènements
void sdl_loop(SDL_Renderer *renderer, float *data, float *alt, const char *filename, const char *var, const size_t NRANGE);

// sdl_render_text : affiche le texte donné sur le rendu aux coordonnées communiquées
void sdl_render_text(SDL_Renderer *renderer, TTF_Font *font, const int x, const int y, const char *text, const bool alpha_bg);

// sdl_save_renderer : exporte le renderer en png
void sdl_save_renderer(const char *file_name, SDL_Renderer *renderer, const int width, const int height);


#endif /* _SDL_API_H */