#!/bin/bash

g++ skim.cc $(root-config --libs --cflags) -I"$HOME" -g -o skim.exe || exit 1

mkdir -p /export/d00/scratch/jwang/L1PbPb2021/crab_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_skim
inputfile=/export/d00/scratch/jwang/L1PbPb2021/crab_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2/L1Ntuple_124.root
outputfile=skim_L1Ntuple_124.root
[[ ${1:-0} -eq 1 ]] && ./skim.exe $inputfile $outputfile

rm skim.exe &> /dev/null
