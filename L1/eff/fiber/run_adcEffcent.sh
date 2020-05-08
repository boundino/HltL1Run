#!/bin/bash

INDEX=(0)

INPUTFILE=(
    "/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_evt1000000.root"
)
TAG=(
    "2018 PbPb ZeroBias"
)

[[ ${1:-0} -eq 1 || $# == 0 ]] && { g++ adceff_savehist.cc $(root-config --libs --cflags) -I"$HOME" -g -o adceff_savehist.exe || exit 1 ; }
[[ ${2:-0} -eq 1 || $# == 0 ]] && { g++ adceff_drawhist.cc $(root-config --libs --cflags) -I"$HOME" -g -o adceff_drawhist.exe || exit 1 ; }
[[ ${3:-0} -eq 1 || $# == 0 ]] && { g++ draw_fiber.cc $(root-config --libs --cflags) -I"$HOME" -g -o draw_fiber.exe || exit 1 ; }

[[ ${1:-0} -eq 1 ]] && { for i in ${INDEX[@]} ; do { ./adceff_savehist.exe ${INPUTFILE[i]} "${TAG[i]}" ; } ; done ; }
[[ ${2:-0} -eq 1 ]] && { for i in ${INDEX[@]} ; do { ./adceff_drawhist.exe "${TAG[i]}" ; } ; done ; }
[[ ${3:-0} -eq 1 ]] && { ./draw_fiber.exe "/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root" "${TAG[0]}" ; }

rm draw_fiber.exe
rm adceff_drawhist.exe
rm adceff_savehist.exe