#!/bin/sh
#use as get_externals.sh TWITTERACCOUNT USERNAME PASSWORD
#account is public but i dont wanna share it
mkdir externals && cd externals
wget -O temphtml  https://twitter.com/$1  &&  grep -Eo '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}' temphtml > ips && rm temphtml
read -r ipaddress < ips
wget ftp://$2:$3@$ipaddress/share/pd/externalsARMV7.zip
unzip externalsARMV7.zip -d .
rm externalsARMV7.zip
rm ips
echo "congrats, you got the externals"
