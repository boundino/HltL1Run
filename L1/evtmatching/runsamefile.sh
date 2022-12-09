#!/bin/bash

FOREST=/raid5/data/wangj/L1PbPb2022/crab_adcRAW_221124_HITestRaw0_HIRun2022A_ZBPVfil_362294-6.root
OUTPUT=/raid5/data/wangj/L1PbPb2022/evtmatching_adcRAW_221124_HITestRaw0_HIRun2022A_ZBPVfil_362294-6.root
# FOREST=/raid5/data/wangj/L1PbPb2022/HiForestMiniAOD_362294_100k_ZB_nhfTh4.root
# OUTPUT=/raid5/data/wangj/L1PbPb2022/evtmatching_HiForestMiniAOD_362294_100k_ZB_nhf2Th4.root

tmp=$(date +%y%m%d%H%M%S)

g++ evtmatch_samefile.cc $(root-config --libs --cflags) -g -o evtmatch_samefile_${tmp}.exe || exit 1

./evtmatch_samefile_${tmp}.exe $FOREST $OUTPUT

rm evtmatch_samefile_${tmp}.exe
