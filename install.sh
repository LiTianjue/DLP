#!/bin/bash

WD=`pwd`

INSTALL_ROOT=/usr/local/dlp

cd ${WD}/managment

make 
make install

cp ${WD}/bin/startDLP_env.sh /usr/bin/startDLP.sh


cp ${WD}/bin/SetEnv/dlp_profile.sh /etc/profile.d/bro.sh


mkdir -p ${INSTALL_ROOT}
cp ${WD}/rules ${INSTALL_ROOT} -r
cp ${WD}/dlp.bro ${INSTALL_ROOT}
