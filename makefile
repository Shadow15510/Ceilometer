CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl2 SDL2_image SDL2_ttf`
LDFLAGS=`pkg-config --libs gtk+-3.0` `pkg-config --libs sdl2 SDL2_image SDL2_ttf` -rdynamic -lnetcdf -lm

EXEC=ceilometer
SRC=src/main.c src/callbacks.c src/netcdf_api.c src/sdl_api.c src/colors.c
OBJ=$(SRC:.c=.o)

all: $(EXEC) clean

# Compilation Linux
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o : include/main.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

# Nettoyage des sources
clean:
	rm src/*.o

clear:
	sudo rm -r /usr/bin/ceilometer_data
	sudo rm /usr/bin/ceilometer

install:
	sudo cp -r ceilometer_data /usr/bin/
	sudo mv ceilometer /usr/bin/ceilometer



