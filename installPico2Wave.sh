#!usr/bin/env bash

wget http://ftp.us.debian.org/debian/pool/non-free/s/svox/libttspico0_1.0+git20130326-9_armhf.deb
wget http://ftp.us.debian.org/debian/pool/non-free/s/svox/libttspico-utils_1.0+git20130326-9_armhf.deb
sudo apt-get install -f ./libttspico0_1.0+git20130326-9_armhf.deb
sudo apt-get install -f ./libttspico-utils_1.0+git20130326-9_armhf.deb
rm libttspico-utils_1.0+git20130326-9_armhf.deb
rm libttspico0_1.0+git20130326-9_armhf.deb
