#!/bin/bash

bropath=.:/usr/local/bro/share/bro:/usr/local/bro/share/bro/policy:/usr/local/bro/share/bro/site

WORK_DIR=/home/andy/GitHub/test/DLP
RULE_DIR=$WORK_DIR/rules

#source /usr/local/bro/share/broctl/scripts/set-bro-path
export BROPATH=${WORK_DIR}:$bropath
#echo $BROPATH

cd $WORK_DIR/log



echo $PWD
echo "Start DLP System"
 
bro -C -i wlan2 -i eth1 $WORK_DIR/dlp.bro 
#bro -C -i eth1 $WORK_DIR/dlp.bro 
