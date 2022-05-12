#!/bin/bash

nentries=-1
FOREST=/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2018/crab_forest_20220501_HIForward_HIRun2018A_HIZeroBiasv1_run327327.root
L1=/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2018/crab_l1Ntuple_20220428_HIForward_HIRun2018A_shuaiySkimedZB_run327327.root
OUTPUT=/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2018/evtmatching_20220501_HIForward_HIRun2018A_HIZeroBiasv1_run327327.root

tmp=$(date +%y%m%d%H%M%S)

g++ evtmatch.cc $(root-config --libs --cflags) -g -o evtmatch_${tmp}.exe || exit 1

[[ ! -f $FOREST || ! -f $L1 ]] && { echo "no file: $FOREST $L1" ; exit 1 ; }
./evtmatch_${tmp}.exe $FOREST $L1 $OUTPUT $nentries

rm evtmatch_${tmp}.exe
