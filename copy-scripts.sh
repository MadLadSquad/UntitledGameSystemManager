#!/bin/bash
# Make the directories for the scripts, resources and config files
mkdir -p ~/.config/UntitledLinuxGamingManager/scripts/
mkdir -p ~/.config/UntitledLinuxGamingManager/resources/
mkdir -p ~/.config/UntitledLinuxGamingManager/config/

cp Installers/Scripts/ugm-cli-install.sh Installers/Scripts/ugm-cli-prepare-install.sh ~/.config/UntitledLinuxGamingManager/scripts/

echo -e "\x1B[32mCopied scripts!\x1B[0m"