#!/bin/bash
if [ "$1" == "--name" ] && [ "$2" != "" ] && [ "$3" == "--driver" ] && [ "$4" != "" ]; then
  if [ "$4" == "N" ]; then
    pro=true
    containerName="$2"
  elif [ "$4" == "M" ]; then
    pro=false
    containerName="$2"
  else
    echo "You're trying to do an automated install but didn't provide the right arguments!"
    exit
  fi
else
  read -rp "Enter your container's name: " containerName

  while true; do
      echo -e "What video drivers do you use want to use Mesa(AMD/Intel) or NVidia?"
      read -rp "M(mesa)/N(NVidia): " yn
      case $yn in
          [Mm]* ) pro=false;break;;
          [Nn]* ) pro=true;break;;
          * ) echo "Please answer with M(Mesa) or N(NVidia)!";;
      esac
  done
fi
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mStarting container installation!\x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"

lxc exec "${containerName}" -- bash -c "pacman -Syyu --noconfirm || pacman -Syu --noconfirm"
# Add 32 bit binary support and install useful utilities
lxc exec "${containerName}" -- bash -c "sed -i '/^#\[multilib\]$/ {N; s/#\[multilib\]\n#Include = \/etc\/pacman.d\/mirrorlist/\[multilib\]\nInclude = \/etc\/pacman.d\/mirrorlist/g}' /etc/pacman.conf"
lxc exec "${containerName}" -- bash -c "pacman -Syyu --noconfirm wget vim"

echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mInstalling drivers!\x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"

if [ "${pro}" = false ]; then
  # TODO: Add support for mesa
	# Add the mesa ppa and install the drivers
	lxc exec "${containerName}" -- bash -c "add-apt-repository --yes ppa:kisak/kisak-mesa"
	lxc exec "${containerName}" -- bash -c "apt update -y && apt upgrade -y"
	lxc exec "${containerName}" -- bash -c "apt install libgl1-mesa-dri:i386 mesa-vulkan-drivers mesa-vulkan-drivers:i386 -y"
else
	# get the nvidia driver version from glxinfo and then construct a new string with the version
	nversiontmp=$(glxinfo | grep "OpenGL version string: 4.6.0 NVIDIA ")
	nversion=${nversiontmp: -6}

  # Iterate the archive for the newest iteration of the 64 bit nvidia drivers
  up1=1
  for ((;;)); do
    stat1=$(curl -Is "https://archive.archlinux.org/packages/n/nvidia-utils/nvidia-utils-${nversion}-${up1}-x86_64.pkg.tar.zst" | head -n 1)

    if echo "${stat1}" | grep "200"; then
      ((up1+=1))
    else
      ((up1-=1))
      break;
    fi
  done

  # Iterate the archive for the newest iteration of the 32bit nvidia drivers and libraries
  up2=1
  for ((;;)); do
    stat2=$(curl -Is "https://archive.archlinux.org/packages/l/lib32-nvidia-utils/lib32-nvidia-utils-${nversion}-${up2}-x86_64.pkg.tar.zst" | head -n 1)
    if echo "${stat2}" | grep "200"; then
      ((up2+=1))
    else
      ((up2-=1))
      break;
    fi
  done

	# Install the nvidia driver and related libraries
	lxc exec "${containerName}" -- bash -c "pacman -U --noconfirm https://archive.archlinux.org/packages/n/nvidia-utils/nvidia-utils-${nversion}-${up1}-x86_64.pkg.tar.zst https://archive.archlinux.org/packages/l/lib32-nvidia-utils/lib32-nvidia-utils-${nversion}-${up2}-x86_64.pkg.tar.zst"
	echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
  echo -e "\x1B[32mInstalling steam, firefox, lutris, wine, winetricks and python!\x1B[0m"
  echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
  # Install steam, pulse, firefox, lutris, wine and a bunch of python dependencies
  lxc exec "${containerName}" -- bash -c "pacman -S --noconfirm pulseaudio steam lutris firefox wine winetricks python-pip python-pipx python-setuptools python-virtualenv"
	lxc exec "${containerName}" -- bash -c "sed -i 's/#IgnorePkg   =/IgnorePkg = lib32-nvidia-utils nvidia-utils/g' /etc/pacman.conf"
fi

# Create an ubuntu user on arch for compatibility reasons lmao
lxc exec "${containerName}" -- bash -c "useradd -m ubuntu && usermod -G wheel,audio,video ubuntu"

# Install protontricks trough pipx
lxc exec "${containerName}" -- bash -c "pipx install protontricks && pipx ensurepath"

# Restart
lxc stop "${containerName}"
lxc start "${containerName}"

lxc exec "${containerName}" -- bash -c "sed -i 's/; enable-shm = yes/enable-shm = no/g' /etc/pulse/client.conf"
lxc exec "${containerName}" -- bash -c "sed -i 's/autospawn = no/autospawn = yes/g' /etc/pulse/client.conf"

(grep "containers:" ~/.config/UntitledLinuxGameManager/config/layout.yaml &> /dev/null && echo "\
  - container: $containerName
    pins:
      - steam
      - lutris
      - firefox
" >> ~/.config/UntitledLinuxGameManager/config/layout.yaml) || echo "\
containers:
  - container: $containerName
    pins:
      - steam
      - lutris
      - firefox
" >> ~/.config/UntitledLinuxGameManager/config/layout.yaml

lxc restart "${containerName}"

lxc config set "${containerName}" environment.PROTON_NO_ESYNC 1
lxc config set "${containerName}" environment.PULSE_SERVER unix:/pulse-native
lxc config set "${containerName}" environment.DISPLAY :0
lxc config device add "${containerName}" PASocket1 proxy bind=container connect=unix:/run/user/1000/pulse/native listen=unix:/pulse-native uid=1000 gid=1000 mode=0777 security.uid=1000 security.gid=1000
lxc config device add "${containerName}" mygpu gpu
lxc config device add "${containerName}" X0 proxy bind=container connect=unix:/tmp/.X11-unix/X0 listen=unix:/tmp/.X11-unix/X0 uid=1000 gid=1000 mode=0777 security.uid=1000 security.gid=1000

echo -e "\x1B[32mContainer installation finished! You might experience network/audio problems, reboot and it should be fixed!\x1B[0m"