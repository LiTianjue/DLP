#!/bin/bash

key='客户\|策略管理中心\|WebXml'

file=$1;

ext_name=${file##*.}

out=$(grep -a $key $1)

if [ $? == 0 ];
then
	echo "Match+++++++++++++++++++++++++++++++++++++++++++++++"
	if [[ $ext_name == "bin" ]]
	then
		./mylogger.sh "[文本] 匹配到关键字 [$out]"
	else
		./mylogger.sh "[$ext_name] 匹配到关键字 [$out]"
	fi
else
	echo "NO Match--------------------------------------------"
	#rm -f $1
fi
