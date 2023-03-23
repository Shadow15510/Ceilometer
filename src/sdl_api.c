#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL_image.h>

#include "include/sdl_api.h"
#include "include/colors.h"


void sdl_image(float *data, float *alt, float minimum, float maximum, const bool filter, const char *var, char *date, const size_t NTIME, const size_t NRANGE)
{
	// Initialisation de la SDL
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("", 0, 0, NTIME, NRANGE, SDL_WINDOW_HIDDEN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Initialisation de la TTF
	TTF_Init();
	TTF_Font *jetbrains = TTF_OpenFont("/usr/bin/ceilometer_data/fonts/JetBrainsMono-Bold.ttf", 40);
	
	SDL_Surface *surface_text = NULL;
	SDL_Texture *texture_text = NULL;
	char altitude[15], hour[15], scale[15];

	// Initialisation du rendu
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Traitement des données
	if (!filter) sdl_get_limits(data, &minimum, &maximum, NTIME, NRANGE);

	// Dessin du rendu
	for (int time = 0; time < NTIME; time ++)
	{
		for (int range = 0; range < NRANGE; range ++)
		{
			if (filter)
			{
				if (data[NRANGE * time + range] < minimum)
					data[NRANGE * time + range] = minimum;
				if (data[NRANGE * time + range] > maximum)
					data[NRANGE * time + range] = maximum;
			}
			// Affichage des données
			int color_index = 1019 - floor(1019 * (sdl_invert_sign(minimum) + data[NRANGE * time + range]) / (sdl_invert_sign(minimum) + maximum));
			SDL_SetRenderDrawColor(renderer, COLORS[color_index][0], COLORS[color_index][1], COLORS[color_index][2], 255);
			SDL_RenderDrawPoint(renderer, time, (NRANGE - range));
		}
	}

	// Échelles et grille
	for (int time = 0; time < NTIME; time ++)
	{
		for (int range = 0; range < NRANGE; range ++)
		{
			// Échelle des altitudes
			if (!(range % 50) && range && range < NRANGE - 50 && time == NTIME / 2)
			{
				int alt_val = floor(alt[range]);
				sprintf(altitude, "%d m", alt_val);
				sdl_render_text(renderer, jetbrains, 2, NRANGE - (range + 52), altitude, true);
			}

			// Grille
			if (!(range % 50) && range && range < NRANGE - 50 && !(time % 240) && time)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
				SDL_Rect rect = {time - 5, range - 5, 10, 10};
				SDL_RenderFillRect(renderer, &rect);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
				SDL_RenderDrawRect(renderer, &rect);
			}
		}
		
		// Échelle des temps
		if (!(time % 240) && time)
		{
			int hour_val = floor(time / 240);
			if (hour_val < 10) sprintf(hour, "0%d:00", hour_val);
			else sprintf(hour, "%d:00", hour_val);

			sdl_render_text(renderer, jetbrains, time - 60, NRANGE - 55, hour, true);
		}
	}

	// Dessin du nuancier de couleur
	for (int x = 0; x < 2040; x ++)
	{
		int color_index = 1019 - floor(x / 2);
		SDL_SetRenderDrawColor(renderer, COLORS[color_index][0], COLORS[color_index][1], COLORS[color_index][2], 255);
		
		for (int y = 10; y < 140; y ++)
			SDL_RenderDrawPoint(renderer, NTIME - 2050 + x, y);
	}
	for (int color_index = 0; color_index <= 1020; color_index ++)
	{
		if (!(color_index % 255))
		{
			float scale_value = minimum + ((color_index * (maximum - minimum)) / 1020);
			sprintf(scale, "% 1.2e", scale_value);
			sdl_render_text(renderer, jetbrains, NTIME - 2040 + 1.75 * color_index, 50, scale, false);
		}
	}
	SDL_Rect rect = {NTIME - 2050, 10, 2040, 130};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);

	// Affichage des métadonnées
	char metadata[50];
	sprintf(metadata, "%s %s", var, date);
	SDL_Color color = {0, 0, 0};
	surface_text = TTF_RenderText_Solid(jetbrains, metadata, color);
	texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);
	int text_width = 0, text_height = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &text_width, &text_height);
	SDL_Rect textrect = {NTIME - 2060 - text_width, 10, text_width, text_height};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80);
	SDL_RenderFillRect(renderer, &textrect);
	SDL_RenderCopy(renderer, texture_text, NULL, &textrect);


	// Exportation au format PNG
	char filename[100];
	sprintf(filename, "/home/%s/%s_%s.png", getenv("USER"), var, date);
	sdl_save_renderer(filename, renderer, NTIME, NRANGE);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_FreeSurface(surface_text);
	SDL_DestroyTexture(texture_text);

	SDL_Quit();
	TTF_CloseFont(jetbrains);
	TTF_Quit();
}


