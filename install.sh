#! /bin/bash

echo "installing NEVADA's data in /usr/bin/nevada_data"
sudo cp -r nevada_data /usr/bin/

echo "installing app in /usr/bin/"
sudo mv nevada /usr/bin/nevada

echo "creation of a desktop entry"
echo -e "[Desktop Entry]\nVersion=1.0\nType=Application\nName=Nevada\nComment=NetCDF Explorer and Visualizer for Atmospheric Data Analysis\nIcon=/usr/bin/nevada_data/netcdf-logo.png\nExec=nevada\nPath=/usr/bin\nActions=\nCategories=Utility" > ~/.local/share/applications/nevada.desktop

