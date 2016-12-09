export DLP_WORK_DIR=/usr/local/dlp

export DLP_RULE_DIR=${DLP_WORK_DIR}/rules

bropath=.:/usr/local/bro/share/bro:/usr/local/bro/share/bro/policy:/usr/local/bro/share/bro/site

export BROPATH=${DLP_WORK_DIR}:$bropath

export BRO_WORK_DIR=/tmp/bro
