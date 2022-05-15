#!/bin/bash

nentries=-1
L1=/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2018/crab_l1Ntuple_20220428_HIForward_HIRun2018A_shuaiySkimedZB_run327327.root
OUTPUT=/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2018/plain_l1Ntuple_20220501_HIForward_HIRun2018A_HIZeroBiasv1_run327327.root

tmp=$(date +%y%m%d%H%M%S)

g++ plainl1evt.cc $(root-config --libs --cflags) -g -o plainl1evt_${tmp}.exe || exit 1

[[ ! -f $L1 ]] && { echo "no file: $FOREST $L1" ; exit 1 ; }
./plainl1evt_${tmp}.exe $L1 $OUTPUT $nentries

rm plainl1evt_${tmp}.exe
