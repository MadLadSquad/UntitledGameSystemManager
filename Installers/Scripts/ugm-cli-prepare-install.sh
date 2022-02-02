#!/bin/bash
if [ "$1" == "--name" ] && [ "$2" != "" ]; then
  user="$2"
else
  read -rp "Enter the name of your regular user: " user

  while true; do
      echo -e "Make sure that you have lxd and lxc installed as well as that you're running this as root!"
      read -rp "Start installation? Y(Yes)/N(No): " yn
      case $yn in
          [Yy]* ) break;;
          [Nn]* ) exit;;
          * ) echo "Please answer with Y(Yes) or N(No)!";;
      esac
  done
fi

# Check if running as root
if [ "$(whoami)" != "root" ]; then
  echo -e "\x1B[31mError: Program not running as root!\x1B[0m"
  exit
fi

id -u "${user}" &> /dev/null || (echo -e "\x1B[31mError: Username ${user} not found!\x1B[0m" && exit)

# Check if lxd and lxc exist
(which lxd &> /dev/null && which lxc &> /dev/null && which pulseaudio &> /dev/null && which X &> /dev/null && which glxinfo &> /dev/null) || (echo -e "\x1B[31mError: Couldn't find one of the following programs in path, install them if you haven't done so or add them to the PATH environment variable if you think they are installed! The following programs are needed:\nLXD, LXC, glxinfo, X, pulseaudio\x1B[0m" && exit)

which rc-update &> /dev/null || which systemctl &> /dev/null || which sv &> /dev/null || (echo -e "\x1B[31mError: Unsupported init system! This script is only compatible with OpenRC, systemd and Runit\x1B[0m" && exit)

# Enable lxd daemon
rc-update add lxd default || systemctl enable lxd.service || ln -s /etc/sv/lxd /var/service || (echo -e "\x1B[31mError: Couldn't enable service, error should be listed above!\x1B[0m" && exit)

# Remove the last line from limits.conf
sed -i '$ d' /etc/security/limits.conf

# Append the following to limits.conf
echo "\
*       soft    nofile  1048576
*       hard    nofile  1048576
root    soft    nofile  1048576
root    hard    nofile  1048576
*       soft    memlock unlimited
*       hard    memlock unlimited
# End of file
" >> /etc/security/limits.conf

# Add the following text to the following files
echo "root:1000000:1000000000" >> /etc/subuid
echo "root:1000000:1000000000" >> /etc/subgid

# Add the user listed by the user to the lxd group
usermod -a -G lxd "${user}"

# Start the lxd service
/etc/init.d/lxd start || systemctl start lxd.service || sv start lxd

su "${user}" -c "(xhost +local: | grep 'access control disabled' || xhost +local:) && exit"

# This complicated line checks if the needed string is there, if it is continue, if it's not add it, restart pulseaudio and continue
(grep "load-module module-native-protocol-unix auth-anonymous=1" /etc/pulse/default.pa &> /dev/null && echo -e "\x1B[32mPulseAudio already installed!\x1B[0m") || (sed -i "s/load-module module-native-protocol-unix/& auth-anonymous=1/" /etc/pulse/default.pa && su "${user}" -c "killall pulseaudio && pulseaudio &> /dev/null & disown && exit")

# Start lxd init with everything being default with the exception of the storage backend
lxd init --auto --storage-backend=dir
# Disable IPv6 because it messes up some configurations' network connectivity
lxc network set lxdbr0 ipv6.nat=false
lxc network set lxdbr0 ipv6.address=none

# Restart LXD so that the network changes can take effect
/etc/init.d/lxd restart || systemctl restart lxd.service || sv restart lxd

echo -e "\x1B[32mPre-install finished successfully!\x1B[0m"
echo -e "\x1B[32mAdd the following path to your PATH environment variable in order for scripts to work: ~/.config/UntitledLinuxGamingManager/scripts/\x1B[0m"
