#!/bin/bash

INDEX=(0)

INPUTFILE=(
    "/export/d00/scratch/jwang/L1PbPb2021/shuai/shuai_L1NtupleRAWEMUCalo_HFAdc_smallStat_326776.root,/export/d00/scratch/jwang/L1PbPb2021/shuai/shuai_L1NtupleRAWEMUCalo_HFAdc_smallStat_326776.root,2018 PbPb ZeroBias (326776),326776"
)

[[ ${1:-0} -eq 1 || $# == 0 ]] && { g++ adceff_savehist_shuai.cc $(root-config --libs --cflags) -I"$HOME" -g -o adceff_savehist.exe || exit 1 ; }
[[ ${2:-0} -eq 1 || $# == 0 ]] && { g++ adceff_drawhist.cc $(root-config --libs --cflags) -I"$HOME" -g -o adceff_drawhist.exe || exit 1 ; }

for i in ${INDEX[@]}
do
    argv=
    IFS=',' ; argv=(${INPUTFILE[i]}) ; unset IFS
    inputfile="${argv[0]} ${argv[1]}"
    tag="${argv[2]}"
    runno=${argv[3]}
    [[ ${1:-0} -eq 1 ]] && { ./adceff_savehist.exe $inputfile "$tag" $runno ; }
    [[ ${2:-0} -eq 1 ]] && { ./adceff_drawhist.exe "$tag" ; }
done

rm adceff_drawhist.exe 2>/dev/null
rm adceff_savehist.exe 2>/dev/null

