#!/bin/bash

INDEX=(1)

INPUTFILE=(
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias,-1"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326676 326718),-1"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326578),326578"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326628),326628"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326630),326630"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326631),326631"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326632),326632"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326633),326633"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326676),326676"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326717),326717"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (326718),326718"
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_offlinefil.root,/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root,2018 PbPb ZeroBias (327465),327465"
)

[[ ${1:-0} -eq 1 || $# == 0 ]] && { g++ adceff_savehist.cc $(root-config --libs --cflags) -I"$HOME" -g -o adceff_savehist.exe || exit 1 ; }
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


# l1: 326578, 326628, 326630, 326631, 326632, 326633, 326676, 326717, 326718, 327465