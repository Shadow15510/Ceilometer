#! /bin/bash

echo "installing NEVADA's data in /usr/bin/nevada_data"
sudo cp -r nevada_data /usr/bin/

echo "installing app in /usr/bin/"
sudo cp nevada /usr/bin/nevada

echo "creation of a desktop entry"
cat << EOF >> ~/.local/share/applications/nevada.desktop
[Desktop Entry]
Name=Nevada
Comment=NetCDF Explorer and Visualizer for Atmospheric Data Analysis
Icon=/usr/bin/nevada_data/netcdf-logo.png
Exec=/usr/bin/nevada
Type=Application
Categories=Utility
EOF
