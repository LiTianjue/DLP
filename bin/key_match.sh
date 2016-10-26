#!/bin/bash

key="SSLVPN"

grep $key $1

if [ $? == 0 ];
then
	echo "Match+++++++++++++++++++++++++++++++++++++++++++++++"
else
	echo "NO Match--------------------------------------------"
fi
