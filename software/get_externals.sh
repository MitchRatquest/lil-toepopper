#!/bin/sh
#use as get_externals.sh TWITTERACCOUNT USERNAME PASSWORD
#account is public but i dont wanna share it
mkdir pd-externals
cd pd-externals
wget -O temphtml  https://twitter.com/$1  &&  grep -Eo '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}' temphtml > ips && rm temphtml
read -r ipaddress < ips
wget ftp://$2:$3@$ipaddress/share/pd/externalsARMV7.zip
unzip externalsARMV7.zip -d .
rm externalsARMV7.zip
rm ips
echo "congrats, you got the externals"
##combine this with the script to generate the declcareALL.pd sketch to correctly name all the stdpath startup options

USER="$( logname )"
ls > allexternals
filename="allexternals"
echo "#N canvas 5 57 450 294 10;" > declareALL.pd #all pd sketches start with this 
while read -r line
do
    name="$line"
    echo "#X declare -stdpath /home/$USER/pd-externals/$name;" >> declareALL.pd
    echo "#X obj 7 34 54 declare -stdpath /home/$USER/pd-externals/$name;" >> declareALL.pd
done < "$filename"
mv declareALL.pd ~/declareALL.pd
rm allexternals
