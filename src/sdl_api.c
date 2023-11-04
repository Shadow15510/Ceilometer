#include <gtk/gtk.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "include/sdl_api.h"
#include "include/colors.h"


extern GtkBuilder *builder;

void sdl_measure(struct netcdf_data *data)
{
	setlocale(LC_ALL, "C");
	int WIDTH = floor(data->x_factor * (data->x_max - data->x_min));
	int HEIGHT = floor(data->y_factor * (data->y_max - data->y_min));

	int x_factor = ceil((float) WIDTH / 1920.), y_factor = ceil((float) HEIGHT / 1080.);

	if (x_factor <= 1)
		x_factor = 1;
	if (y_factor <= 1)
		y_factor = 1;

	data->x_factor /= x_factor;
	WIDTH /= x_factor;
	data->y_factor /= y_factor;
	HEIGHT /= y_factor;

	// SDL initialization
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Renderer initialization
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Processing of the data
	if (!data->filter) sdl_get_limits(data);

	GtkProgressBar *pbar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));

	// Draw render
	for (int x = 0; x < WIDTH; x ++)
	{
		gtk_progress_bar_set_fraction(pbar, (gdouble) (x + 1) / WIDTH);
		gtk_main_iteration_do(false);
		for (int y = 0; y < HEIGHT; y ++)
		{
			int data_x = x / data->x_factor + data->x_min;
			int data_y = y / data->y_factor + data->y_min;

			int index = floor(data->Y_AXIS * data_x + data_y);
			if (data->filter)
			{
				if (data->var[index] < data->minimum)
					data->var[index] = data->minimum;
				if (data->var[index] > data->maximum)
					data->var[index] = data->maximum;
			}
			// color scale
			int color_index = 1019 - floor(1019 * (sdl_invert_sign(data->minimum) + data->var[index]) / (sdl_invert_sign(data->minimum) + data->maximum));
			if (color_index >= 0 && color_index < 1020)
			{
				SDL_SetRenderDrawColor(renderer, COLORS[color_index][0], COLORS[color_index][1], COLORS[color_index][2], 255);
				SDL_RenderDrawPoint(renderer, x, HEIGHT - y);
			}
		}
	}

	char filename[100];
	sprintf(filename, "%s/%s_%s.csv", getenv("HOME"), data->varname, data->date);

	SDL_RenderPresent(renderer);
	sdl_loop(renderer, filename, data, HEIGHT);

	char metadata[50];
	sprintf(metadata, "%s %s (mesures)", data->varname, data->date);

	// Write the metadata on the image
	TTF_Init();
	TTF_Font *jetbrains = TTF_OpenFont("/usr/bin/nevada_data/fonts/cmu.serif-roman.ttf", 20);
	sdl_render_text(renderer, jetbrains, 2, 2, metadata, true);
	TTF_CloseFont(jetbrains);
	TTF_Quit();

	sprintf(filename, "%s/%s_mesures_%s.png", getenv("HOME"), data->varname, data->date);
	sdl_save_renderer(filename, renderer, WIDTH, HEIGHT);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();	
}



