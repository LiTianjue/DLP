#!/bin/bash

LOG_SERVER="172.16.2.16"
LOG_PORT=514

logger -n $LOG_SERVER -P $LOG_PORT "$1"
