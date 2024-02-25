#!/bin/bash
if [ "$1" == "--name" ] && [ "$2" != "" ]; then
  user="$2"
else
  read -rp "Enter the name of your regular user: " user

  while true; do
      echo -e "Make sure that you have incus and lxc installed as well as that you're running this as root!"
      read -rp "Start installation? Y(Yes)/N(No): " yn
      case $yn in
          [Yy]* ) break;;
          [Nn]* ) exit;;
          * ) echo "Please answer with Y(Yes) or N(No)!";;
      esac
  done
fi

# Enable Incus daemon
rc-update add incus default || systemctl enable incus.service

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

# Add the user listed by the user to the incus group
usermod -a -G incus,incus-admin "${user}"

# Start the incus service
/etc/init.d/incus start || systemctl start incus.service

su "${user}" -c "(xhost +local: | grep 'access control disabled' || xhost +local:) && exit"

# This complicated line checks if the needed string is there, if it is continue, if it's not add it, restart pulseaudio and continue
(grep "load-module module-native-protocol-unix auth-anonymous=1" /etc/pulse/default.pa &> /dev/null && echo -e "\x1B[32mPulseAudio already installed!\x1B[0m") || (sed -i "s/load-module module-native-protocol-unix/& auth-anonymous=1/" /etc/pulse/default.pa && su "${user}" -c "killall pulseaudio && pulseaudio &> /dev/null & disown && exit")

# Start incus init with everything being default with the exception of the storage backend
incus admin init --auto --storage-backend=dir
# Disable IPv6 because it messes up some configurations' network connectivity
incus network set incusbr0 ipv6.nat=false
incus network set incusbr0 ipv6.address=none

# Restart Incus so that the network changes can take effect
/etc/init.d/incus restart || systemctl restart incus.service

echo -e "\x1B[32mPre-install finished successfully!\x1B[0m"
echo -e "\x1B[32mAdd the following path to your PATH environment variable in order for scripts to work: ~/.config/UntitledLinuxGamingManager/scripts/\x1B[0m"
