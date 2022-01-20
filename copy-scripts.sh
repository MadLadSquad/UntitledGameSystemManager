#!/bin/bash
# Make the directories for the scripts, resources and config files
mkdir -p ~/.config/UntitledLinuxGameManager/scripts/ || echo -e "\x1B[32mThe scripts folder already exists!\x1B[0m"
mkdir -p ~/.config/UntitledLinuxGameManager/resources/ || echo -e "\x1B[32mThe resources folder already exists!\x1B[0m"
mkdir -p ~/.config/UntitledLinuxGameManager/config/ || echo -e "\x1B[32mThe config folder already exists!\x1B[0m"

cp Installers/Scripts/ugm-cli-install.sh Installers/Scripts/ugm-cli-prepare-install.sh Installers/Scripts/ugm-cli-update.sh ~/.config/UntitledLinuxGameManager/scripts/

echo -e "\x1B[32mCopied scripts!\x1B[0m"