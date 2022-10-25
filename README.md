# UntitledLinuxGameManager
[![CI](https://github.com/MadLadSquad/UntitledLinuxGameManager/actions/workflows/ci.yml/badge.svg)](https://github.com/MadLadSquad/UntitledLinuxGameManager/actions/workflows/ci.yml)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
[![trello](https://img.shields.io/badge/Trello-UDE-blue])](https://trello.com/b/HmfuRY2K/untitleddesktop)
[![Discord](https://img.shields.io/discord/717037253292982315.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/4wgH8ZE)
 
## Major announcement: After a big 6 months of no development the application is planned for a complete rewrite with the LXC C API and the new UntitledDesktop framework! When the rewrite is ready it will be pushed as tag 1.0 any issues present will be fixed in the new release!
A linux gaming system manager
## About
UntitledLinuxGameManager is a cli/gui program to help with managing a linux gaming system.

It automatically creates an Ubuntu container with the latest updates and software, and gives you a nice gui/cli interface to manage the programs in the container.

Using a container users can:
- isolate their gaming system from the host
- easily backup, redeploy or move their system
- increase game/program stability and compatibility
## Dependencies
The manager requires the following to be installed on your system:
- X
- PulseAudio
- glxinfo
- An AMD/Intel/Nvidia GPU with supported drivers
- The cli install scripts require bash, since for now they include bash isms and don't fully conform
## Install
You can find a guide [here](https://github.com/MadLadSquad/UntitledLinuxGameManager/wiki/Installing)
## Showcase
The standalone GUI manager window:

![image](https://user-images.githubusercontent.com/40400590/155132857-adee86d8-cea2-44ee-be05-c9fdd34dec99.png)

The GUI manager and game launchers:

![image](https://user-images.githubusercontent.com/40400590/155133030-bd2e2fad-cacd-4093-abdc-d7931147d84c.png)


