#!/bin/bash

WD=`pwd`

dpkg -l | awk '{print $2}' | sort > old.txt

# 1. 安装依赖环境
apt-get update && apt-get upgrade -y && echo `date`

apt-get -y install build-essential git bison flex gawk cmake swig libssl-dev libgeoip-dev libpcap-dev python-dev libcurl4-openssl-dev wget libncurses5-dev ca-certificates --no-install-recommends



# 2. 下载编译Bro

VER=2.5

${WD}/common/buildbro ${VER} http://www.bro.org/downloads/bro-${VER}.tar.gz
ln -s /usr/local/bro-${VER} /usr/local//bro




#
## Final setup stuff
#

cp ${WD}/common/bro_profile.sh /etc/profile.d/bro.sh
suorce /etc/profile.d/bro.sh

#
## Cleanup, so docker-squash can do it's thing
#
dpkg -l | awk '{print $2}' | sort > new.txt
apt-get clean 

rm -rf /var/lib/apt/lists/* /tmp/*

