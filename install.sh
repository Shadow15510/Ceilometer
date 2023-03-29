#! /bin/bash

echo "Installation de NEVADA"
echo "Mot de passe requis pour l'installation"
echo "-----"

echo "installation du dossier /usr/bin/nevada_data"
sudo cp -r nevada_data /usr/bin/

echo "installation de l'application dans /usr/bin/"
sudo mv nevada /usr/bin/nevada

echo "Création d'une entrée dans la liste des applications"
echo -e "[Desktop Entry]\nVersion=1.0\nType=Application\nName=Nevada\nComment=NetCDF Explorer and Visualizer for Atmospheric Data Analysis\nIcon=/usr/bin/nevada_data/netcdf-logo.png\nExec=nevada\nPath=/usr/bin\nActions=\nCategories=Utility" > ~/.local/share/applications/nevada.desktop

