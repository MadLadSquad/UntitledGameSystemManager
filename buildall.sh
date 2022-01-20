#!/bin/bash
cpus=$(grep -c processor /proc/cpuinfo)
echo -e "\x1B[32mCompiling with ${cpus} compile jobs! \x1B[0m"
echo -e "\x1B[32mInstalling the GUI\x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"

./copy-scripts.sh

cd Installers/GUI/ || exit
mkdir build
cd build || exit
cmake .. -G "Unix Makefiles"
make -j "${cpus}"
cp ugm-gui-installer ../../../

cd ../../../

echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mCompiled the GUI installer! \x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"

cd Managers/CLI/ || exit
mkdir build
cd build || exit
cmake .. -G "Unix Makefiles"
make -j "${cpus}"
cp ugm-cli ../../../

cd ../../../

echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mCompiled the CLI manager! \x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"

cd Managers/GUI/ || exit
mkdir build
cd build || exit
cmake .. -G "Unix Makefiles"
make -j "${cpus}"
cp ugm-gui ../../../

cd ../../../

echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mCompiled the GUI manager! \x1B[0m"
echo -e "\x1B[32mNothing more to build, build finished!\x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"