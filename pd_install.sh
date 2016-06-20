#!/bin/bash

PROCESSOR="$( uname -m)"  #x86_64, armv7l, armv6l checks machine (-p didnt work on my x86)

case $PROCESSOR in
    x86_64 | i686) VERSION=pd-0.46-7.src.tar.gz   # x86 and amd64 and i686
        #STRIPPED="$(sed 's/.gz//' <<< "$VERSION" )"
        ;;
    armv6l) VERSION=pd-0.46-7.armv6.tar.gz # raspi
        #STRIPPED="$(sed 's/.gz//' <<< "$VERSION" )"
        ;;
    armv7l) VERSION=pd-0.46-7.armv7.tar.gz # orangepi H3 boards
        #STRIPPED="$(sed 's/.gz//' <<< "$VERSION" )"
        ;;
    *) VERSION=pd-0.46-7.src.tar.gz  # anything else
        ;;
esac

wget http://msp.ucsd.edu/Software/$VERSION
gzip -d < $VERSION | tar xvf -  # one fell swoop
#gzip -d $VERSION
#tar -xvf $STRIPPED 

sudo apt-get update 
sudo apt-get -y install libasound2 alsa-utils mplayer software-properties-common git autoconf libtool make  build-essential gettext portaudio19-dev  jack libasound-dev qjackctl jackd2 tcl8.6 tk8.6 unzip xauth
sudo apt-get -y build-dep puredata 
sudo ln -s /usr/bin/wish8.6 /usr/bin/wish

sudo alsactl --file .asound.state restore
sudo alsactl store #now save that state to your default alsactl path
cd pd-0.46-7
./autogen.sh
./configure --disable-portaudio --disable-portmidi --no-recursion
make
sudo make install
#autoconf
#make clean
#sudo make -f makefile.gnu install
#sudo make install

cd extra
sudo make install

#add user to audio group if not already (is default on armbian 5.14)
USER="$( logname )"
sudo echo "@audio   -   rtprio  99" >> /etc/security/limits.conf
sudo echo "@audio   -   memlock  unlimited" >> /etc/security/limits.conf
sudo adduser $USER audio

#enable X11Forwarding (default is on in armbian 5.14)
if [ -n "$( grep "X11Forwarding no" /etc/ssh/sshd_config )" ]; then
    sed -i "s/X11Forwarding no/X11Forwarding yes/g" /etc/ssh/sshd_config
fi

#su -u $USER export DISPLAY=localhost:10.0


#now grab a test file for DAC
#should be a sine wave
cd ~
wget http://log.liminastudio.com/wp-content/uploads/2012/06/testPatch.pd_.zip
unzip testPatch.pd_.zip
rm testPatch.pd_.zip
pd -rt -nogui -noadc -alsa testPatch.pd

#if that doesn't output a sine wave
#alsamixer
#and make sure you have the correct alsa device selected and press 'm' to unmute it
#save the settings with 
#sudo alsactl store
