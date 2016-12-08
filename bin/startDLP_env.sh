#!/bin/bash

mkdir ${BRO_WORK_DIR}
cd ${BRO_WORK_DIR}

echo "Start DLP System"
bro -C -i eth0 ${DLP_WORK_DIR}/dlp.bro  &
