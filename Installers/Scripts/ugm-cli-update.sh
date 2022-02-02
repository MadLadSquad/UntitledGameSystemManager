#!/bin/bash
if [ "$1" == "--name" ] && [ "$2" != "" ] && [ "$3" == "--driver" ] && [ "$4" != "" ] && [ "$5" == "--gpu" ]; then
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
      echo -e "What video drivers do you use Mesa(AMD/Intel) or NVidia?"
      read -rp "M(mesa)/N(NVidia): " yn
      case $yn in
          [Mm]* ) pro=false;break;;
          [Nn]* ) pro=true;break;;
          * ) echo "Please answer with M(Mesa) or N(NVidia)!";;
      esac
  done

  while true; do
        echo -e "Do you want to update your GPU drivers?"
        read -rp "Y(Yes)/N(No): " yn
        case $yn in
            [Yy]* ) gpu=true;break;;
            [Nn]* ) gpu=false;break;;
            * ) echo "Please answer with Y(Yes) or N(No)!";;
        esac
    done
fi

lxc exec "${containerName}" -- bash -c "pacman -Syyu --noconfirm"
# get the nvidia driver version from glxinfo and then construct a new string with the version
if gpu && pro; then
  which glxinfo &> /dev/null || (echo -e "\x1B[31mError: glxinfo not found! The glxinfo program is needed in order to update your GPU drivers!\x1B[0m" && exit)
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
  lxc exec "${containerName}" -- bash -c "sed -i 's/IgnorePkg/#IgnorePkg   =/g' /etc/pacman.conf"
  lxc exec "${containerName}" -- bash -c "pacman -U --noconfirm https://archive.archlinux.org/packages/n/nvidia-utils/nvidia-utils-${nversion}-${up1}-x86_64.pkg.tar.zst https://archive.archlinux.org/packages/l/lib32-nvidia-utils/lib32-nvidia-utils-${nversion}-${up2}-x86_64.pkg.tar.zst"
  lxc exec "${containerName}" -- bash -c "sed -i 's/#IgnorePkg   =/IgnorePkg = lib32-nvidia-utils nvidia-utils/g' /etc/pacman.conf"
fi