#!/bin/bash

make l1mb_savehist || exit 1
make l1mb_drawhist || exit 1
# https://cmsoms.cern.ch/cms/triggers/l1_rates?cms_run=327327&props.11273_11270.selectedCells=L1A%20physics:2

inputs=(
    configs/run375549_replay.conf
)

for ii in ${inputs[@]}
do
    # IFS=',' ; params=(${ii}) ; unset IFS;

    [[ ${1:-0} -eq 1 ]] && ./l1mb_savehist $ii
    [[ ${2:-0} -eq 1 ]] && ./l1mb_drawhist $ii

done

