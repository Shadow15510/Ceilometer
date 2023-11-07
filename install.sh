#! /bin/bash

echo "installing NEVADA's data in ~/.local/bin/nevada_data"
cp -r nevada_data ~/.local/bin/

echo "installing app in ~/.local/bin/"
cp nevada ~/.local/bin/nevada

echo "creation of a desktop entry"
cat << EOF >> ~/.local/share/applications/nevada.desktop
[Desktop Entry]
Name=Nevada
Comment=NetCDF Explorer and Visualizer for Atmospheric Data Analysis
Icon=$HOME/.local/bin/nevada_data/netcdf-logo.png
Exec=$HOME/.local/bin/nevada
Type=Application
Categories=Utility
EOF
