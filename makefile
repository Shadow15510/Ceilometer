CC=gcc
CFLAGS=-I. `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl2 SDL2_image SDL2_ttf`
LDFLAGS=`pkg-config --libs gtk+-3.0` `pkg-config --libs sdl2 SDL2_image SDL2_ttf` -rdynamic -lnetcdf -lm

EXEC=nevada
SRC=src/main.c src/callbacks.c src/netcdf_api.c src/sdl_api.c src/colors.c
OBJ=$(SRC:%.c=build/%.o)

all: $(EXEC)

# Compilation Linux
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o : include/main.h

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

# Nettoyage des sources
clean:
	rm -rf build	

clear:
	sudo echo "Désinstallation de NEVADA"
	sudo rm -r /usr/bin/nevada_data
	sudo rm /usr/bin/nevada
	sudo rm ~/.local/share/applications/nevada.desktop

install:
	sudo echo "Installation de NEVADA"
	sudo cp -r nevada_data /usr/bin/
	sudo mv nevada /usr/bin/nevada
	echo -e "[Desktop Entry]\nVersion=1.0\nType=Application\nName=Nevada\nComment=NetCDF Explorer and Visualizer for Atmospheric Data Analysis\nIcon=/usr/bin/nevada_data/netcdf-logo.png\nExec=nevada\nPath=/usr/bin\nActions=\nCategories=Utility" > ~/.local/share/applications/nevada.desktop




