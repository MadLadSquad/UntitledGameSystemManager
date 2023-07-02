#!/bin/bash
if [ "$1" != "" ] ; then
  if [ "$1" == "N" ]; then
    pro=true
    if [ "$2" != "" ]; then
      nversion="$2"
    else
      echo "Driver for nvidia GPU not provided"
      exit
    fi
  elif [ "$1" == "M" ]; then
    pro=false
  else
    echo "You're trying to do an automated install but didn't provide the right arguments!"
    exit
  fi
else
  while true; do
      echo -e "What video drivers do you use Mesa(AMD/Intel) or NVidia?"
      read -rp "M(mesa)/N(NVidia): " yn
      case $yn in
          [Mm]* ) pro=false;break;;
          [Nn]* ) pro=true;break;;
          * ) echo "Please answer with M(Mesa) or N(NVidia)!";;
      esac
  done
fi

ping -c 5 google.com

pacman -Syyu --noconfirm
# get the nvidia driver version from glxinfo and then construct a new string with the version
if "${pro}"; then
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
  sed -i 's/IgnorePkg/#IgnorePkg   =/g' /etc/pacman.conf
  pacman -U --noconfirm https://archive.archlinux.org/packages/n/nvidia-utils/nvidia-utils-${nversion}-${up1}-x86_64.pkg.tar.zst https://archive.archlinux.org/packages/l/lib32-nvidia-utils/lib32-nvidia-utils-${nversion}-${up2}-x86_64.pkg.tar.zst
  sed -i 's/#IgnorePkg   =/IgnorePkg = lib32-nvidia-utils nvidia-utils/g' /etc/pacman.conf
fi
