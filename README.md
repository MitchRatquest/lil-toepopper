# lil-toepopper
Digital audio processing unit in a guitar pedal interface with linux and puredata

![alt text](https://github.com/MitchRatquest/lil-toepopper/blob/master/pedalIPaddress.gif?raw=true "demo of ip address")


To install the basics in linux, run pd_install.sh
Its helpful to have either xming and putty set up for X11 forwarding on Windows or use ssh -X

Putty's settings are Connection>SSH>X11: Enable X11, X display location localhost:0, and MIT-Magic-Cookie-1.

Still a work in progress. 

To install, run pd_install.sh, then run get_externals.sh, and finally load your pd sketch with declareALL.pd to have the libraries working correctly. 
