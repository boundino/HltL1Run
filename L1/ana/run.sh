#!/bin/bash

comp="$(root-config --libs --cflags) -I$HOME"
[[ $# == 0 || ${1:-0} -eq 1 ]] && { g++ l1mb_savehist.cc $comp -g -o l1mb_savehist.exe || exit 1 ; }
[[ $# == 0 || ${2:-0} -eq 1 ]] && { g++ l1mb_drawhist.cc $comp -g -o l1mb_drawhist.exe || exit 1 ; }

declare -A ZBrate
ZBrate["326776"]="6951177"
ZBrate["326822"]="6967215"
ZBrate["327078"]="5146649"
ZBrate["merge"]="6000000"

for run in 326776 326822 327078 merge
do
    input=/raid5/data/wangj/L1PbPb2022/skim_L1NtupleRAWEMUCalo_HFAdc_smallStat_${run}.root
    outputdir=ZB_data_${run}
    tag="2018 ZB (Run ${run})"

    [[ ${1:-0} -eq 1 ]] && ./l1mb_savehist.exe $input $outputdir ${ZBrate[$run]}
    [[ ${2:-0} -eq 1 ]] && ./l1mb_drawhist.exe $outputdir "$tag"

done

rm *.exe
