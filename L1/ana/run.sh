#!/bin/bash

comp="$(root-config --libs --cflags) -I$HOME"
[[ $# == 0 || ${1:-0} -eq 1 ]] && { g++ l1mb_savehist.cc $comp -g -o l1mb_savehist.exe || exit 1 ; }
[[ $# == 0 || ${2:-0} -eq 1 ]] && { g++ l1mb_drawhist.cc $comp -g -o l1mb_drawhist.exe || exit 1 ; }

# https://cmsoms.cern.ch/cms/triggers/l1_rates?cms_run=327327&props.11273_11270.selectedCells=L1A%20physics:2

inputs=(
    /raid5/data/wangj/L1PbPb2022/skim_L1NtupleRAWEMUCalo_HFAdc_smallStat_326822.root,configs/run326822.conf,326822
    # /raid5/data/wangj/L1PbPb2022/evtmatching_20220512_HIForward_HIRun2018A_HIZeroBiasv1_run327238.root,configs/run327238.conf,327238
    /raid5/data/wangj/L1PbPb2022/evtmatching_20220512_HIForward_HIRun2018A_HIZeroBiasv1_run327327_fixmatch.root,configs/run327327.conf,327327
)

for ii in ${inputs[@]}
do
    IFS=',' ; params=(${ii}) ; unset IFS;
    input=${params[0]}
    conf=${params[1]}
    run=${params[2]}

    outputdir=ZB_data_${run}
    tag="2018 ZB (Run ${run})"

    echo $input $conf $outputdir $tag

    [[ ${1:-0} -eq 1 ]] && ./l1mb_savehist.exe $input $outputdir $conf
    [[ ${2:-0} -eq 1 ]] && ./l1mb_drawhist.exe $outputdir "$tag"

done

rm *.exe
