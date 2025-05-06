#!/bin/bash

nentries=-1
FOREST=/mnt/submit03/d00/scratch/jwang/L1PbPb2022/crab_forest_20220512_HIForward_HIRun2018A_HIZeroBiasv1_run327238.root
L1=root://cms-xrd-global.cern.ch//store/user/jwang/L1Run2022/crab_l1Ntuple_20220428_HIForward_HIRun2018A_shuaiySkimedZB_run327238.root
OUTPUT=/mnt/submit03/d00/scratch/jwang/L1PbPb2022/evtmatching_forest_20220512_HIForward_HIRun2018A_HIZeroBiasv1_run327238.root

tmp=$(date +%y%m%d%H%M%S)

g++ evtmatch_inverse.cc $(root-config --libs --cflags) -g -o evtmatch_inverse_${tmp}.exe || exit 1

# [[ ! -f $FOREST || ! -f $L1 ]] && { echo "no file: $FOREST $L1" ; exit 1 ; }
./evtmatch_inverse_${tmp}.exe $FOREST $L1 $OUTPUT $nentries

rm evtmatch_inverse_${tmp}.exe