void sdl_render_var2d(struct netcdf_data *data)
{
	setlocale(LC_ALL, "C");
	int WIDTH = floor(data->x_factor * (data->x_max - data->x_min));
	int HEIGHT = floor(data->y_factor * (data->y_max - data->y_min));

	char xlabels[300] = {'\0'}, ylabels[300] = {'\0'};
	char tmp[64] = {'\0'};
	GtkProgressBar *pbar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));

	// Draw render
	FILE *temp = fopen("output.dat", "w");
	for (int x = 0; x < WIDTH; x ++)
	{
		gtk_progress_bar_set_fraction(pbar, (gdouble) (x + 1) / WIDTH);
		gtk_main_iteration_do(false);

		if (x % (int) (WIDTH / 10) == 0)
		{
			int index = (int) floor(x / data->x_factor + data->x_min);
			sdl_convert_epoch((time_t) data->x_labels[index], "%H:%M", tmp);
			if (x != 0)
				sprintf(xlabels, "%s, '%s' %d", xlabels, tmp, x);
			else
				sprintf(xlabels, "'%s' %d", tmp, x);
		}

		for (int y = 0; y < HEIGHT; y ++)
		{
			if (y % (int) (HEIGHT / 10) == 0)
			{
				int label = floor(data->y_labels[(int) floor(y / data->y_factor + data->y_min)]);
				if (y != 0)
					sprintf(ylabels, "%s, '%d %s' %d", ylabels, label, data->y_unit, y);
				else
					sprintf(ylabels, "'%d %s' %d", label, data->y_unit, y);
			}

			int data_x = x / data->x_factor + data->x_min;
			int data_y = y / data->y_factor + data->y_min;

			int index = floor(data->Y_AXIS * data_x + data_y);
			if (data->filter)
			{
				if (data->var[index] < data->minimum)
					data->var[index] = data->minimum;
				if (data->var[index] > data->maximum)
					data->var[index] = data->maximum;
			}
			fprintf(temp, "%d %d %f\n", x, y, data->var[index]);
		}
		fprintf(temp, "\n");
	}
	fclose(temp);

	// Calling Gnuplot
	FILE *gnuplot = popen("gnuplot", "w");
	fprintf(gnuplot, "set term jpeg size %d, %d font '/usr/bin/nevada_data/font/cmu.serif-roman.ttf, %d'\n", WIDTH, HEIGHT, (int) (WIDTH / 90));
	fprintf(gnuplot, "set xlabel '%s'\n", data->x_name);
	fprintf(gnuplot, "set ylabel '%s'\n", data->y_name);
	fprintf(gnuplot, "set title '%s %s' noenhanced\n", data->varname, data->date);
	fprintf(gnuplot, "set xtics (%s)\n", xlabels);
	fprintf(gnuplot, "set ytics (%s)\n", ylabels);
	fprintf(gnuplot, "set grid xtics ytics; set grid\n");
	fprintf(gnuplot, "set output '%s/%s_%s.jpg'; set view map; set xrange [*: *] noextend; set yrange [*: *] noextend; set pm3d\n", getenv("HOME"), data->varname, data->date);
	fprintf(gnuplot, "splot 'output.dat' using 1:2:3 with pm3d notitle\n");
	pclose(gnuplot);

	system("rm output.dat");
}


void sdl_loop(SDL_Renderer *renderer, const char *filename, struct netcdf_data *data, const int HEIGHT)
{
	uint8_t exit = 0;
	int x, y, index = 1;
	char csv_row[100], coord_index[100], hour[5], min[5];

	SDL_Event event;
	TTF_Init();
	TTF_Font *jetbrains = TTF_OpenFont("/usr/bin/nevada_data/font/cmu.serif-roman.ttf", 20);

	FILE *file = NULL;
	file = fopen(filename, "w");
	sprintf(csv_row, "id; %s; %s; %s\n", data->varname, data->x_name, data->y_name);
	fputs(csv_row, file);

	while (event.type != SDL_QUIT)
	{
		SDL_PollEvent(&event);

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;

				int data_x = floor(x / data->x_factor + data->x_min);
				int data_y = floor((HEIGHT - y) / data->y_factor + data->y_min);

				float targeted_data = data->var[(int) floor(data->Y_AXIS * data_x + data_y)];
				int targeted_x = floor(data->x_labels[data_x]);
				int targeted_y = floor(data->y_labels[data_y]);

				char date[255];
				sdl_convert_epoch((time_t) targeted_x, "%H:%M", date);

				SDL_Rect x_rect = {x - 5, y - 1, 10, 3};
				SDL_Rect y_rect = {x - 1, y - 5, 3, 10};
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &x_rect);
				SDL_RenderFillRect(renderer, &y_rect);

				sprintf(coord_index, "%d", index);
				sdl_render_text(renderer, jetbrains, x + 2, y + 2, coord_index, true);

				sprintf(csv_row, "%d; %f; %s; %d\n", index, targeted_data, date, targeted_y);

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


void sdl_get_limits(struct netcdf_data *data)
{	
	for (int x = 0; x < data->X_AXIS; x ++)
	{
		for (int y = 0; y < data->Y_AXIS; y ++)
		{
			if (data->var[data->Y_AXIS * x + y] < data->minimum)
				data->minimum = data->var[data->Y_AXIS * x + y];
			if (data->var[data->Y_AXIS * x + y] > data->maximum)
				data->maximum = data->var[data->Y_AXIS * x + y];
		}
	}
}


float sdl_invert_sign(float a)
{
	return -a;
}


void sdl_save_renderer(const char *filename, SDL_Renderer *renderer, const int width, const int height)
{
	SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
	IMG_SavePNG(surface, filename);

	SDL_FreeSurface(surface);
}


void sdl_convert_epoch(const time_t epoch, const char *format, char *date)
{
	struct tm tm;
	tm = *gmtime(&epoch);
	strftime(date, sizeof(date), format, &tm);
}
