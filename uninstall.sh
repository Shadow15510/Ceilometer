#! /bin/bash

echo "deleting the folder ~/.local/bin/nevada_data"
rm -r ~/.local/bin/nevada_data

echo "removing of ~/.local/bin/nevada"
rm ~/.local/bin/nevada

echo "deleting the desktop entry"
rm ~/.local/share/applications/nevada.desktop