void sdl_measure(float *data, float *alt, float minimum, float maximum, const bool filter, const char *var, char *date, const size_t NTIME, const size_t NRANGE)
{
	// Initialisation de la SDL
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("", 0, 0, NTIME / 3, NRANGE / 3 - 100, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Initialisation du rendu
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Traitement des données
	if (!filter) sdl_get_limits(data, &minimum, &maximum, NTIME, NRANGE);
	
	// Dessin du rendu
	for (int time = 0; time < NTIME / 3; time ++)
	{
		for (int range = 0; range < NRANGE / 3 - 100; range ++)
		{		
			if (filter)
			{
				if (data[NRANGE * (time * 3) + (range * 3)] < minimum)
					data[NRANGE * (time * 3) + (range * 3)] = minimum;
				if (data[NRANGE * (time * 3) + (range * 3)] > maximum)
					data[NRANGE * (time * 3) + (range * 3)] = maximum;
			}

			// Affichage des données
			int color_index = 1019 - floor(1019 * (sdl_invert_sign(minimum) + data[NRANGE * (time * 3) + (range * 3)]) / (sdl_invert_sign(minimum) + maximum));
			SDL_SetRenderDrawColor(renderer, COLORS[color_index][0], COLORS[color_index][1], COLORS[color_index][2], 255);
			
			SDL_RenderDrawPoint(renderer, time, (NRANGE / 3 - 100 - range));
		}
	}
	SDL_RenderPresent(renderer);

	char metadata[50];
	sprintf(metadata, "%s %s (mesures)", var, date);

	char filename[100];
	sprintf(filename, "/home/%s/%s_%s.csv", getenv("USER"), var, date);

	sdl_loop(renderer, data, alt, filename, var, NRANGE);

	// Initialisation de la TTF
	TTF_Init();
	TTF_Font *jetbrains = TTF_OpenFont("/usr/bin/ceilometer_data/fonts/JetBrainsMono-Bold.ttf", 20);
	sdl_render_text(renderer, jetbrains, 2, 2, metadata, true);

	sprintf(filename, "/home/%s/%s_mesures_%s.png", getenv("USER"), var, date);
	sdl_save_renderer(filename, renderer, NTIME / 3, NRANGE / 3 - 100);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	TTF_CloseFont(jetbrains);
	TTF_Quit();
}


void sdl_get_limits(float *data, float *minimum, float *maximum, const size_t NTIME, const size_t NRANGE)
{	
	for (int time = 0; time < NTIME; time ++)
	{
		for (int range = 0; range < NRANGE; range ++)
		{
			if (data[NRANGE * time + range] < *minimum)
				*minimum = data[NRANGE * time + range];
			if (data[NRANGE * time + range] > *maximum)
				*maximum = data[NRANGE * time + range];
		}
	}
}


float sdl_invert_sign(float a)
{
	return -a;
}


void sdl_loop(SDL_Renderer *renderer, float *data, float *alt, const char *filename, const char *var, const size_t NRANGE)
{
	uint8_t exit = 0;
	int x, y, index = 1;
	char csv_row[100], coord_index[100], hour[5], min[5];

	SDL_Event event;
	TTF_Init();
	TTF_Font *jetbrains = TTF_OpenFont("/usr/bin/ceilometer_data/fonts/JetBrainsMono-Bold.ttf", 20);

	FILE *file = NULL;
	file = fopen(filename, "w");
	sprintf(csv_row, "identifiant; %s; altitude (m); heure\n", var);
	fputs(csv_row, file);

	while (event.type != SDL_QUIT)
	{
		SDL_PollEvent(&event);

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;

				float targeted_data = data[NRANGE * (x * 3) + NRANGE - 3 * (y + 100)];
				int targeted_alt = floor(alt[NRANGE - 3 * (y + 100)]);
				int targeted_hour = floor(3 * x / 240);
				int targeted_min = floor(60 * ((3 * x) % 240) / 240);

				if (targeted_hour < 10)
					sprintf(hour, "0%d", targeted_hour);
				else
					sprintf(hour, "%d", targeted_hour);

				if (targeted_min < 10)
					sprintf(min, "0%d", targeted_min);
				else
					sprintf(min, "%d", targeted_min);

				SDL_Rect x_rect = {x - 5, y - 1, 10, 3};
				SDL_Rect y_rect = {x - 1, y - 5, 3, 10};
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &x_rect);
				SDL_RenderFillRect(renderer, &y_rect);

				sprintf(coord_index, "%d", index);
				sdl_render_text(renderer, jetbrains, x + 2, y + 2, coord_index, true);

				sprintf(csv_row, "%d; %f; %d; %s:%s\n", index, targeted_data, targeted_alt, hour, min);

				SDL_RenderPresent(renderer);				
				fputs(csv_row, file);
				index ++;

				break;
		}
	}
	fclose(file);
	TTF_CloseFont(jetbrains);
	TTF_Quit();
}


void sdl_render_text(SDL_Renderer *renderer, TTF_Font *font, const int x, const int y, const char *text, const bool alpha_bg)
{
	SDL_Color color = {0, 0, 0};
	SDL_Surface *surface_text;
	SDL_Texture *texture_text;

	surface_text = TTF_RenderText_Solid(font, text, color);
	texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);
	
	int text_width = 0, text_height = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &text_width, &text_height);

	SDL_Rect textrect = {x, y, text_width, text_height};
	if (alpha_bg)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80);
		SDL_RenderFillRect(renderer, &textrect);
	}

	SDL_RenderCopy(renderer, texture_text, NULL, &textrect);
	SDL_FreeSurface(surface_text);
	SDL_DestroyTexture(texture_text);
}


void sdl_save_renderer(const char *file_name, SDL_Renderer *renderer, const int width, const int height)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, file_name);
    
    SDL_FreeSurface(surface);
}
