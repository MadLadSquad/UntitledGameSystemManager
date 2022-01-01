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

# Get the date and extract the last 2 letters, for use with the ubuntu image versioning system
year=$(date)
yearNum="${year: -2}"

# Check if the launch command works, if it doesn't decrement the year until we find a compatile one
while ! (lxc launch "images:ubuntu/${yearNum}.04" "${containerName}"); do ((yearNum--)); done

# Pass the GPU, pulse socket and X display, then define appropriate environment variables for them
lxc config device add "${containerName}" mygpu gpu
lxc config device add "${containerName}" X0 proxy bind=container connect=unix:/tmp/.X11-unix/X0 listen=unix:/tmp/.X11-unix/X0 uid=1000 gid=1000 mode=0777 security.uid=1000 security.gid=1000
lxc config device add "${containerName}" PASocket1 proxy bind=container connect=unix:/run/user/1000/pulse/native listen=unix:/home/ubuntu/pulse-native uid=1000 gid=1000 mode=0777 security.uid=1000 security.gid=1000
lxc config set "${containerName}" environment.DISPLAY :0
lxc config set "${containerName}" environment.PULSE_SERVER unix:/home/ubuntu/pulse-native
lxc config set "${containerName}" environment.PROTON_NO_ESYNC 1

# Add 32 bit binary support and install useful utilities
lxc exec "${containerName}" -- bash -c "dpkg --add-architecture i386"
lxc exec "${containerName}" -- bash -c "apt update -y && apt upgrade -y"
lxc exec "${containerName}" -- bash -c "apt install wget software-properties-common pciutils -y"

echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mInstalling drivers!\x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
if [ "${pro}" = false ]; then
	# Add the mesa ppa and install the drivers
	lxc exec "${containerName}" -- bash -c "add-apt-repository --yes ppa:kisak/kisak-mesa"
	lxc exec "${containerName}" -- bash -c "apt update -y && apt upgrade -y"
	lxc exec "${containerName}" -- bash -c "apt install libgl1-mesa-dri:i386 mesa-vulkan-drivers mesa-vulkan-drivers:i386 -y"
else
	# get the nvidia driver version from glxinfo and then construct a new string with the version
	nversiontmp=$(glxinfo | grep "OpenGL version string: 4.6.0 NVIDIA ")
	nversion=${nversiontmp: -6}

	# Install the nvidia driver ppa and install the drivers as well as the vulkan libraries using the driver version from the host
	lxc exec "${containerName}" -- bash -c "add-apt-repository --yes ppa:graphics-drivers/ppa"
	lxc exec "${containerName}" -- bash -c "apt update -y && apt upgrade -y"
	lxc exec "${containerName}" -- bash -c "apt install nvidia-driver-${nversion: : -3 } libvulkan1 libvulkan1:i386 -y"
fi

echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mInstalling steam, firefox, lutris, wine, winetricks and python!\x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
# Install steam, pulse, firefox, lutris, wine and a bunch of python dependencies
lxc exec "${containerName}" -- bash -c "wget -O ~/steam.deb http://media.steampowered.com/client/installer/steam.deb && apt install -y ~/steam.deb && rm ~/steam.deb"
lxc exec "${containerName}" -- bash -c "sed -i 's/; enable-shm = yes/enable-shm = no/g' /etc/pulse/client.conf"
lxc exec "${containerName}" -- bash -c "apt install firefox lutris winetricks python3-pip python3-setuptools python3-venv pipx -y"

# Install protontricks trough pipx
lxc exec "${containerName}" -- bash -c "pipx install protontricks && pipx ensurepath"

# Restart
lxc stop "${containerName}"
lxc start "${containerName}"



echo -e "\x1B[32mUbuntu container installation finished! You might experience network/audio problems, reboot and it should be fixed!\x1B[0m"
