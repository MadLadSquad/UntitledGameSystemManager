#!/bin/bash
# Make the directories for the scripts, resources and config files
mkdir -p ~/.config/UntitledLinuxGameManager/scripts/
mkdir -p ~/.config/UntitledLinuxGameManager/resources/
mkdir -p ~/.config/UntitledLinuxGameManager/config/

cp Installers/Scripts/ugm-cli-install.sh Installers/Scripts/ugm-cli-prepare-install.sh ~/.config/UntitledLinuxGameManager/scripts/

echo -e "\x1B[32mCopied scripts!\x1B[0m"