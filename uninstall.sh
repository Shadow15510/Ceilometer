#! /bin/bash

echo "Désinstallation de NEVADA"
echo "Mot de passe requis pour la déinstallation"
echo "-----"

echo "Suppression du dossier /usr/bin/nevada_data"
sudo rm -r /usr/bin/nevada_data

echo "Déinstallation de /usr/bin/nevada"
sudo rm /usr/bin/nevada

echo "Suppression de l'application"
sudo rm ~/.local/share/applications/nevada.desktop
