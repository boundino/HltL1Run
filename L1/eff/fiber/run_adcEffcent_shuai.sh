#!/bin/bash

RUNS=(
    326776
    326822
    326859
    326942
    327078
    327148
    # -1
)

[[ ${1:-0} -eq 1 || $# == 0 ]] && { g++ adceff_savehist_shuai.cc $(root-config --libs --cflags) -I"$HOME" -g -o adceff_savehist.exe || exit 1 ; }
[[ ${2:-0} -eq 1 || $# == 0 ]] && { g++ adceff_drawhist.cc $(root-config --libs --cflags) -I"$HOME" -g -o adceff_drawhist.exe || exit 1 ; }

for i in ${RUNS[@]}
do
    INPUTFILE="/export/d00/scratch/jwang/L1PbPb2021/shuai/skim_shuai_L1NtupleRAWEMUCalo_HFAdc_smallStat.root,/export/d00/scratch/jwang/L1PbPb2021/shuai/skim_shuai_L1NtupleRAWEMUCalo_HFAdc_smallStat.root,2018 PbPb ZeroBias (${i}),$i"
    IFS=',' ; argv=($INPUTFILE) ; unset IFS ;
    inputfile="${argv[0]} ${argv[1]}"
    tag="${argv[2]}"
    runno=${argv[3]}
    [[ ${1:-0} -eq 1 ]] && { ./adceff_savehist.exe $inputfile "$tag" $runno ; }
    [[ ${2:-0} -eq 1 ]] && { ./adceff_drawhist.exe "$tag" ; }
done

rm adceff_drawhist.exe 2>/dev/null
rm adceff_savehist.exe 2>/dev/null

