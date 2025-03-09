#!/usr/bin/env bash
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mStarting container installation!\x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"

ping -c 5 google.com || ping -c 5 google.com || (echo "error" > /root/error.txt && exit)

pacman -Syyu --noconfirm || pacman -Syu --noconfirm
# Setup 32 bit
sed -i '/^#\[multilib\]$/ {N; s/#\[multilib\]\n#Include = \/etc\/pacman.d\/mirrorlist/\[multilib\]\nInclude = \/etc\/pacman.d\/mirrorlist/g}' /etc/pacman.conf
pacman -Syyu --noconfirm wget vim curl

echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
echo -e "\x1B[32mInstalling drivers!\x1B[0m"
echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"

if [ "$1" != 'N' ]; then
    # TODO: Test this!
    pacman -S --noconfirm mesa lib32-mesa
else
    # Iterate the archive for the newest iteration of the 64 bit nvidia drivers
    up1=1
    for ((;;)); do
        stat1=$(curl -Is "https://archive.archlinux.org/packages/n/nvidia-utils/nvidia-utils-$2-${up1}-x86_64.pkg.tar.zst" | head -n 1)

        if echo "${stat1}" | grep "200" &> /dev/null; then
            ((up1+=1))
        else
            ((up1-=1))
            break;
        fi
    done

    # Iterate the archive for the newest iteration of the 32bit nvidia drivers and libraries
    up2=1
    for ((;;)); do
        stat2=$(curl -Is "https://archive.archlinux.org/packages/l/lib32-nvidia-utils/lib32-nvidia-utils-$2-${up2}-x86_64.pkg.tar.zst" | head -n 1)
        if echo "${stat2}" | grep "200" &> /dev/null; then
            ((up2+=1))
        else
            ((up2-=1))
            break;
        fi
    done

    # Install the nvidia driver and related libraries
    pacman -U --noconfirm "https://archive.archlinux.org/packages/n/nvidia-utils/nvidia-utils-$2-${up1}-x86_64.pkg.tar.zst" "https://archive.archlinux.org/packages/l/lib32-nvidia-utils/lib32-nvidia-utils-$2-${up2}-x86_64.pkg.tar.zst"

    echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"
    echo -e "\x1B[32mInstalling steam, firefox, lutris, wine, winetricks and python!\x1B[0m"
    echo -e "\x1B[32m---------------------------------------------------------------------------\x1B[0m"

    sed -i 's/#IgnorePkg   =/IgnorePkg = lib32-nvidia-utils nvidia-utils/g' /etc/pacman.conf
fi

pacman -S --noconfirm pulseaudio steam lutris firefox wine winetricks python-pip python-pipx python-setuptools python-virtualenv mpv thunar kitty vim ibus git base-devel
# Create an ubuntu user on arch for compatibility reasons lmao
useradd -m ubuntu && usermod -G wheel,audio,video ubuntu
chown ubuntu:ubuntu /home/ubuntu

# Install protontricks trough pipx
pipx install protontricks && pipx ensurepath

sed -i 's/; enable-shm = yes/enable-shm = no/g' /etc/pulse/client.conf
sed -i 's/autospawn = no/autospawn = yes/g' /etc/pulse/client.conf

pacman -S --noconfirm xdg-utils git perl-file-mimeinfo dolphin xwayland weston mutter

# Set locale to en_US
sed -i 's/#en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/g' /etc/locale.gen
locale-gen
localectl set-locale LANG=en_US.UTF-8
unset LANG
source /etc/profile.d/locale.sh

# Adds needed user directories
su ubuntu -c "xdg-user-dirs-update"

# Install some utility client applications
pacman -S --noconfirm dolphin vscode gimp discord teamspeak3 pacman-contrib base-devel reflector rsync papirus-icon-theme breeze-gtk kde-gtk-config
