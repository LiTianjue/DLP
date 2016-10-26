#!/bin/bash

key='UpgradeVersionAction'
echo "run it"

echo $1 
echo $2

echo "$1" | grep -q "$key"


if [ $? == 0 ];
then
	echo "Match+++++++++++++++++++++++++++++++++++++++++++++++"
	./mylogger.sh "URI[$1]中匹 配到关键字　$key[$2]" 
else
	echo "NO Match--------------------------------------------"
	#rm -f $1
fi
