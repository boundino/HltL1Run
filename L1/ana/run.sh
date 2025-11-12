#!/bin/bash

make l1mb_savehist || exit 1
make l1mb_drawhist || exit 1
# https://cmsoms.cern.ch/cms/triggers/l1_rates?cms_run=327327&props.11273_11270.selectedCells=L1A%20physics:2

conf=$1

if [[ $conf == *conf && -f $conf ]] ; then
    [[ ${2:-0} -eq 1 ]] && ./l1mb_savehist $conf
    [[ ${3:-0} -eq 1 ]] && ./l1mb_drawhist $conf
else
    echo "usage: ./run.sh configs/XXX.conf ([run_save]) ([run_draw])"
    echo "example: ./run.sh configs/run388095.conf 1 1"
fi

